// #include <iostream>
// #include "shader.h"
// #include "camera.h"
// #include <glad/glad.h>
// #include <GLFW/glfw3.h>
// //#ifndef STB_IMAGE_IMPLEMENTATION
// //#define STB_IMAGE_IMPLEMENTATION
// //#endif
// #include "stb_image.h"
// #include "funcs.h"
// #include "window.h"
// #include "model.h"
// #include <glm/glm.hpp>
// #include <glm/gtc/matrix_transform.hpp>
// #include <glm/gtc/type_ptr.hpp>


// void framebuffer_size_callback(GLFWwindow* window, int width, int height);
// //void loadTexture(const std::string &path, unsigned int *tId);
// void processInput(GLFWwindow *window);
// void mouse_callback(GLFWwindow* window, double xPos, double yPos);


// //
// //float vertices[] = {
// //	 0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
// //	 0.5f,-0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
// //	-0.5f,-0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
// //	-0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f
// //};

// float vertices[] = {
// 	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
// 	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
// 	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
// 	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
// 	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
// 	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

// 	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
// 	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
// 	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
// 	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
// 	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
// 	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

// 	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,-1.0f, 0.0f, 0.0f,
// 	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,-1.0f, 0.0f, 0.0f,
// 	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,-1.0f, 0.0f, 0.0f,
// 	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,-1.0f, 0.0f, 0.0f,
// 	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,-1.0f, 0.0f, 0.0f,
// 	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,-1.0f, 0.0f, 0.0f,

// 	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
// 	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
// 	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
// 	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
// 	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
// 	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

// 	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,-1.0f, 0.0f,
// 	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,-1.0f, 0.0f,
// 	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,-1.0f, 0.0f,
// 	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,-1.0f, 0.0f,
// 	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,-1.0f, 0.0f,
// 	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,-1.0f, 0.0f,

// 	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,1.0f, 0.0f,
// 	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,1.0f, 0.0f,
// 	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,
// 	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,1.0f, 0.0f,
// 	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,1.0f, 0.0f,
// 	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,1.0f, 0.0f,
// };

// unsigned int indicies[] = {
// 	0, 1, 2,
// 	0, 2, 3
// };

// glm::vec3 cubePositions[] = {
// 	glm::vec3(0.0f,  0.0f,  0.0f),
// 	glm::vec3(2.0f,  5.0f, -15.0f),
// 	glm::vec3(-1.5f, -2.2f, -2.5f),
// 	glm::vec3(-3.8f, -2.0f, -12.3f),
// 	glm::vec3(2.4f, -0.4f, -3.5f),
// 	glm::vec3(-1.7f,  3.0f, -7.5f),
// 	glm::vec3(1.3f, -2.0f, -2.5f),
// 	glm::vec3(1.5f,  2.0f, -2.5f),
// 	glm::vec3(1.5f,  0.2f, -1.5f),
// 	glm::vec3(-1.3f,  1.0f, -1.5f)
// };



// unsigned int VBO, VAO, lVAO, EBO;
// unsigned int vertexShader;
// unsigned int fragShader;
// unsigned int texture, texture2;
// float dt = 0.07;

// int width, height, nChannels;
// float theta = 0.0f;
// float dtheta = 0.001f;

// float ang = 0.0f;
// float dang = 0.001f;

// Camera cam;
// //Assimp::Importer importer;
// int main() {
// 	stbi_set_flip_vertically_on_load(true);
// 	Window window(800, 600);

// 	//glfwInit();
// 	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
// 	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
// 	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


// 	//GLFWwindow* window = glfwCreateWindow(800, 600, "Learn opengl", NULL, NULL);
// 	//if (window == NULL)
// 	//{
// 	//	std::cout << "Failed to create window" << std::endl;
// 	//	glfwTerminate();
// 	//	return -1;
// 	//}


// 	//glfwMakeContextCurrent(window);
// 	glfwSetFramebufferSizeCallback(window.window, framebuffer_size_callback);
// 	glfwSetCursorPosCallback(window.window, mouse_callback);

// 	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
// 	{
// 		std::cout << "Failed to initliaze GLAD" << std::endl;
// 		return -1;
// 	}

// 	glm::vec4 v(1.0f, 1.0f, 1.0f, 0.0f);

