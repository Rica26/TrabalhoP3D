#ifndef LOADOBJ_H
#define LOADOBJ_H

#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <Windows.h>
#include <gl\glew.h>
#include <GL\GL.h>
#include "Camera.h"


#include <GLFW\glfw3.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/quaternion.hpp>

namespace LoadObj
{
	class Ball
	{

	public:
		Ball(glm::vec3 position, GLuint shaderProgram, const Camera& camera, glm::vec3 orientation=glm::vec3(0,0,0), GLfloat scale=0.08f);
		void Load(const std::string obj_model_filepath);
		void Install();
		void Render(glm::vec3 position, glm::vec3 orientation);
		glm::vec3 position;
		glm::vec3 orientation;
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> textcoords;
		//static std::vector<glm::vec3> getInitialBallPositions();
		//void Rotation(double dx);
		void UpdateRotationMatrix(const glm::mat4& rotationMatrix);
		
	private:

		void LoadMaterial(char* mtl_file_path);
		void LoadTexture(const char* file_path);
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
		glm::mat3 normalMatrix;
		glm::mat4 rotationMatrix;

		// VAO e VBO
		GLuint VAO;
		GLuint VBO_vertices;
		GLuint VBO_normals;
		GLuint VBO_textcoords;
		GLuint shaderProgram;
		GLuint textureID;
		const Camera& camera;
		GLfloat scale;
		glm::quat currentRotation;

	};


	

}

#endif // LOADOBJ_H