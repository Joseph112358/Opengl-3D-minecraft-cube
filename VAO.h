#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include<glad/glad.h>
#include"VBO.h"

class VAO
{
public:
	//ID reference for the vertex array object
	GLuint ID;

	//Constructor that generates a VAO ID
	VAO();

	//links a VBO to the VAO using a certain layout
	void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type,GLsizeiptr stride, void* offset);

	//Binds the VBO
	void Bind();

	void Unbind();

	void Delete();
};

#endif

