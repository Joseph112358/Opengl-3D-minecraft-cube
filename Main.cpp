#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include "Camera.h"

const unsigned int width = 800;
const unsigned int height = 800;

GLfloat vertices[] =
{//         COORDINATES    /      COLORS      /    TEX COORDS /
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.5f, // Lower left corner 0
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner 1
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	0.5f, 1.0f,// Upper right corner 2
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	0.5f, 0.5f, // Lower right corner 3
	 0.5f, -0.5f, -1.0f,    1.0f, 1.0f, 1.0f,   0.0f, 0.5f, // 4
	 0.5f, 0.5f, -1.0f ,    1.0f, 1.0f, 1.0f,   0.0f ,1.0f, //5
	-0.5f, 0.5f, -1.0f,     1.0f, 0.0f, 0.0f,   0.5f, 1.0f,  //6
	 -0.5f, -0.5f, -1.0f,   0.0f, 0.0f, 0.0f,   0.5f, 0.5f,//7

	 // top
	 -0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.5f, 0.5f, // (1) 8
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 0.5f, // (2) 9
	  0.5f, 0.5f, -1.0f ,    1.0f, 1.0f, 1.0f,  1.0f ,1.0f, //(5) 10
	  -0.5f, 0.5f, -1.0f,     1.0f, 0.0f, 0.0f,  0.5f, 1.0f,//(6) 11
	  //bottom
	- 0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, //(0) 12
	  0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	0.5f, 0.0f, //(3) 13
	  0.5f, -0.5f, -1.0f,    1.0f, 1.0f, 1.0f,   0.5f, 0.5f, //(4) 14
	   -0.5f, -0.5f, -1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.5f //(7) 15
};




GLuint indices[] = {
		0, 2, 1, // Upper triangle
	    0, 3, 2, // Lower triangle
		2, 3, 4, 
		2, 5, 4, 
		6, 5, 4,
		6, 7, 4,
		7, 1, 0,
		7, 1, 6,
		8 , 9 , 10,
		8 , 11 , 10,
		12, 13, 14, 
		12, 15, 14

};

GLfloat lightVertices[] = {
	//     COORDINATES     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
};

GLuint lightIndices[] = {
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main() {
	//initialize opengl
	glfwInit();

	//Tell GLFW what version of OpenGL we are using
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
	

	//Creates a window of width 800, height 800 called Minecraft
	GLFWwindow* window = glfwCreateWindow(width, height,"Minecraft", NULL,NULL);

	//tells us if window creation was successful or not
	if (window == NULL) {
		std::cout << "Failed to cerate window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//introduce winodw in current context
	glfwMakeContextCurrent(window);

	//load glad so it configures OpenGL in the window
	gladLoadGL();

	glViewport(0,0,width,height);


	//Generates Shader Object using shaders default.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");

	//generates vertex array object and binds it
	VAO VAO1;
	VAO1.Bind();

	//generates vertex buffer object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	//generates Element Buffer object and links it to indices
	EBO EBO1(indices, sizeof(indices));

    //links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	
	//light
	Shader lightShader("light.vert", "light.frag");
	// Generates Vertex Array Object and binds it
	VAO lightVAO;
	lightVAO.Bind();
	// Generates Vertex Buffer Object and links it to vertices
	VBO lightVBO(lightVertices, sizeof(lightVertices));
	// Generates Element Buffer Object and links it to indices
	EBO lightEBO(lightIndices, sizeof(lightIndices));
	// Links VBO attributes such as coordinates and colors to VAO
	lightVAO.LinkAttrib(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
	// Unbind all to prevent accidentally modifying them
	lightVAO.Unbind();
	lightVBO.Unbind();
	lightEBO.Unbind();
	

	
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 cubePos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 cubeModel = glm::mat4(1.0f);
	cubeModel = glm::translate(cubeModel, cubePos);

	lightShader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE,glm::value_ptr(lightModel));
	shaderProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
	//Texture
	Texture dirtblock("Atlas2.png", GL_TEXTURE_2D,GL_TEXTURE0 ,GL_RGBA, GL_UNSIGNED_BYTE);
	dirtblock.texUnit(shaderProgram, "tex0", 0);



	//

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f ));

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		camera.Inputs(window);
		camera.updateMatrix(45.0f, 0.1f, 100.0f);

		shaderProgram.Activate();
		camera.Matrix(shaderProgram, "camMatrix");

		dirtblock.Bind();

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		lightShader.Activate();
		camera.Matrix(lightShader, "camMatrix");
		lightVAO.Bind();
		glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);

		glfwPollEvents();
	}
	//29 minutes for comments
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	dirtblock.Delete();
	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}