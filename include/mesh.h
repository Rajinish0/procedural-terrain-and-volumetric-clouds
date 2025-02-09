#ifndef MESH_HEADER
#define MESH_HEADER 


#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


struct Vertex {
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoord;

};

struct Material{
	float opacity = 1.0f;
};

struct Texture {
	enum Type{
		DIFFUSE,
		SPECULAR
	};
	unsigned int id;
	Type type;
	std::string path;

	Texture() 
		:id(0) {};
	Texture(unsigned int id, Type type, std::string path)
		:id(id), type(type), path(path) {}
	Texture(std::string path, std::string directory, Type type, 
			GLuint S_WRAP = GL_REPEAT, 
			GLuint T_WRAP = GL_REPEAT);
	Texture(std::string completePath, Type type,
			GLuint S_WRAP = GL_REPEAT, 
			GLuint T_WRAP = GL_REPEAT);
};


class Mesh {
public:
	std::vector<Vertex> verticies;
	std::vector<Texture> textures;
	std::vector<unsigned int> indicies;
	Material material;
	
	Mesh(std::vector<Vertex> verticies, std::vector<Texture> textures, std::vector<unsigned int> indicies, Material);
	Mesh(std::vector<Vertex> verticies, std::vector<Texture> textures, std::vector<unsigned int> indicies);
	Mesh(Mesh&&) noexcept;

	Mesh(const Mesh&) = default;
	Mesh& operator=(const Mesh&) = default;

	Mesh& operator=(Mesh&&) noexcept;
	void draw(Shader& shader);
	unsigned int VAO, VBO, EBO;
 	/*
	 * TO DO:
	 * 	this is being called unexpectedly sometimes, need to 
	 * 	debug this.
	 */
//	~Mesh();

private:
	void setUpMesh();
	void clearResources();
};

#endif
