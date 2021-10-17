#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
//EBO is Element Buffer objects
//It stores indices that OpenGL uses to decide what vertices to draw.
//

class EBO 
{
public:
	//ID reference of Elements BUffer Object
	GLuint ID;
	//constructor that generates Buffer Object and links it to indicies
	EBO(GLuint* indices, GLsizeiptr size);

	//binds the EBO
	void Bind();
	//unbinds the EBO
	void Unbind();

	void Delete();
};

#endif