// 	Shader myshdr{ "shaders/shader.glsl", "shaders/frag2.glsl" };
// 	Shader lightshdr{"shaders/light_vertex.glsl", "shaders/light_frag.glsl"};
// 	Shader myModelShdr{ "shaders/mvshader.glsl", "shaders/mfshader.glsl" };
// 	/*Model myModel("D:/backpack/backpack.obj");*/
// 	Model myModel("D:/sponza/Sponza/sponza.obj");
// 	Mesh m = funcs::genSphere();


// 	glEnable(GL_DEPTH_TEST);


// 	glGenBuffers(1, &VBO);
// 	glGenBuffers(1, &EBO);

// 	//loadTexture("D:/container2.png", &texture);
// 	//loadTexture("D:/container2_specular.png", &texture2);

// 	texture = funcs::TextureFromFile("container2.png", "D:");
// 	texture2 = funcs::TextureFromFile("container2_specular.png", "D:");


// 	glGenVertexArrays(1, &VAO);
// 	glGenVertexArrays(1, &lVAO);
// 	glBindVertexArray(VAO);


// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	

// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);

// 	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
// 	glEnableVertexAttribArray(1);

// 	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
// 	glEnableVertexAttribArray(2);

// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
// 	glBindVertexArray(0);


// 	glBindVertexArray(lVAO);

// 	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);

// 	glBindBuffer(GL_ARRAY_BUFFER, 0);
// 	glBindVertexArray(0);




// 	glViewport(0, 0, 800, 600);

// 	glm::mat4 trans(1.0f);
// 	glm::mat4 model(1.0f);
// 	glm::mat4 lmodel(1.0f);
// 	glm::mat4 view(1.0f);
// 	glm::mat4 proj(1.0f);
// 	glm::vec3 lpos(1.2f, 0.0f, 1.0f);
// 	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
// 	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
// 	proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 200.0f);
// 	lmodel = glm::translate(lmodel, lpos);
// 	lmodel = glm::scale(lmodel, glm::vec3(.7f));
// 	//trans = glm::scale(trans, glm::vec3(0.5f));
// 	//theta += dtheta;

// 	myshdr.use();
// 	myshdr.setInt("material.diffuse", 0);
// 	myshdr.setInt("material.specular", 1);
// 	myshdr.setMatrix("model", model);
// 	myshdr.setMatrix("view",  view);
// 	myshdr.setMatrix("proj", proj);
// 	//myshdr.setVec3("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
// 	//myshdr.setVec3("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
// 	//myshdr.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
// 	myshdr.setFloat("material.shininess", 16.0f);
// 	myshdr.setVec3("light.ambient", glm::vec3(0.2f));
// 	myshdr.setVec3("light.diffuse", glm::vec3(0.0f));
// 	myshdr.setVec3("light.specular", glm::vec3(1.0f));
// 	myshdr.setFloat("light.constant", 1.0f);
// 	myshdr.setFloat("light.linear", 0.0f);
// 	myshdr.setFloat("light.quadratic", 0.0f);
// 	lightshdr.use();
// 	lightshdr.setMatrix("model", lmodel);
// 	lightshdr.setMatrix("view", view);
// 	lightshdr.setMatrix("proj", proj);
	

// 	myshdr.use();
	
	

// 	glActiveTexture(GL_TEXTURE0);
// 	glBindTexture(GL_TEXTURE_2D, texture);

// 	glActiveTexture(GL_TEXTURE1);
// 	glBindTexture(GL_TEXTURE_2D, texture2);

// 	//while (!glfwWindowShouldClose(window))
// 	while (!window.shouldClose())
// 	{
// 		glm::mat4 lmodel2(1.0f);
// 		lmodel2 = glm::rotate(lmodel2, ang, (glm::vec3(0.0f, 1.0, 0.0f)));
// 		lmodel2 = lmodel2 * lmodel;
// 		myshdr.use();
// 		trans = glm::rotate(trans, dtheta, glm::vec3(0.0f, 0.0f, 1.0f));
// 		myshdr.setMatrix("transform", trans);
// 		//myshdr.setVec3("lightcol", glm::vec3(1.0f));
// 		myshdr.setVec3("lightpos", glm::vec3(lmodel2 * glm::vec4(lpos, 0.0f)) );
// 		//myshdr.setVec3("lightpos", lpos);
// 		myshdr.setVec3("camPos", cam.position);
// 		view = cam.getView();

