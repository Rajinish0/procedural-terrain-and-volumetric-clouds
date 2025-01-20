/*
* FOR EXPLORING INSTANCING
*/


#include <iostream>
#include "shader.h"
#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "perlin.h"
//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif
#include "stb_image.h"
#include "funcs.h"
#include "window.h"
#include "model.h"
#include "plane.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "audio_manager.h"
#include "terrain.h"
#include "framebuffer.h"
#include "endless_terrain.h"
#include <ft2build.h> // checking if build was good


//#define DRAW_NORMALS


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);


float vertices[] = {
	// positions          // texture Coords
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

float planeVertices[] = {
	// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

	 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
	-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
	 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
};


/*
* just realized this is kind of stupid, because
* i could have used planeVertices and passed 6 instead of
* 36 in glDrawArrays
*/
float grassVertices[]{
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f
};

float quadvertices[]{
	-1.0f, -1.0f, 0.0f, 0.0f,
	 1.0f, -1.0f, 1.0f, 0.0f,
	 1.0f,  1.0f, 1.0f, 1.0f,
	 1.0f,  1.0f, 1.0f, 1.0f,
	-1.0f,  1.0f, 0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f, 0.0f
};

float simpleQuad[]{
	-0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
	 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
	-0.5f,-0.5f, 0.0f, 0.0f, 1.0f,


	-0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
	 0.5f,-0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f, 0.5f, 0.0f, 1.0f, 1.0f,
};

unsigned int indicies[] = {
	0, 1, 2,
	0, 2, 3
};

glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};



unsigned int cubeVBO, cubeVAO, planeVAO, planeVBO,
grassVAO, grassVBO, fbo, rbo, quadVAO, uboBlock,
quadVBO, squareVAO, squareVBO, instanceVBO,
asteroidVBO;
unsigned int vertexShader;
unsigned int fragShader;
unsigned int texture, texture2,
grassTexture, fbtexture;
float dt = 0.007f;
float lastTime = NULL;
float moveFac = 0.0f;
float moveVel = 0.015f;
// float xOff = 0.0f, yOff = 0.0f;

float densityThreshold = .420003f;
float scale 		   = .595005f;
float weatherScale     = .0001;
float higherScale 	   = .375008;
glm::vec3 offSet(0.0f);

Camera cam;
std::vector<glm::vec3> vegetation;
Perlin2d perlin(256, 4);
Plane *plane;
Terrain *terr;

float terrainSize = 10.0f;

unsigned int FBO, RBO, textureId;


// Perlin2d perlin(256, 8);

// Mesh genPlane(int size, std::vector<float> heightmap){
// 	std::vector<Vertex> vertices;
// 	std::vector<Texture> textures;
// 	std::vector<unsigned int> indicies;

// 	float tlX = (size - 1)/-2.0f;
// 	float tlY = (size - 1)/2.0f;

// 	int LOD = 1;
// 	int sz = (size - 1) / LOD  + 1;
// 	std::cout << "SZ: " << sz << std::endl;

// 	float x, y;

// 	for (unsigned int i =0; i < size; i+=LOD){
// 		for (unsigned int j =0; j < size; j+=LOD){
// 			Vertex v;
// 			// std::cout << perlin.perlin(j, i) << std::endl;
// 			x = (tlX + (float)j)/(float(size));
// 			y = (tlY - (float)i)/(float(size));
// 			v.position = glm::vec3(tlX + (float)j, heightmap[i*size + j], tlY - (float)i);
// 			v.normal   = glm::vec3(0.0f, 0.0f, 1.0f);
// 			v.texCoord = glm::vec2( ( (float) j )/ ( (float)(size - 1)) , ( (float) (i) ) / ( (float) (size - 1) ) );

// 			// std::cout << v.position.x << ", " << v.position.y << std::endl;
// 			vertices.push_back(v);

// 			// if (i < sz -1 && j < sz - 1){

// 			// }
// 		}
// 	}

// 	for (int i =0; i < sz - 1; ++i){
// 		for (int j =0; j < sz - 1; ++j){
// 			indicies.push_back(funcs::flatten(i, j, sz));
// 			indicies.push_back(funcs::flatten(i, j + 1, sz));
// 			indicies.push_back(funcs::flatten(i + 1, j, sz));

