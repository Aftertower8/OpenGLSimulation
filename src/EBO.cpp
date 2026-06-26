#include "EBO.h"

EBO::EBO(){
	glGenBuffers(1, &ID);
}

void EBO::Upload(GLuint* indices, GLsizeiptr size){
	//Bind the EBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	//Introduce the vertices into the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
}

void EBO::Bind(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void EBO::Unbind(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete(){
	glDeleteBuffers(1, &ID);
}