// 		processInput(window.window);
// 		//cam.handleMovement(window.window, dt);
// 		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
// 		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// 		myshdr.setMatrix("view", view);
// 		for (int i = 0; i < 10; ++i) {
// 			glActiveTexture(GL_TEXTURE0);
// 			glBindTexture(GL_TEXTURE_2D, texture);

// 			glActiveTexture(GL_TEXTURE1);
// 			glBindTexture(GL_TEXTURE_2D, texture2);
// 			model = glm::mat4(1.0f);
// 			model = glm::translate(model, cubePositions[i]);
// 			myshdr.setMatrix("model", model);
// 			glBindVertexArray(VAO);
// 			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// 			glDrawArrays(GL_TRIANGLES, 0, 36);
// 		}
// 		/*m.draw(myshdr);*/


// 		myModelShdr.use();
// 		myModelShdr.setMatrix("proj", proj);
// 		myModelShdr.setMatrix("view", view);
// 		myModelShdr.setMatrix("model", model);
// 		myModelShdr.setFloat("material.shininess", 16.0f);
// 		myModelShdr.setVec3("light.ambient", glm::vec3(0.2f));
// 		myModelShdr.setVec3("light.diffuse", glm::vec3(0.0f));
// 		myModelShdr.setVec3("light.specular", glm::vec3(1.0f));
// 		myModelShdr.setFloat("light.constant", 1.0f);
// 		myModelShdr.setFloat("light.linear", 0.0f);
// 		myModelShdr.setFloat("light.quadratic", 0.0f);
// 		myModelShdr.setVec3("lightpos", glm::vec3(lmodel2 * glm::vec4(lpos, 0.0f)));
// 		myModelShdr.setVec3("camPos", cam.position);

// 		model = glm::mat4(1.0f);
// 		model = glm::rotate(model, ang, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
// 		model = glm::translate(model, glm::vec3(3.5f, 0.0f, 0.0f));
// 		model = glm::rotate(model, ang, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
// 		myModelShdr.setMatrix("model", model);
// 		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
// 		m.draw(myModelShdr);
// 		//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


// 		model = glm::mat4(1.0f);
// 		//model = glm::rotate(model, ang, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
// 		model = glm::scale(model, glm::vec3(0.1f));
// 		myModelShdr.setMatrix("model", model);
// 		myModel.draw(myModelShdr);

// 		lightshdr.use();
// 		lightshdr.setMatrix("view", view);
// 		//lmodel = glm::mat4(1.0f);
// 		//lmodel = glm::translate(lmodel, lpos);
// 		//lmodel = glm::scale(lmodel, glm::vec3(0.2f));
// 		lightshdr.setMatrix("model", lmodel2);
// 		ang += dang;
// 		glBindVertexArray(lVAO);
// 		glDrawArrays(GL_TRIANGLES, 0, 36);

// 		window.update();
// 		/*glfwSwapBuffers(window);
// 		glfwPollEvents();*/
// 	}

// 	glfwTerminate();
// 	return 0;
// }


// //void loadTexture(const std::string& path, unsigned int *tId) {
// //
// //	int width, height, nChannels;
// //	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nChannels, 0);
// //
// //	glGenTextures(1, tId);
// //	glBindTexture(GL_TEXTURE_2D, *tId);
// //
// //
// //	GLenum fmt = (nChannels == 4) ? GL_RGBA : GL_RGB;
// //
// //	if (data) {
// //		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, fmt, GL_UNSIGNED_BYTE, data);
// //		glGenerateMipmap(GL_TEXTURE_2D);
// //	}
// //
// //	else {
// //		std::cerr << "Failed to load texture" << std::endl;
// //	}
// //
// //	stbi_image_free(data);
// //}

// void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
// 	glViewport(0, 0, width, height);
// }

// void processInput(GLFWwindow* window)
// {
// 	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
// 		glfwSetWindowShouldClose(window, true);

// 	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
// 		cam.move(Camera::UP, dt);

// 	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
// 		cam.move(Camera::DOWN, dt);

// 	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
// 		cam.move(Camera::RIGHT, dt);

// 	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
// 		cam.move(Camera::LEFT, dt);

// 	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
// 		cam.yaw -= cam.sensitivity;
// 		cam.updateDirection();
// 	}
// }

// void mouse_callback(GLFWwindow* window, double xPos, double yPos) {
// 	cam.handleMouse(xPos, yPos);
// }