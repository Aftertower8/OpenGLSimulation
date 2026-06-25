#include "shaderClass.h"

std::string get_file_contents(const char *filename){
	std::ifstream in(filename, std::ios::binary);
	if(in){
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

Shader::Shader(const char *vertexFile, const char *fragmentFile){
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	const char *vertexSource = vertexCode.c_str();
	const char *fragmentSource = fragmentCode.c_str();

	//Create Vertex Shader Object and get its reference
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	//Attach Vertex Shader source to the Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	//Compile the Vertex Shader into machine code
	glCompileShader(vertexShader);
	compileErrors(vertexShader, "VERTEX_SHADER");

	//Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	//Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	//Compile the Fragment Shader into machine code
	glCompileShader(fragmentShader);
	compileErrors(fragmentShader, "FRAGMENT_SHADER");

	//Create Shader Program Object and get its reference
	ID = glCreateProgram();
	//Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	//Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(ID);
	compileErrors(ID, "SHADER_PROGRAM");

	//Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate(){
	glUseProgram(ID);
}

void Shader::Delete(){
	glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const std::string type){
	GLint isCompiled = 0;
	char infoLog[1024];
	if(type != "SHADER_PROGRAM"){
		glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE){
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
	}
	else{
		glGetProgramiv(shader, GL_LINK_STATUS, &isCompiled);
		if(isCompiled == GL_FALSE){
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
	}
}