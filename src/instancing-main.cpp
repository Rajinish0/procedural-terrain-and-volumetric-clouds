#include <iostream>
#include "shader.h"
#include "camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif
#include "stb_image.h"
#include "funcs.h"
#include "window.h"
#include "model.h"
#include "perlin.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "framebuffer.h"
#include "endless_terrain.h"
#include "compute_shader.h"
#include "engine_consts.h"
#include "textrender.h"
#include "airplane.h"
#include <ft2build.h> // checking if build was good

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


// #define DRAW_NORMALS 1
// #define LINE_MODE 1


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);


float densityThreshold = REngine::DENSITY_THRESHOLD;
float scale 		   = REngine::SCALE;
float weatherScale     = REngine::WEATHER_SCALE;
float higherScale 	   = REngine::HIGHER_SCALE;
float SIGMA 		   = REngine::SIGMA;
float HG			   = REngine::HG;
float moveFac = 0.0f;
float moveVel = 0.015f;
float cloudHeight = 155.5f;
unsigned int texture2, skboxTexture;
float dt = 0.007f;
float lastTime = NULL;




template <typename T>
T randNum(T min, T max){
	T dist = (max - min);
	float random_number = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
	return (min + dist*random_number);
}


Camera cam;

glm::vec3 lightPos = glm::vec3(-1.0f, 7.0f, 2.0f);

float planeRoll = 0.0f;
float fps;

bool showCfg = true;


