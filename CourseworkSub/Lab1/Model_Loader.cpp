#include "Model_Loader.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

static bool CompareModelIndexPtr(const ModelIndex* a, const ModelIndex* b);
static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token);
static inline unsigned int ParseModelIndexValue(const std::string& token, unsigned int start, unsigned int end);
static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end);
static inline std::vector<std::string> SplitString(const std::string &s, char delim);

ModelObject::ModelObject(const std::string& fileName)
{
	m_HasUVs = false;
	m_HasNormals = false;
    std::ifstream file;
    file.open(fileName.c_str());

    std::string line;
    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
        
            unsigned int lineLength = line.length();
            
            if(lineLength < 2)
                continue;
            
            const char* lineCStr = line.c_str();
            
            switch(lineCStr[0])
            {
                case 'v':
                    if(lineCStr[1] == 't')
                        this->m_UVs.push_back(ParseModelVec2(line));
                    else if(lineCStr[1] == 'n')
                        this->m_Normals.push_back(ParseModelVec3(line));
                    else if(lineCStr[1] == ' ' || lineCStr[1] == '\t')
                        this->m_Vertices.push_back(ParseModelVec3(line));
                break;
                case 'f':
                    CreateModelFace(line);
                break;
                default: break;
            };
        }
    }
    else
    {
        std::cerr << "Unable to load mesh: " << fileName << std::endl;
    }
}

void IndexedModel::CalculateNormals()
{
    for(unsigned int i = 0; i < m_Indices.size(); i += 3)
    {
        int i0 = m_Indices[i];
        int i1 = m_Indices[i + 1];
        int i2 = m_Indices[i + 2];

        glm::vec3 v1 = m_Positions[i1] - m_Positions[i0];
        glm::vec3 v2 = m_Positions[i2] - m_Positions[i0];
        
        glm::vec3 normal = glm::normalize(glm::cross(v1, v2));
            
        m_Normals[i0] += normal;
        m_Normals[i1] += normal;
        m_Normals[i2] += normal;
    }
    
    for(unsigned int i = 0; i < m_Positions.size(); i++)
        m_Normals[i] = glm::normalize(m_Normals[i]);
}

IndexedModel ModelObject::ToIndexedModel()
{
    IndexedModel result;
    IndexedModel normalModel;
    
    unsigned int numIndices = m_ModelIndices.size();
    
    std::vector<ModelIndex*> indexLookup;
    
    for(unsigned int i = 0; i < numIndices; i++)
        indexLookup.push_back(&m_ModelIndices[i]);
    
    std::sort(indexLookup.begin(), indexLookup.end(), CompareModelIndexPtr);
    
    std::map<ModelIndex, unsigned int> normalModelIndexMap;
    std::map<unsigned int, unsigned int> indexMap;
    
    for(unsigned int i = 0; i < numIndices; i++)
    {
        ModelIndex* currentIndex = &m_ModelIndices[i];
        
        glm::vec3 currentPosition = m_Vertices[currentIndex->m_VertexIndex];
        glm::vec2 currentTexCoord;
        glm::vec3 currentNormal;
        
        if(m_HasUVs)
            currentTexCoord = m_UVs[currentIndex->m_UVIndex];
        else
            currentTexCoord = glm::vec2(0,0);
            
        if(m_HasNormals)
            currentNormal = m_Normals[currentIndex->m_NormalIndex];
        else
            currentNormal = glm::vec3(0,0,0);
        
        unsigned int normalModelIndex;
        unsigned int resultModelIndex;
        
        //Create model to properly generate m_Normals on
        std::map<ModelIndex, unsigned int>::iterator it = normalModelIndexMap.find(*currentIndex);
        if(it == normalModelIndexMap.end())
        {
            normalModelIndex = normalModel.m_Positions.size();
        
            normalModelIndexMap.insert(std::pair<ModelIndex, unsigned int>(*currentIndex, normalModelIndex));
            normalModel.m_Positions.push_back(currentPosition);
            normalModel.m_TextureCoords.push_back(currentTexCoord);
            normalModel.m_Normals.push_back(currentNormal);
        }
        else
            normalModelIndex = it->second;
        
        //Create model which properly separates texture coordinates
        unsigned int previousVertexLocation = FindLastVertexIndex(indexLookup, currentIndex, result);
        
        if(previousVertexLocation == (unsigned int)-1)
        {
            resultModelIndex = result.m_Positions.size();
        
            result.m_Positions.push_back(currentPosition);
            result.m_TextureCoords.push_back(currentTexCoord);
            result.m_Normals.push_back(currentNormal);
        }
        else
            resultModelIndex = previousVertexLocation;
        
        normalModel.m_Indices.push_back(normalModelIndex);
        result.m_Indices.push_back(resultModelIndex);
        indexMap.insert(std::pair<unsigned int, unsigned int>(resultModelIndex, normalModelIndex));
    }
    
    if(!m_HasNormals)
    {
        normalModel.CalculateNormals();
        
        for(unsigned int i = 0; i < result.m_Positions.size(); i++)
            result.m_Normals[i] = normalModel.m_Normals[indexMap[i]];
    }
    
    return result;
};

