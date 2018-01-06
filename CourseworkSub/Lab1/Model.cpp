#include "Model.h"
#include <vector>


void Model::InitializeMesh(Vertex* vertices, unsigned int numVertices, unsigned int* indices, unsigned int numIndices)
{
	IndexedModel model;

	for (unsigned int i = 0; i < numVertices; i++)
	{
		model.m_Positions.push_back(*vertices[i].GetPosition());
		model.m_TextureCoords.push_back(*vertices[i].GetTextureCoord());

	}

	for (unsigned int i = 0; i < numIndices; i++)
		model.m_Indices.push_back(indices[i]);

	InitializeModel(model);

}

void Model::InitializeModel(const IndexedModel& model)
{
	
	m_DrawCount = model.m_Indices.size();

	glGenVertexArrays(1, &m_VertexArrayObject); //generate a vertex array and store it in the VAO
	glBindVertexArray(m_VertexArrayObject); //bind the VAO (any operation that works on a VAO will work on our bound VAO - binding)

	glGenBuffers(NUM_BUFFERS, m_VertexArrayBuffers); //generate our buffers based of our array of data/buffers - GLuint m_VertexArrayBuffers[NUM_BUFFERS];
	
	glBindBuffer(GL_ARRAY_BUFFER, m_VertexArrayBuffers[POSITION_VERTEXBUFFER]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, model.m_Positions.size() * sizeof(model.m_Positions[0]), &model.m_Positions[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU (determined by type)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexArrayBuffers[TEXCOORD_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ARRAY_BUFFER, model.m_Positions.size() * sizeof(model.m_TextureCoords[0]), &model.m_TextureCoords[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_VertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.m_Normals[0]) * model.m_Normals.size(), &model.m_Normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VertexArrayBuffers[INDEX_VB]); //tell opengl what type of data the buffer is (GL_ARRAY_BUFFER), and pass the data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, model.m_Indices.size() * sizeof(model.m_Indices[0]), &model.m_Indices[0], GL_STATIC_DRAW); //move the data to the GPU - type of data, size of data, starting address (pointer) of data, where do we store the data on the GPU

	glBindVertexArray(0); // unbind our VAO
}

Model::Model()
{
	m_DrawCount = NULL;
}

void Model::LoadModel(const std::string& filename)
{
	IndexedModel model = ModelObject(filename).ToIndexedModel();
	InitializeModel(model);
}

Model::~Model()
{
	glDeleteVertexArrays(1, &m_VertexArrayObject); // delete arrays
}

void Model::DrawMesh()
{
	glBindVertexArray(m_VertexArrayObject);
	
	glDrawElements(GL_TRIANGLES, m_DrawCount, GL_UNSIGNED_INT, 0);
	//glDrawArrays(GL_TRIANGLES, 0, m_DrawCount);
	
	glBindVertexArray(0);
}