int main() {
	// stbi_set_flip_vertically_on_load(true);

	Window window(800, 600);


	glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window.window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initliaze GLAD" << std::endl;
		return -1;
	}
	EndlessTerrain terr(cam);

	 ComputeShader computeShdr {
		"shaders/compute.glsl"
	 };

	 Shader screenShdr {
		"shaders/screenV.glsl",
		"shaders/screenF.glsl"
	 };

	Shader shader4{"shaders/e_v.glsl", "shaders/e_f.glsl"};
	Shader shader5{"shaders/normal_viz_v.glsl", "shaders/normal_viz_f.glsl", "shaders/normal_viz.glsl"};
	Shader waterShader{"shaders/w_v.glsl", "shaders/w_f.glsl"};
	Shader airPlaneShader{"shaders/model-vshader.glsl", "shaders/model-fshader.glsl"};
	Texture dudv{"textures/dudv.png", Texture::DIFFUSE, GL_REPEAT, GL_REPEAT};

	glEnable(GL_DEPTH_TEST);

	FrameBuffer fbo, fb, fb2;
	TextRenderer textRenderer;

	unsigned int compT;
	glGenTextures(1, &compT);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, compT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 800, 600, 0, GL_RGBA, 
				GL_FLOAT, NULL);

	glBindImageTexture(0, compT, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);


	glViewport(0, 0, 800, 600);
	glm::mat4 trans(1.0f);
	glm::mat4 model(1.0f);
	glm::mat4 lmodel(1.0f);
	glm::mat4 view(1.0f);
	glm::mat4 proj(1.0f);
	glm::mat4 invProj(1.0f);
	glm::mat4 invView(1.0f);
	glm::vec3 lpos(1.2f, 0.0f, 1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	const float near = 0.1f;
	const float far  = 500.0f;
	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, near, far);
	glm::mat4 planeModel = glm::rotate(
		glm::mat4(1.0f), 
		glm::radians(90.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);

	float sphereRad = 0.2f;
	glm::mat4 sphereModel = glm::mat4(1.0f);
	sphereModel = glm::translate(sphereModel, glm::vec3(-2 * sphereRad, sphereRad, 0.0f));
	sphereModel = glm::scale(sphereModel, glm::vec3(sphereRad));

	glm::mat4 sphereModel2 = glm::mat4(1.0f);
	sphereModel2 = glm::translate(sphereModel2, glm::vec3(2 * sphereRad, sphereRad, 0.0f));
	sphereModel2 = glm::scale(sphereModel2, glm::vec3(sphereRad));



	invProj = glm::inverse(proj);
	invView = glm::inverse(view);
	cam.position = glm::vec3(0.0f);
	Mesh sphere = funcs::genSphere();

	computeShdr.use();
	computeShdr.setVec3("camPos", cam.position);
	computeShdr.setVec3("bounding_rect.pos", glm::vec3(0.0f, 130.5f, 0.0f));
	computeShdr.setVec3("bounding_rect.dims", glm::vec3(400.0f, 200.0f, 400.0f));
	computeShdr.setFloat("near", near);
	computeShdr.setFloat("far", far);
	computeShdr.setMatrix("invProjMat", glm::inverse(proj));
	glm::vec3 lightPos(3.0f, 2.0f, 0.0f);

	// fbo.setClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 0.1f));
	fbo.setClearColor(glm::vec4(0.529,0.708,0.922, 1.0f));
	GLuint ntId = funcs::genWorleyNoise(50, 50, 50);
	// GLuint weatherTextureId = funcs::loadWeatherData("weather_data.raw");
	GLuint weatherTextureId = funcs::loadWeatherData("weather_data_2.raw");
	GLuint detailTextureId = funcs::loadDetailTexture("low_res.raw");
	GLuint highTexture = funcs::loadGeneric3dTexture("f_data_HIGH.raw");
	std::cout << "LOADING WEATHER TEXTURE" << std::endl;
	GLuint weatherDataTexure = funcs::loadGeneric2dTexture("weather_data_f.raw");
	std::cout << "DONE loading textures" << std::endl;

	glm::mat4 cubeModel = glm::translate(
		glm::mat4(1.0f),
		glm::vec3(3.0f, 0.0f, 0.0f)
	);

	float waterHeight = 5.0f;
	Plane plane;
	Model airPlane {"models/plane/a22.obj"};
	Airplane myAirPlane {"models/plane/a22.obj"};

	myAirPlane.setPos(glm::vec3(0.0f, 40.0f, 0.0f));
	myAirPlane.mount(&cam);
	myAirPlane.attach(&window);


	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.window, "#canvas");
	ImGui_ImplOpenGL3_Init("#version 150");

	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	while (!window.shouldClose())
	{

		fps = 1.0f/(glfwGetTime()- lastTime);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		myAirPlane.update(1.0f/60.0f);

		// cam.up = 
		// glm::normalize(
		// glm::vec3(
		// 	glm::rotate(
		// 	glm::mat4(1.0f),
		// 	glm::radians(planeRoll),
		// 	glm::vec3(0.0f, 0.0f, 1.0f)
		// ) * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)
		// ));
		// cam.updateDirection();
		glEnable(GL_BLEND);
		glEnable(GL_CLIP_DISTANCE0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		processInput(window.window);
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		view = cam.getView();

		// std::cout << "sc: "<< scale << std::endl;
		// std::cout << "dt: "<< densityThreshold << std::endl;
		// std::cout << "wc: "<< weatherScale << std::endl;
		// std::cout << "hc: "<< higherScale << std::endl;

		lightPos = glm::rotate(
			glm::mat4(1.0f), 
			glm::radians(0.1f), 
			glm::vec3(0.0f, 1.0f, 0.0f)) * glm::vec4(lightPos, 0.0f);

		glm::vec3 sunD = glm::normalize(glm::vec3(
			//std::cos(glfwGetTime()), 
			0.0f,
			1.0f,
			// std::sin(glfwGetTime())
			-2.0f
		));

		// fbo.Bind();

		#if LINE_MODE
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		#endif

		shader4.use();
		shader4.setMatrix("proj", proj);
		shader4.setFloat("maxHeight", REngine::MAX_TERRAIN_HEIGHT);
		shader4.setVec3("sunDirection", sunD);
		// shader4.setMatrix("view", cam.getView());
		float offSet = 2*(cam.position.y - waterHeight);
		cam.position = glm::vec3(cam.position.x, 
								 cam.position.y - offSet,
								 cam.position.z);//cam.position.y -= offSet;
		cam.pitch = -cam.pitch;
		cam.updateDirection();
		shader4.setMatrix("view", cam.getView());
		shader4.setVec4("planeNorm", glm::vec4(0.0f, 1.0f, 0.0f, -waterHeight));

		fb.Bind();
		terr.draw(shader4);
		fb.unBind();

		cam.position = glm::vec3(cam.position.x, 
								 cam.position.y + offSet,
								 cam.position.z);
		cam.pitch = -cam.pitch;
		cam.updateDirection();
		shader4.setMatrix("view", cam.getView());
		shader4.setVec4("planeNorm", glm::vec4(0.0f,-1.0f, 0.0f, waterHeight));
		fb2.Bind();
		terr.draw(shader4);
		fb2.unBind();

		fbo.Bind();
		#if DRAW_NORMALS
			shader5.use();
			shader5.setMatrix("proj", proj);
			shader5.setFloat("maxHeight", REngine::MAX_TERRAIN_HEIGHT);
			shader5.setMatrix("view", cam.getView());
			terr.draw(shader5);
		#endif


		#if LINE_MODE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		#endif

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_CLIP_DISTANCE0);
		waterShader.use();
		waterShader.setMatrix("proj", proj);
		waterShader.setMatrix("view", cam.getView());
		glm::mat4 m =glm::translate(glm::mat4(1.0f), glm::vec3(cam.position.x, waterHeight, cam.position.z));
		glm::mat4 m2 = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 m3 =glm::scale(glm::mat4(1.0f), glm::vec3((50.0f*20.0f)/2.0f));
		waterShader.setMatrix("model", m*m2*m3);

		waterShader.use();
		waterShader.setInt("t1", 0);
		waterShader.setInt("t2", 1);
		waterShader.setInt("dudv", 2);
		waterShader.setFloat("moveFac", moveFac);
		waterShader.setVec3("camPos", cam.position);
		waterShader.setVec3("planeNorm", glm::vec3(0.0f, 1.0f, 0.0f));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, fb.textureId);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, fb2.textureId);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, dudv.id);

		glBindVertexArray(plane.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);

		shader4.use();
		shader4.setMatrix("proj", proj);
		shader4.setMatrix("view", cam.getView());
		terr.draw(shader4);
		// myAirPlane.updateCameraFeatures();
		airPlaneShader.use();
		airPlaneShader.setMatrix("proj", proj);
		airPlaneShader.setVec3("sunDirection", sunD);
		// planeModel = glm::mat4(1.0f);
		// planeModel = glm::scale(planeModel, glm::vec3(5.0f));
		// planeModel = glm::rotate(planeModel, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		// planeModel = glm::rotate(planeModel, glm::radians(planeRoll), glm::vec3(1.0f, 0.0f, 0.0f));
		// // airPlaneShader.setMatrix("model", planeModel);
		// airPlaneShader.setMatrix("model", glm::translate(
		// 									glm::rotate(glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0) ), 
		// 									glm::vec3(0.0f, 25.0f, 0.0f)
		// 					) ) ;
		// airPlaneShader.setMatrix("view", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0, -80.0f)) ) ;
		airPlaneShader.setMatrix("view", cam.getView());

		// airPlane.draw(airPlaneShader);
		myAirPlane.draw(airPlaneShader);

		fbo.unBind();
		computeShdr.use();
		computeShdr.setMatrix(
			"invViewMat",
			glm::inverse(view)
		);
		computeShdr.setVec3("camPos", cam.position);
		computeShdr.setVec3("bounding_rect.pos", glm::vec3(cam.position.x, cloudHeight, cam.position.z));
		computeShdr.setVec3("offSet", glm::vec3(
			cam.position.x, 
			0.0f,
			cam.position.z + (float)glfwGetTime()*REngine::WIND_SPEED
		));


		// std::cout << "DENSITY THRESHOLD: " << densityThreshold << std::endl;
		// std::cout << "SIGMA: " 			   << SIGMA << std::endl;
		// std::cout << "HG: " 			   << HG << std::endl;
		
		glDisable(GL_BLEND);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, compT);
        computeShdr.setInt("texture_diffuse1", 1);
        computeShdr.setInt("depthTexture", 2);
		computeShdr.setInt("texture_clouds", 3);
		computeShdr.setInt("weather_data", 4);
		computeShdr.setInt("detailTexture", 5);
		computeShdr.setFloat("densityThreshold", densityThreshold);
		computeShdr.setFloat("scale", scale);
		computeShdr.setFloat("weatherScale", weatherScale);
		computeShdr.setFloat("higherScale", higherScale);
		computeShdr.setFloat("SIGMA", SIGMA);
		computeShdr.setFloat("HG", HG);
		computeShdr.setVec3("sunDirection", sunD);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, fbo.textureId);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, fbo.depthTextureId);
		glActiveTexture(GL_TEXTURE3);
		// glBindTexture(GL_TEXTURE_3D, ntId);
		glBindTexture(GL_TEXTURE_3D, highTexture);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, weatherTextureId);
		// glBindTexture(GL_TEXTURE_2D, weatherDataTexure);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_3D, detailTextureId);
		glDispatchCompute((unsigned int)std::ceil(800.0/16.0), 
						  (unsigned int)std::ceil(600.0/16.0),
						  1);
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		// fbo.draw(quadShdr);

		screenShdr.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, compT);
		glBindVertexArray(plane.VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// plane.draw(quadShdr, 0);

		if (showCfg){
			ImGui::Begin("Config", &showCfg);
			ImGui::Text(("FPS: " + std::to_string(fps)).c_str());
			myAirPlane.addConfigParamsToImgui();
			ImGui::SliderFloat("Density Threshold", &densityThreshold, 0.0f, 1.0f);
			ImGui::SliderFloat("Scale", &scale, 0.0f, 2.0f);
			ImGui::SliderFloat("HG", &HG, 0.0f, 1.0f);
			ImGui::SliderFloat("SIGMA", &SIGMA, 0.0f, 2.0f);
			ImGui::SliderFloat("Higher Scale", &higherScale, 0.0f, 20.0f);
			ImGui::SliderFloat("Weather Scale", &weatherScale, 0.0f, 0.005f);
			ImGui::SliderFloat("Cloud Height", &cloudHeight, 20.0f, 200.0f);
			ImGui::End();

		}
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		window.update();


	}

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
	dt = (cTime - lastTime) * 5;
	lastTime = cTime;

	moveFac += moveVel * dt;
	moveFac -= (int)moveFac;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	// if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	// 	cam.move(Camera::UP, dt*10.0f);

	// if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	// 	cam.move(Camera::DOWN, dt*10.0f);

	// if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	// 	cam.move(Camera::RIGHT, dt*10.0f);

	// if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	// 	cam.move(Camera::LEFT, dt*10.0f);

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		densityThreshold += 0.001f;
	
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		densityThreshold -= 0.001f;

	// if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
	// 	// scale += 0.001f;;
	// 	planeRoll -= 0.1f;
	
	// if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
	// 	// scale -= 0.001f;
	// 	planeRoll += 0.1f;


	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		// higherScale += .01f;
		HG += 0.001;
	
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		// higherScale -= .01f;
		HG -= 0.001;

	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
		weatherScale += 0.0001f;

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		weatherScale -= 0.0001f;

	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		SIGMA += 0.001f;

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		SIGMA -= 0.001f;

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		showCfg = !showCfg;

	// if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
	// 	cam.incYaw(-cam.sensitivity*3.0f);
	// 	// cam.yaw -= cam.sensitivity*3.0f;
	// 	// cam.updateDirection();
	// }

	// if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
	// 	cam.incYaw(cam.sensitivity*3.0f);
	// 	// cam.yaw += cam.sensitivity*3.0f;
	// 	// cam.updateDirection();
	// }

	// if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
	// 	// cam.pitch += cam.sensitivity*3.0f;
	// 	cam.incPitch(cam.sensitivity*3.0f);
	// 	// cam.updateDirection();
	// }


	// if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
	// 	cam.incPitch(-cam.sensitivity*3.0f);
	// 	//cam.pitch -= cam.sensitivity*3.0f;
	// 	// cam.updateDirection();
	// }
}


void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	// cam.handleMouse(xPos, yPos);
}

