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
#include <ft2build.h> // checking if build was good


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

Camera cam;
std::vector<glm::vec3> vegetation;
Perlin2d perlin(256, 8);
Plane *plane;
Terrain *terr;

float terrainSize = 10.0f;

unsigned int FBO, RBO, textureId;


int main() {
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

	FrameBuffer fb;
	FrameBuffer fb2;

	// glGenFramebuffers(1, &FBO);
	// glGenRenderbuffers(1, &RBO);


	// glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// glGenTextures(1, &textureId);
	// glBindTexture(GL_TEXTURE_2D, textureId);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// glBindTexture(GL_TEXTURE_2D, 0);
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

	// glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	// glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 800, 600);
	// glBindRenderbuffer(GL_RENDERBUFFER, 0);

	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);
	// if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	// 	std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;


	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	Shader shader{"shaders/p_v.glsl", "shaders/p_f.glsl"};
	Shader shader2{"shaders/q_v.glsl", "shaders/q_f.glsl"};
	Shader shader3{"shaders/w_v.glsl", "shaders/w_f.glsl"};
	terr = new Terrain{100, terrainSize, shader};
	Texture dudv{"textures/dudv.png", Texture::DIFFUSE, GL_REPEAT, GL_REPEAT};
	plane = new Plane;

	glEnable(GL_DEPTH_TEST);
	glm::mat4 proj(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);
	float waterHeight = -0.2f;

	while (!window.shouldClose())
	{
		processInput(window.window);

		glEnable(GL_CLIP_DISTANCE0);

		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f));
		model = glm::rotate(
			model, glm::radians(-90.0f),
			glm::vec3(1.0f, 0.0f, 0.0f));
		shader.use();

		shader.setMatrix("proj", proj);
		shader.setMatrix("model", model);
		//reflection
		float offSet = 2*(cam.position.y - waterHeight);
		cam.position = glm::vec3(cam.position.x, 
								 cam.position.y - offSet,
								 cam.position.z);//cam.position.y -= offSet;
		cam.pitch = -cam.pitch;
		cam.updateDirection();
		shader.setMatrix("view", cam.getView());
		shader.setVec4("planeNorm", glm::vec4(0.0f, 1.0f, 0.0f, -waterHeight));
		// glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		fb.Bind();
		terr->draw();
		fb.unBind();

		cam.position = glm::vec3(cam.position.x, 
								 cam.position.y + offSet,
								 cam.position.z);
		cam.pitch = -cam.pitch;
		cam.updateDirection();
		shader.setMatrix("view", cam.getView());

		//refraction
		shader.setVec4("planeNorm", glm::vec4(0.0f,-1.0f, 0.0f, -waterHeight));
		fb2.Bind();
		terr->draw();
		fb2.unBind();

		
		// glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(0.86f, 0.82f, 0.78f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CLIP_DISTANCE0);
		shader3.use();
		shader3.setMatrix("proj", proj);
		shader3.setMatrix("view", cam.getView());
		glm::mat4 m =glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, waterHeight, 0.0f));
		glm::mat4 m2 =glm::scale(glm::mat4(1.0f), glm::vec3((terrainSize)/2.0f));
		shader3.setMatrix("model", m*model*m2);

		shader3.use();
		shader3.setInt("t1", 0);
		shader3.setInt("t2", 1);
		shader3.setInt("dudv", 2);
		shader3.setFloat("moveFac", moveFac);
		shader3.setVec3("camPos", cam.position);
		shader3.setVec3("planeNorm", glm::vec3(0.0f, 1.0f, 0.0f));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fb.textureId);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fb2.textureId);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, dudv.id);

		glBindVertexArray(plane->VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		terr->draw();
		// plane->draw(shader3, fb.textureId);

		// shader2.use();
		// shader2.setFloat("xOff",-0.7f);
		// shader2.setFloat("yOff", 0.4f);
		// fb.draw(shader2);

		// shader2.setFloat("xOff", 0.7f);
		// // shader2.setFloat("yOff", 0.4f);
		// fb2.draw(shader2);

		// shader2.use();
		// glBindVertexArray(quadVAO);
		// glDisable(GL_DEPTH_TEST);
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, textureId);
		// glDrawArrays(GL_TRIANGLES, 0, 6);
		// glBindTexture(GL_TEXTURE_2D, 0);
		// glBindVertexArray(0);

		window.update();

	}
	delete terr;
	delete plane;
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
		cam.move(Camera::UP, 0.10f*dt);
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
		// yOff -= 0.001f;
		cam.move(Camera::DOWN, 0.10f*dt);
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
		cam.move(Camera::RIGHT, 0.10f*dt);
		// xOff += 0.001f;
	}	

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
		// xOff -= 0.001f;
		cam.move(Camera::LEFT, 0.10f*dt);
	}


	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS){
		yOff += 0.001f;
	}

	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS){
		yOff -= 0.001f;
	}

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS){
		xOff += 0.001f;
	}	

	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS){
		xOff -= 0.001f;
	}


	terr->move(xOff, yOff);

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
	cam.handleMouse(xPos, yPos);
}
