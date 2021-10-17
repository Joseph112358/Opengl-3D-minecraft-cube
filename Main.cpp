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

const unsigned int width = 800;
const unsigned int height = 800;

GLfloat vertices[] =
{//         COORDINATES    /      COLORS      /    TEX COORDS /
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // Lower left corner 0
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // Upper left corner 1
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f,// Upper right corner 2
	 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 1.0f,	1.0f, 0.0f, // Lower right corner 3
	 0.5f, -0.5f, -1.0f,    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 4
	 0.5f, 0.5f, -1.0f ,    1.0f, 1.0f, 1.0f,   0.0f ,1.0f, //5
	-0.5f, 0.5f, -1.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f,  //6
	 -0.5f, -0.5f, -1.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.0f,//7


};

GLuint indices[] = {
		0, 2, 1, // Upper triangle
	    0, 3, 2, // Lower triangle
		2, 4, 3,
		2, 5, 4,
		6, 5, 4,
		6, 7, 4,
		7, 1, 0,
		7, 1,6
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

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	//Texture
	Texture dirtblock("Minecraft_grass2.png", GL_TEXTURE_2D,GL_TEXTURE0 ,GL_RGBA, GL_UNSIGNED_BYTE);
	dirtblock.texUnit(shaderProgram, "tex0", 0);


	float rotation = 0.0f;
	double prevTime = glfwGetTime();
	
	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderProgram.Activate();

		double crntTime = glfwGetTime();
		if (crntTime - prevTime >= 1 / 60){
			rotation += 0.5f;
			prevTime = crntTime;
		}

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);

		model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
		view = glm::translate(view, glm::vec3(0.0f,-0.5f,-2.0f));
		//                                                        /closest point / furthest point we can see :)
		proj = glm::perspective(glm::radians(45.0f),(float)(width/height),0.1f,100.0f);

		int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

		glUniform1f(uniID, 0.5f);
		dirtblock.Bind();

		VAO1.Bind();
		glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
		
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