unsigned int ModelObject::FindLastVertexIndex(const std::vector<ModelIndex*>& indexLookup, const ModelIndex* currentIndex, const IndexedModel& result)
{
    unsigned int start = 0;
    unsigned int end = indexLookup.size();
    unsigned int current = (end - start) / 2 + start;
    unsigned int previous = start;
    
    while(current != previous)
    {
        ModelIndex* testIndex = indexLookup[current];
        
        if(testIndex->m_VertexIndex == currentIndex->m_VertexIndex)
        {
            unsigned int countStart = current;
        
            for(unsigned int i = 0; i < current; i++)
            {
                ModelIndex* possibleIndex = indexLookup[current - i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->m_VertexIndex != currentIndex->m_VertexIndex)
                    break;
                    
                countStart--;
            }
            
            for(unsigned int i = countStart; i < indexLookup.size() - countStart; i++)
            {
                ModelIndex* possibleIndex = indexLookup[current + i];
                
                if(possibleIndex == currentIndex)
                    continue;
                    
                if(possibleIndex->m_VertexIndex != currentIndex->m_VertexIndex)
                    break;
                else if((!m_HasUVs || possibleIndex->m_UVIndex == currentIndex->m_UVIndex) 
                    && (!m_HasNormals || possibleIndex->m_NormalIndex == currentIndex->m_NormalIndex))
                {
                    glm::vec3 currentPosition = m_Vertices[currentIndex->m_VertexIndex];
                    glm::vec2 currentTexCoord;
                    glm::vec3 currentNormal;
                    
                    if(m_HasUVs)
                        currentTexCoord = m_UVs[currentIndex->m_UVIndex];
                    else
                        currentTexCoord = glm::vec2(0,0);
                        
                    if(m_HasNormals)
                        currentNormal = m_Normals[currentIndex->m_NormalIndex];
                    else
                        currentNormal = glm::vec3(0,0,0);
                    
                    for(unsigned int j = 0; j < result.m_Positions.size(); j++)
                    {
                        if(currentPosition == result.m_Positions[j] 
                            && ((!m_HasUVs || currentTexCoord == result.m_TextureCoords[j])
                            && (!m_HasNormals || currentNormal == result.m_Normals[j])))
                        {
                            return j;
                        }
                    }
                }
            }
        
            return -1;
        }
        else
        {
            if(testIndex->m_VertexIndex < currentIndex->m_VertexIndex)
                start = current;
            else
                end = current;
        }
    
        previous = current;
        current = (end - start) / 2 + start;
    }
    
    return -1;
}

void ModelObject::CreateModelFace(const std::string& line)
{
    std::vector<std::string> tokens = SplitString(line, ' ');

    this->m_ModelIndices.push_back(ParseModelIndex(tokens[1], &this->m_HasUVs, &this->m_HasNormals));
    this->m_ModelIndices.push_back(ParseModelIndex(tokens[2], &this->m_HasUVs, &this->m_HasNormals));
    this->m_ModelIndices.push_back(ParseModelIndex(tokens[3], &this->m_HasUVs, &this->m_HasNormals));

    if((int)tokens.size() > 4)
    {
        this->m_ModelIndices.push_back(ParseModelIndex(tokens[1], &this->m_HasUVs, &this->m_HasNormals));
        this->m_ModelIndices.push_back(ParseModelIndex(tokens[3], &this->m_HasUVs, &this->m_HasNormals));
        this->m_ModelIndices.push_back(ParseModelIndex(tokens[4], &this->m_HasUVs, &this->m_HasNormals));
    }
}

ModelIndex ModelObject::ParseModelIndex(const std::string& token, bool* hasUVs, bool* hasNormals)
{
    unsigned int tokenLength = token.length();
    const char* tokenString = token.c_str();
    
    unsigned int vertIndexStart = 0;
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    ModelIndex result;
    result.m_VertexIndex = ParseModelIndexValue(token, vertIndexStart, vertIndexEnd);
    result.m_UVIndex = 0;
    result.m_NormalIndex = 0;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.m_UVIndex = ParseModelIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasUVs = true;
    
    if(vertIndexEnd >= tokenLength)
        return result;
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, '/');
    
    result.m_NormalIndex = ParseModelIndexValue(token, vertIndexStart, vertIndexEnd);
    *hasNormals = true;
    
    return result;
}

glm::vec3 ModelObject::ParseModelVec3(const std::string& line) 
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 2;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float z = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec3(x,y,z);

    //glm::vec3(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()))
}

glm::vec2 ModelObject::ParseModelVec2(const std::string& line)
{
    unsigned int tokenLength = line.length();
    const char* tokenString = line.c_str();
    
    unsigned int vertIndexStart = 3;
    
    while(vertIndexStart < tokenLength)
    {
        if(tokenString[vertIndexStart] != ' ')
            break;
        vertIndexStart++;
    }
    
    unsigned int vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float x = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    vertIndexStart = vertIndexEnd + 1;
    vertIndexEnd = FindNextChar(vertIndexStart, tokenString, tokenLength, ' ');
    
    float y = ParseOBJFloatValue(line, vertIndexStart, vertIndexEnd);
    
    return glm::vec2(x,y);
}

static bool CompareModelIndexPtr(const ModelIndex* a, const ModelIndex* b)
{
    return a->m_VertexIndex < b->m_VertexIndex;
}

static inline unsigned int FindNextChar(unsigned int start, const char* str, unsigned int length, char token)
{
    unsigned int result = start;
    while(result < length)
    {
        result++;
        if(str[result] == token)
            break;
    }
    
    return result;
}

static inline unsigned int ParseModelIndexValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atoi(token.substr(start, end - start).c_str()) - 1;
}

static inline float ParseOBJFloatValue(const std::string& token, unsigned int start, unsigned int end)
{
    return atof(token.substr(start, end - start).c_str());
}

static inline std::vector<std::string> SplitString(const std::string &s, char delim)
{
    std::vector<std::string> elems;
        
    const char* cstr = s.c_str();
    unsigned int strLength = s.length();
    unsigned int start = 0;
    unsigned int end = 0;
        
    while(end <= strLength)
    {
        while(end <= strLength)
        {
            if(cstr[end] == delim)
                break;
            end++;
        }
            
        elems.push_back(s.substr(start, end - start));
        start = end + 1;
        end = start;
    }
        
    return elems;
}
