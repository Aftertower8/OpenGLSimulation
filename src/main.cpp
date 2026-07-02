#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
			//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			vao.Bind();
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
			vao.Unbind();
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


void run(GLFWwindow *window){
	Shader shaderProgram("../resources/shaders/default.vert", "../resources/shaders/default.frag");

	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	Circle kolo(0.5, 3);
	kolo.initGL();

	//Main while loop
	while(!glfwWindowShouldClose(window)){
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);	
		//Tell OpenGL thich shader program we want to use
		shaderProgram.Activate();
		//only after activating shaderProgram
		glm::mat4 trans = glm::mat4(1.0f);
		//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
		//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		unsigned int transformLoc = glGetUniformLocation(shaderProgram.ID, "transform");
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
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

	shaderProgram.Delete();
}

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
	
	run(window);

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
