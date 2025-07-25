#ifndef FUNCS_GUARD
#define FUNCS_GUARD
#include <iostream>
#include <string>
#include "mesh.h"
#include "perlin.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace funcs {
	unsigned int TextureFromFile(const std::string& path, std::string directory, 
								 GLuint S_WRAP = GL_REPEAT, 
								 GLuint T_WRAP = GL_REPEAT);


	unsigned int TextureFromFile(const std::string& path, GLuint S_WRAP = GL_REPEAT, 
								GLuint T_WRAP = GL_REPEAT);


	unsigned int loadCubeMap(std::vector<std::string> faces);
	Mesh genSphere();
	Mesh genTorus(); // or donut?

	template<typename T>
	T clamp(T val, T lowerb, T upperb){
		if (val < lowerb) return lowerb;
		if (val > upperb) return upperb;
		return val;
	}

	int flatten(int i, int j, int width);



	template<typename... Textures>
	Mesh genPlane(int numBlocks, float totalSz, const Textures... diffuseTextures){
		Perlin2d perlin;
		std::vector<Vertex> vertices;
		std::vector<Texture> textures;
		std::vector<unsigned int> indices;

		float bsz = totalSz / (float) (numBlocks - 1);
		float startx = -totalSz / 2.0f;
		float starty = totalSz / 2.0f;

		for (int i =0; i < numBlocks; ++i){
			for (int j =0; j < numBlocks; ++j){
				Vertex v;
				float x = startx + bsz * j;
				float y = starty - bsz * i;
				v.position = glm::vec3( x, y, perlin.perlin((x + totalSz / 2.0f)/2.0f , (y - totalSz)/2.0f)/2.0f);
				v.normal   = glm::vec3(0.0f, 0.0f, 1.0f);
				v.texCoord = glm::vec2( ( (float) j )/ ( (float)(numBlocks - 1)) , ( (float) (numBlocks - i - 1) ) / ( (float) (numBlocks - 1) ) );
				// v.texCoord *= 20.0f; // (using wrapping mode as GL_REPEAT)

				vertices.push_back(v);
			}
		}

		for (unsigned int i =0; i < numBlocks - 1; ++i){
			for (unsigned int j =0; j < numBlocks - 1; ++j){
				indices.push_back( flatten(i, j, numBlocks) );
				indices.push_back( flatten(i, j+1, numBlocks) );
				indices.push_back( flatten(i + 1, j, numBlocks) );


				indices.push_back( flatten(i, j+1, numBlocks) );
				indices.push_back( flatten(i + 1, j, numBlocks) );
				indices.push_back( flatten(i + 1, j + 1, numBlocks) );
			}
		}

		for (const auto& tPath : {diffuseTextures...})
			textures.emplace_back(tPath, Texture::DIFFUSE, GL_REPEAT, GL_REPEAT);

		for (Texture t : textures)
			std::cout << t.path << std::endl;
		//this fold syntax is a little too mind bending for me rn, i'm sticking with the loop rn
		// (textures.emplace_back(diffuseTextures, Texture::DIFFUSE), ...);

		return Mesh(vertices, textures, indices);
	}
	Mesh genPlane2(int size, int LOD);

	GLuint genWorleyNoise(unsigned int width, unsigned int height, unsigned int breadth, unsigned int num_tiles = 10);
	GLuint loadWeatherData(std::string fname);
	GLuint loadDetailTexture(std::string fname);
	GLuint loadGeneric3dTexture(std::string fname);
	GLuint loadGeneric2dTexture(std::string fname);
	glm::vec2 genRandomCoords2d(float min = -5000.0f, 
						 	  	float max =  5000.0f);


	template<typename T>
	T lerp(T lower, T upper, T percent){
		return lower + (upper - lower) * percent;
	}

	template<typename T>
	T remap(T val, T lower, T upper, T newLower, T newUpper){
		// return newLower + (newUpper - newLower)*(val - lower)/(upper-lower);
		return lerp(newLower, newUpper, (val-lower)/(upper-lower));
	}

}

#endif
