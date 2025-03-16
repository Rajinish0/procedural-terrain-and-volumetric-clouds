// #define DRAW_NORMALS 1
// #define LINE_MODE 1
// #define DRAW_WATER 1


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
#include "audio_manager.h"

#include "compute_shader.h"
#include "engine_consts.h"
#include "textrender.h"
#include "airplane.h"
#include <ft2build.h> // checking if build was good
#include "cloud_system.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "dynamic_system.h"
#include "error_utils.h"

#include "game.h"



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

inline std::ostream& operator<<(std::ostream& os, const glm::vec2& v){
	os << '(' << v.x << ', ' << v.y << ')';
	return os;
}

std::ostream& operator<<(std::ostream& os, const glm::vec3& v){
	os << '(' << v.x << ', ' << v.y << ', ' << v.z << ')';
	return os;
}


Camera camera;

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
	// EndlessTerrain terr(camera);

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
	Shader textShader{"shaders/textVshader.glsl", "shaders/textFshader.glsl"};
	Texture dudv{"textures/dudv.png", Texture::DIFFUSE, GL_REPEAT, GL_REPEAT};

	glm::vec3 sunD = glm::normalize(glm::vec3(
		0.0f,
		1.0f,
		-2.0f
	));

	glEnable(GL_DEPTH_TEST);

	FrameBuffer fbo, fb, fb2;
	TextRenderer textRenderer("fonts/ocraext.ttf");


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

	
    glm::mat4 orthoProj = glm::ortho(0.0f, static_cast<float>(800.0f), 
									 static_cast<float>(600.0f), 0.0f, -1.0f, 1.0f);
	glm::mat4 planeModel = glm::rotate(
		glm::mat4(1.0f), 
		glm::radians(90.0f),
		glm::vec3(1.0f, 0.0f, 0.0f)
	);



	invProj = glm::inverse(proj);
	invView = glm::inverse(view);
	camera.position = glm::vec3(0.0f);

	shader4.use();
	shader4.setMatrix("proj", proj);

	airPlaneShader.use();
	airPlaneShader.setMatrix("proj", proj);

	shader5.use();
	shader5.setMatrix("proj", proj);

	computeShdr.use();
	computeShdr.setMatrix("invProjMat", invProj);

	waterShader.use();
	waterShader.setMatrix("proj", proj);

	textShader.use();
	textShader.setMatrix("proj", orthoProj);
	glm::vec3 lightPos(3.0f, 2.0f, 0.0f);

	fbo.setClearColor(glm::vec4(0.529,0.708,0.922, 1.0f));
	glm::mat4 cubeModel = glm::translate(
		glm::mat4(1.0f),
		glm::vec3(3.0f, 0.0f, 0.0f)
	);

	float waterHeight = 5.0f;
	Plane plane;
	std::shared_ptr<AudioManager> audioMgr = std::make_shared<AudioManager>();
	Airplane myAirPlane {"models/plane/a22.obj", 
						 "models/air_drop/air_drop.obj",
						 "audio/aircraft.wav",
						 "audio/parachute.wav",
						 audioMgr
						};


	myAirPlane.setPos(glm::vec3(0.0f, 40.0f, 0.0f));
	myAirPlane.mount(&camera);
	myAirPlane.attach(&window);


	DynamicSystem terrainSystem(
		camera, shader4, shader5, 
		airPlaneShader, waterShader, 
		myAirPlane, 
		REngine::MAX_TERRAIN_HEIGHT, waterHeight,
		 sunD);
	CloudSystem CloudSystem(
		"f_data_HIGH.raw",
		"low_res.raw",
		"weather_data_2.raw",
		camera,
		computeShdr,
		glm::vec3(400.0f, 200.0f, 400.0f),
		near, far, cloudHeight, proj, 
		sunD, fbo
	);

	std::shared_ptr<game::GameParameters> gameParams =
	std::make_shared<game::GameParameters>(
			textRenderer,
			myAirPlane
	);

	game::Game myGame(gameParams);

	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window.window, "#canvas");
	ImGui_ImplOpenGL3_Init("#version 150");

	ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	glm::vec3 textColor = {0.23921f, 0.415686f, 0.44705f};
	glm::vec2 randCoords = funcs::genRandomCoords2d();
	while (!window.shouldClose())
	{

		fps = 1.0f/(glfwGetTime()- lastTime);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		terrainSystem.update( 1.0f/60.0f );


		glEnable(GL_BLEND);
		glEnable(GL_CLIP_DISTANCE0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		processInput(window.window);
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		view = camera.getView();


		// fbo.Bind();

		#if LINE_MODE
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		#endif


		#ifdef DRAW_WATER
		// shader4.setMatrix("view", camera.getView());
		float offSet = 2*(camera.position.y - waterHeight);
		camera.position = glm::vec3(camera.position.x, 
								 camera.position.y - offSet,
								 camera.position.z);//camera.position.y -= offSet;
		camera.pitch = -camera.pitch;
		camera.updateDirection();
		shader4.setMatrix("view", camera.getView());
		shader4.setVec4("planeNorm", glm::vec4(0.0f, 1.0f, 0.0f, -waterHeight));
		fb.Bind();
		terrainSystem.terrain.draw(shader4);
		fb.unBind();

		camera.position = glm::vec3(camera.position.x, 
								 camera.position.y + offSet,
								 camera.position.z);
		camera.pitch = -camera.pitch;
		camera.updateDirection();
		shader4.setMatrix("view", camera.getView());
		shader4.setVec4("planeNorm", glm::vec4(0.0f,-1.0f, 0.0f, waterHeight));
		fb2.Bind();
		terrainSystem.terrain.draw(shader4);
		fb2.unBind();

		#endif

		// fbo.Bind();
		


		#if LINE_MODE
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		#endif

		// glEnable(GL_BLEND);
		// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// glEnable(GL_DEPTH_TEST);
		// glDisable(GL_CLIP_DISTANCE0);
		#ifdef DRAW_WATER
		waterShader.use();
		waterShader.setMatrix("proj", proj);
		waterShader.setMatrix("view", camera.getView());
		glm::mat4 m =glm::translate(glm::mat4(1.0f), glm::vec3(camera.position.x, waterHeight, camera.position.z));
		glm::mat4 m2 = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 m3 =glm::scale(glm::mat4(1.0f), glm::vec3((50.0f*20.0f)/2.0f));
		waterShader.setMatrix("model", m*m2*m3);

		waterShader.use();
		waterShader.setInt("t1", 0);
		waterShader.setInt("t2", 1);
		waterShader.setInt("dudv", 2);
		waterShader.setFloat("moveFac", moveFac);
		waterShader.setVec3("cameraPos", camera.position);
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
		#endif


		terrainSystem.draw(fbo);

		// fbo.unBind();

		CloudSystem.update(fbo);
		CloudSystem.draw(screenShdr);
		
		myGame.update(dt);
		myGame.draw(textShader);

		audioMgr->update();
		

		if (showCfg){
			ImGui::Begin("Config", &showCfg);
			ImGui::Text(("FPS: " + std::to_string(fps)).c_str());
			myAirPlane.addConfigParamsToImgui();
			CloudSystem.addConfigParamsToImgui();
			myGame.addConfigParamsToImgui();
			// ImGui::ColorEdit3("Text Color", &textColor.r);
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

	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		densityThreshold += 0.001f;
	
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		densityThreshold -= 0.001f;


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

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && 
		glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		showCfg = !showCfg;

}


void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	// camera.handleMouse(xPos, yPos);
}

