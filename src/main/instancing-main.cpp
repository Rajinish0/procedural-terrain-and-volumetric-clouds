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
#include "sprite_renderer.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "dynamic_system.h"
#include "error_utils.h"

#include "game.h"



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xPos, double yPos);


float cloudHeight = 155.5f;
float dt = 0.007f;
float timeBetweenFrames = 0.0f;
float lT = 0.0f;
float avg = 0.0f;
float lastTime = NULL;


Camera camera;

float fps;

bool showCfg = true;


int main() {
	Window window(800, 600, "Procedural Terrain And Clouds");


	glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window.window, mouse_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initliaze GLAD" << std::endl;
		return -1;
	}

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
	Shader spriteShader{"shaders/quad_vertex.glsl", "shaders/quad_frag.glsl"};
	Texture dudv{"textures/dudv.png", Texture::DIFFUSE, GL_REPEAT, GL_REPEAT};

	SpriteRenderer spriteRenderer{};

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
	glm::vec3 lpos(1.2f, 0.0f, 1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
	const float near = REngine::NEAR_VIEW;
	const float far  = REngine::FAR_VIEW;
	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, near, far);

	
	glm::mat4 orthoProj = glm::ortho(0.0f, static_cast<float>(800.0f), 
									 static_cast<float>(600.0f), 0.0f, -1.0f, 1.0f);



	invProj = glm::inverse(proj);
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

	spriteShader.use();
	spriteShader.setMatrix("proj", orthoProj);


	fbo.setClearColor(glm::vec4(0.529,0.708,0.922, 1.0f));
	glm::mat4 cubeModel = glm::translate(
		glm::mat4(1.0f),
		glm::vec3(3.0f, 0.0f, 0.0f)
	);

	float waterHeight = 5.0f;
	Plane plane;
	std::shared_ptr<AudioManager> audioMgr = std::make_shared<AudioManager>();
	AirplaneParameters airplaneParams = {
		"models/plane/a22.obj",
		"models/air_drop/air_drop.obj",
		"audio/aircraft.wav",
		"audio/parachute.wav",
		audioMgr
	};
	Airplane myAirPlane {airplaneParams};


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
		"cloud_data/f_data_HIGH.raw",
		"cloud_data/low_res.raw",
		"cloud_data/weather_data_2.raw",
		camera,
		computeShdr,
		glm::vec3(400.0f, 200.0f, 400.0f),
		near, far, cloudHeight, proj, 
		sunD, fbo
	);

	std::shared_ptr<game::GameParameters> gameParams =
	std::make_shared<game::GameParameters>(
			textRenderer,
			myAirPlane,
			camera,
			terrainSystem,
			CloudSystem,
			spriteRenderer
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
		float t = glfwGetTime();
		timeBetweenFrames = t - lT;
		lT = t;
		fps = 1.0f/(timeBetweenFrames);
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		glEnable(GL_BLEND);
		glEnable(GL_CLIP_DISTANCE0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		processInput(window.window);
		glClearColor(0.1f, 0.1f, 0.1f, 0.1f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		avg = timeBetweenFrames * 0.1 + avg * 0.9;
		myGame.update(avg);
		myGame.draw(textShader, screenShdr, spriteShader);

		audioMgr->update();
		

		if (showCfg){
			ImGui::Begin("Config", &showCfg);
			ImGui::Text(("FPS: " + std::to_string(fps)).c_str());
			myAirPlane.addConfigParamsToImgui();
			CloudSystem.addConfigParamsToImgui();
			terrainSystem.addConfigParamsToImgui();
			myGame.addConfigParamsToImgui();
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

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS && 
		glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		showCfg = !showCfg;

}


void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
	// camera.handleMouse(xPos, yPos);
}

