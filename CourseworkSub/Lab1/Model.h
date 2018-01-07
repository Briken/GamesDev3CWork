#pragma once
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <string>
#include "Model_Loader.h"
#include "Collider.h"


struct Vertex
{
public:
	Vertex(const glm::vec3& pos, const glm::vec2& texCoord)
	{
		this->m_Position = pos;
		this->m_TextureCoord = texCoord;
		this->m_Normal = m_Normal;
	}

	glm::vec3* GetPosition() { return &m_Position; }
	glm::vec2* GetTextureCoord() { return &m_TextureCoord; }

	
private:
	glm::vec3 m_Position;
	glm::vec2 m_TextureCoord;
	glm::vec3 m_Normal;
};

class Model
{
public:
	Model();
	~Model();
	

	//each function does as its name would suggest
	void DrawMesh(); //draws the loaded mesh
	void InitializeMesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices);//initiaises the mesh before drawing
	void LoadModel(const std::string& filename);//loads the model from a filename
	void InitializeModel(const IndexedModel& model);//pulls the attributes from the model
	
private:



	enum
	{
		POSITION_VERTEXBUFFER,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,
		NUM_BUFFERS
	};

	GLuint m_VertexArrayObject;
	GLuint m_VertexArrayBuffers[NUM_BUFFERS]; // create our array of buffers
	unsigned int m_DrawCount; //how much of the vertexArrayObject do we want to DrawMesh
};

