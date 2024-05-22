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


#include <GLFW\glfw3.h>

#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtc/type_ptr.hpp>

namespace LoadObj
{
//	class Ball
//	{
//		struct Material {
//			glm::vec3 ambient; // Ambient
//			glm::vec3 diffuse; // Diffuse
//			glm::vec3 specular; // Specular
//			float shininess;     // Shininess
//			GLuint textureID; // Texture ID
//			std::string textureFilePath; // Texture file path
//		};
//	public:
//		void Load(const std::string& obj_model_filepath);
//		void Install();
//		void Render(glm::vec3 position, glm::vec3 orientation);
//		void LoadMaterial(const std::string& mtl_file_path);
//		void LoadTexture(const std::string& file_path, GLuint& textureID);
//	private:
//
//		// Estruturas de dados para armazenar vértices, normais, e coordenadas de textura
//		std::vector<glm::vec3> vertices;
//		std::vector<glm::vec3> normals;
//		std::vector<glm::vec2> textcoords;
//		std::vector<unsigned int> indiceVert,indiceNormal,indiceUv;
//
//		// VAO e VBO
//		GLuint VAO;
//		GLuint VBO_vertices;
//		GLuint VBO_normals;
//		GLuint VBO_textcoords;
//		GLuint EBO;
//		GLuint shaderProgram;
//		Material material;
//
//
//	};
//
//
//	
//
//}

#endif // LOADOBJ_H