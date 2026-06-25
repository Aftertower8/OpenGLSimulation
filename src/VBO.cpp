#include "VBO.h"

VBO::VBO(){
	glGenBuffers(1, &ID);
	//Bind the VBO specifying it's a GL_ARRAY_BUFFER
	//glBindBuffer(GL_ARRAY_BUFFER, ID);
	//Introduce the vertices into the VBO
	//glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Upload(GLfloat* vertices, GLsizeiptr size){
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}

void VBO::Bind(){
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VBO::Unbind(){
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete(){
	glDeleteBuffers(1, &ID);
}

