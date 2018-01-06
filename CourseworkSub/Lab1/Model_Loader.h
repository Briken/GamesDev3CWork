#ifndef OBJ_LOADER_H_INCLUDED
#define OBJ_LOADER_H_INCLUDED

#include <glm/glm.hpp>
#include <vector>
#include <string>

struct ModelIndex
{
    unsigned int m_VertexIndex;
    unsigned int m_UVIndex;
    unsigned int m_NormalIndex;
    
    bool operator<(const ModelIndex& r) const { return m_VertexIndex < r.m_VertexIndex; }
};

class IndexedModel
{
public:
    std::vector<glm::vec3> m_Positions;
    std::vector<glm::vec2> m_TextureCoords;
    std::vector<glm::vec3> m_Normals;
    std::vector<unsigned int> m_Indices;
    
    void CalculateNormals();
};

class ModelObject
{
public:
    std::vector<ModelIndex> m_ModelIndices;
    std::vector<glm::vec3> m_Vertices;
    std::vector<glm::vec2> m_UVs;
    std::vector<glm::vec3> m_Normals;
    bool m_HasUVs;
    bool m_HasNormals;
    
    ModelObject(const std::string& fileName);
    
    IndexedModel ToIndexedModel();
private:
    unsigned int FindLastVertexIndex(const std::vector<ModelIndex*>& indexLookup, const ModelIndex* currentIndex, const IndexedModel& result);
    void CreateModelFace(const std::string& line);
    
    glm::vec2 ParseModelVec2(const std::string& line);
    glm::vec3 ParseModelVec3(const std::string& line);
    ModelIndex ParseModelIndex(const std::string& token, bool* hasUVs, bool* hasNormals);
};

#endif // OBJ_LOADER_H_INCLUDED
