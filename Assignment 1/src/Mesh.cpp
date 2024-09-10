#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> Vertices, std::vector<unsigned int> Indices, std::vector<Texture> Textures)
	: Vertices(std::move(Vertices)), Indices(std::move(Indices)), Textures(std::move(Textures))
{
	setupMesh();
}

void Mesh::draw(const Shader& Shader) const
{
	unsigned int DiffuseNr = 1;
	unsigned int SpecularNr = 1;
	for (unsigned int I = 0; I < Textures.size(); I++)
	{
		glActiveTexture(GL_TEXTURE0 + I);
		std::string Number;
		std::string Name = Textures[I].Type;
		if (Name == "texture_diffuse")
			Number = std::to_string(DiffuseNr++);
		else if (Name == "texture_specular")
			Number = std::to_string(SpecularNr++);

		Shader.setInt((Name + Number).c_str(), I);
		glBindTexture(GL_TEXTURE_2D, Textures[I].Id);
	}

	glBindVertexArray(Vao);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(Indices.size()), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);

	glActiveTexture(GL_TEXTURE0);
}

void Mesh::setupMesh()
{
	glGenVertexArrays(1, &Vao);
	glGenBuffers(1, &Vbo);
	glGenBuffers(1, &Ebo);

	glBindVertexArray(Vao);
	glBindBuffer(GL_ARRAY_BUFFER, Vbo);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), Vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned int), Indices.data(), GL_STATIC_DRAW);

	// Vertex positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(nullptr));
	// Vertex normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
	// Vertex texture coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}