// 			indicies.push_back(funcs::flatten(i, j + 1, sz));
// 			indicies.push_back(funcs::flatten(i + 1, j, sz));
// 			indicies.push_back(funcs::flatten(i + 1, j + 1, sz));
// 		}
// 	}

// 	return Mesh(vertices, textures, indicies);
// }


// std::vector<float> generateHeightData(unsigned int size, glm::vec2 center = glm::vec2(0.0f)){
// 	// Perlin2d perlin;
// 	std::vector<float> v;
// 	float tlX = (size - 1)/-2.0f;
// 	float tlY = (size - 1)/2.0f;
// 	float x, y;
// 	float scale = 0.1f;
// 	for (unsigned int i =0; i < size; ++i){
// 		for (unsigned int j =0; j < size; ++j){
// 			x = (center.x + tlX + (float)j) * scale;
// 			y = (center.y + tlY - (float)i) * scale;
// 			float p = perlin.perlin(x, y) ;
// 			// std::cout << p << std::endl;
// 			v.push_back(p);
// 		}
// 	}

// 	return v;
// }


int main() {
	// std::srand(42);
	stbi_set_flip_vertically_on_load(true);

	Window window(800, 600);


	glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window.window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initliaze GLAD" << std::endl;
		return -1;
	}

	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadvertices), quadvertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float)*4, (void*)(sizeof(float)*2));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	
	Shader shader{"shaders/p_v.glsl", "shaders/p_f.glsl"};
	Shader shader2{"shaders/q_v.glsl", "shaders/q_f.glsl"};
	Shader shader3{"shaders/w_v.glsl", "shaders/w_f.glsl"};
	Shader shader4{"shaders/e_v.glsl", "shaders/e_f.glsl"};
	Shader shader5{"shaders/normal_viz_v.glsl", "shaders/normal_viz_f.glsl", "shaders/normal_viz.glsl"};
	Shader shader6{"shaders/quadV.glsl", "shaders/quadF.glsl"};

	glEnable(GL_DEPTH_TEST);
	glm::mat4 proj(1.0f);
	float near = .1f;
	float far = 200.f;
	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, near, far);
	float waterHeight = -0.2f;

	EndlessTerrain terr(cam);
	FrameBuffer fbo;


	shader6.use();
	shader6.setVec3("camPos", cam.position);
	shader6.setFloat("near", near);
	shader6.setFloat("far", far);
	shader6.setMatrix("invProjMat", glm::inverse(proj));


	// unsigned int size = 241;
	// unsigned int chunkSize = size - 1;
	// std::vector<float> heightData = generateHeightData(size); // Use Perlin noise or another noise function
	// std::vector<float> heightData2 = generateHeightData(size, glm::vec2(0.0f, (float)chunkSize));
	// Mesh p = genPlane(size, heightData); 
	// Mesh p2 = genPlane(size, heightData2);

	// shader4.use();
	// shader4.setMatrix("proj", proj);

	// float tlX = (size - 1)/-2.0f;
	// float tlY = (size - 1)/2.0f;

	// shader4.setVec2("offset", glm::vec2(-tlX, tlY));

	// GLuint heightmapTexture;
	// glGenTextures(1, &heightmapTexture);
	// glBindTexture(GL_TEXTURE_2D, heightmapTexture);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// // Generate or load heightmap data (here we're assuming grayscale data)
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size, size, 0, GL_RED, GL_FLOAT, heightData.data());
	// glGenerateMipmap(GL_TEXTURE_2D);


	// GLuint heightmapTexture2;
	// glGenTextures(1, &heightmapTexture2);
	// glBindTexture(GL_TEXTURE_2D, heightmapTexture2);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// // Generate or load heightmap data (here we're assuming grayscale data)
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size, size, 0, GL_RED, GL_FLOAT, heightData2.data());
	// glGenerateMipmap(GL_TEXTURE_2D);

	// Plane plane;
	GLuint ntId = funcs::genWorleyNoise(50, 50, 50);
	GLuint weatherTextureId = funcs::loadWeatherData("weather_data.raw");
	GLuint detailTextureId = funcs::loadDetailTexture("low_res.raw");

	while (!window.shouldClose())
	{
		processInput(window.window);

		// glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

		glClearColor(0.86f, 0.82f, 0.78f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		fbo.Bind();
		shader4.use();
		shader4.setMatrix("proj", proj);
		shader4.setMatrix("view", cam.getView());
		terr.draw(shader4);

		#ifdef DRAW_NORMALS
		shader5.use();
		shader5.setMatrix("proj", proj);
		shader5.setMatrix("view", cam.getView());
		terr.draw(shader5);
		#endif
		fbo.unBind();

		shader6.use();
		shader6.setMatrix(
			"invViewMat",
			glm::inverse(cam.getView())
		);
		shader6.setVec3("camPos", cam.position);
		// shader6.setVec3("offSet", glm::vec3(
		// 	0.0f, 
		// 	0.0f,
		// 	(float)glfwGetTime()
		// ));
		shader6.setInt("texture_clouds", 2);
		shader6.setInt("weather_data", 3);
		shader6.setInt("detailTexture", 4);
		shader6.setFloat("densityThreshold", densityThreshold);
		shader6.setFloat("scale", scale);
		shader6.setFloat("weatherScale", weatherScale);
		shader6.setFloat("higherScale", higherScale);
		shader6.setVec3("offSet", glm::vec3(
			cam.position.x, 
			0.0,
			cam.position.z
		));
		shader6.setVec3("bounding_rect.pos", glm::vec3(cam.position.x, 45.5f, cam.position.z));
		shader6.setVec3("bounding_rect.dims", glm::vec3(300.0f, 100.0f, 300.0f));
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_3D, ntId);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, weatherTextureId);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_3D, detailTextureId);
		fbo.draw(shader6);

		// fbo.Bind();
		// fbo.unBind();

		// plane.draw(shader6, 0, 0);
		// glBindVertexArray(quadVAO);
		// shader6.use();
		// glDrawArrays(GL_TRIANGLES, 0, 6);
		// glBindVertexArray(0);


		// shader4.use();
		// shader4.setInt("heightMap", 0);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, heightmapTexture);
		// shader4.setMatrix("view", cam.getView());
		// shader4.setMatrix("model", glm::scale(glm::mat4(1.0f), glm::vec3(10.f/float(chunkSize))));
		// p.draw(shader4);

		// // glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, size, size, 0, GL_RED, GL_FLOAT, heightData2.data());
		// // glGenerateMipmap(GL_TEXTURE_2D);

		// glm::mat4 model(1.0f);
		// model = glm::translate(model, glm::vec3(0.0f, 0.0f, 10.0f));
		// model = glm::scale(model, glm::vec3(10.f/float(chunkSize)));

		// shader4.setMatrix("model", model);
		// shader4.setInt("heightMap", 0);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, heightmapTexture2);
		// p2.draw(shader4);

		window.update();
	}
	// delete terr;
	// delete plane;
	glfwTerminate();
	return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{

	if (!lastTime) {
		lastTime = glfwGetTime();
		return;
	}

	float cTime = glfwGetTime();
	dt = (cTime - lastTime) * 10;
	lastTime = cTime;
	float xOff = 0.0f, yOff = 0.0f;

	moveFac += moveVel * dt;
	moveFac -= (int)moveFac;
	
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
		// yOff += 0.001f;
		cam.move(Camera::UP, 0.50f*dt);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		// yOff -= 0.001f;
		cam.move(Camera::DOWN, 0.50f*dt);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		cam.move(Camera::RIGHT, 0.50f*dt);
		// xOff += 0.001f;
	}	

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		// xOff -= 0.001f;
		cam.move(Camera::LEFT, 0.50f*dt);
	}


	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		densityThreshold += 0.01f;
	
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		densityThreshold -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		scale += 0.01f;
	
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		scale -= 0.01f;

	// if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
	// 	yOff += 0.001f;
	// }

	// if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
	// 	yOff -= 0.001f;
	// }

	// if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
	// 	xOff += 0.001f;
	// }	

	// if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
	// 	xOff -= 0.001f;
	// }


	// terr->move(xOff, yOff);

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		cam.yaw -= cam.sensitivity*dt*100;
		cam.updateDirection();
	}

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		cam.yaw += cam.sensitivity*dt*100;
		cam.updateDirection();
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		//cam.pitch += cam.sensitivity;
		cam.incPitch(cam.sensitivity*dt*100);
		cam.updateDirection();
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		cam.incPitch(-cam.sensitivity*dt*100);
		//cam.pitch -= cam.sensitivity;
		cam.updateDirection();
	}
}

void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	// cam.handleMouse(xPos, yPos);
}
