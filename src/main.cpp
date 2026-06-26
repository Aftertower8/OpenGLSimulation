#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>

#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"

#define WIDTH 800
#define HEIGHT 800

class Circle{
	public:
		Circle(float radius, int step){
			this->radius = radius;
			this->step = step;
			setVertices();
			setIndeces();
		}
		void initGL(){
			vao.Bind();
			vbo.Upload((GLfloat*)vertices.data(), (GLsizeiptr)(vertices.size() * sizeof(glm::vec3)));
			ebo.Upload((GLuint*)indices.data(), (GLsizeiptr)(indices.size() * sizeof(GLuint)));
			vao.LinkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(glm::vec3), (void*)0);
			vao.Unbind();
			vbo.Unbind();
			ebo.Unbind();
		}
		void drawGL(){
			vao.Bind();
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		}
		void deleteGL(){
			vao.Bind();
			vbo.Delete();
			ebo.Delete();
		}
	private:
		std::vector<glm::vec3> vertices;
		std::vector<GLuint> indices;
		float radius;
		int step;
		VAO vao;
		VBO vbo;
		EBO ebo;
		void setVertices(){
			vertices.resize(step+1);
			vertices[0] = glm::vec3{0.0f, 0.0f, 0.0f};
			for(int i=1; i<=step; i++){
				double angle = 2.0f * M_PI / step * i;
				vertices[i] = glm::vec3{radius * cos(angle), radius * sin(angle), 0.0f};
			}
		}
		void setIndeces(){
			for(int i=1; i<step; i++){
				indices.push_back(0);
				indices.push_back(i);
				indices.push_back(i+1);
			}
			indices.push_back(0);
			indices.push_back(1);
			indices.push_back(step);
		}
};

//Vertices coordinates <-1;1>
	GLfloat vertices[] = {
		-0.5f, 	-0.5f * float(sqrt(3)) / 3, 	0.0f,	0.8f, 0.3f, 0.02f,	//lower left corner
		0.5f, 	-0.5f * float(sqrt(3)) / 3, 	0.0f,	0.8f, 0.3f, 0.02f,	//lower right corner
		0.0f, 	0.5f * float(sqrt(3)) * 2 / 3, 	0.0f,	1.0f, 0.6f, 0.32f,	//upper corner
		-0.25f, 0.5f * float(sqrt(3)) / 6, 		0.0f,	0.9f, 0.45f, 0.17f,	//inner left
		0.25f, 	0.5f * float(sqrt(3)) / 6, 		0.0f,	0.9f, 0.45f, 0.17f,	//inner right
		0.0f, 	-0.5f * float(sqrt(3)) / 3, 	0.0f,	0.8f, 0.3f, 0.02f	//inner down
	};

	GLuint indices[] = {
		0, 3, 5, //lower left triangle
		3, 2, 4, //lower right triangle
		5, 4, 1 //upper triangle
	};


int main(){
	//init GLFW
	glfwInit();
	
	//Tell GLFW OpenGL version
	//OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Tell GLFW that we use CORE profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	
	//Create GLFWwindow object WIDTH x HEIGHT, named SIM
	GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "SIM", NULL, NULL);
	//Error check (failed to create window)
	if(window == NULL){
		std::cout << "Failed to create window\n";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	gladLoadGL();

	//Specify viewport of OpenGL in the window
	//x = <0;WIDTHT>
	//y = <0;HEIGHT>
	glViewport(0, 0, WIDTH, HEIGHT);
	
	Shader shaderProgram("../resources/shaders/default.vert", "../resources/shaders/default.frag");
	/*
	VAO VAO1;
	VAO1.Bind();

	VBO VBO1;
	VBO1.Upload(vertices, sizeof(vertices));
	EBO EBO1;
	EBO1.Upload(indices, sizeof(indices));

	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();
	*/

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	Circle kolo(0.2, 30);
	kolo.initGL();

	//Main while loop
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
		//Tell OpenGL thich shader program we want to use
		shaderProgram.Activate();
		//only after activating shaderProgram
		glUniform1f(uniID, 0.1);
		//Bind the VAO so OpenGL knows how to use it
		//VAO1.Bind();
		//Draw the triangle using the GL_TRIANGLES primitive
		//glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		//Swap the back buffer with the front buffer
		//circle class test:
		kolo.drawGL();


		glfwSwapBuffers(window);
		//Take care of all GLFW elements
		glfwPollEvents();
	}

	//Delete all the objects we've created
	//VAO1.Delete();
	//VBO1.Delete();
	//EBO1.Delete();
	kolo.deleteGL();
	shaderProgram.Delete();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
