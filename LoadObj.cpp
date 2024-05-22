//#include "LoadObj.h"
//#include <iostream>
//#include <fstream>
//#include <sstream>
//#include <stdexcept>
//#include "stb_image.h"
//#include "LoadShaders.h"
//
//namespace LoadObj {
//
//	///*void Ball::Load(const std::string& obj_model_filepath)
//	//{
//	//	std::ifstream file(obj_model_filepath);
// //   
//	//	if (!file.is_open()) {
// //       
//	//		throw ("Não foi possível abrir o ficheiro");
// //   
//	//	}
//
//	//	std::string line;
//	//	while (std::getline(file, line)) 
//	//	{
//	//		std::stringstream ss(line);
//	//		std::string prefix;
//	//		ss >> prefix;
//	//		if (prefix == "v")
//	//		{
//	//			glm::vec3 vert;
//	//			ss >> vert.x >> vert.y >> vert.z;
//	//			vertices.push_back(vert);
//	//		}
//	//		else if (prefix == "vn")
//	//		{
//	//			glm::vec3 normal;
//	//			ss >> normal.x >> normal.y >> normal.z;
//	//			normals.push_back(normal);
//	//		}
//	//		else if (prefix == "vt")
//	//		{
//	//			glm::vec2 uv;
//	//			ss >> uv.x >> uv.y;
//	//			textcoords.push_back(uv);
//	//		}
//	//		else if (prefix == "f") {
//	//			// Face (simplificação: assumindo formato específico)
//	//			unsigned int vertexIndex[3], normalIndex[3], uvIndex[3];
//	//			char slash;
//	//			for (int i = 0; i < 3; i++) {
//	//				ss >> vertexIndex[i] >> slash >> uvIndex[i] >> slash >> normalIndex[i];
//	//				// OBJ indices are 1-based, so we need to subtract 1
//	//				indiceVert.push_back(vertexIndex[i] - 1);
//	//				indiceNormal.push_back(normalIndex[i] - 1);
//	//				indiceUv.push_back(uvIndex[i] - 1);
//	//				// We could use uvIndex[i] and normalIndex[i] to further process the data
//	//			}
//	//		}
//	//	}
//	//
//	//	file.close();
//	//}
//	//void Ball::LoadMaterial(const std::string& mtl_file_path) {
//	//	std::ifstream file(mtl_file_path);
//
//	//	if (!file.is_open()) {
//	//		throw std::runtime_error("Failed to open the material file");
//	//	}
//
//	//	std::string line;
//	//	while (std::getline(file, line)) {
//	//		std::stringstream ss(line);
//	//		std::string prefix;
//	//		ss >> prefix;
//
//	//		if (prefix == "Ka") {
//	//			ss >> material.ambient.r >> material.ambient.g >> material.ambient.b;
//	//		}
//	//		else if (prefix == "Kd") {
//	//			ss >> material.diffuse.r >> material.diffuse.g >> material.diffuse.b;
//	//		}
//	//		else if (prefix == "Ks") {
//	//			ss >> material.specular.r >> material.specular.g >> material.specular.b;
//	//		}
//	//		else if (prefix == "Ns") {
//	//			ss >> material.shininess;
//	//		}
//	//		else if (prefix == "map_Kd") {
//	//			ss >> material.textureFilePath;
//	//			// Load the texture
//	//			LoadTexture(material.textureFilePath, material.textureID);
//	//		}
//	//	}
//
//	//	file.close();
//	//}
//
//	//void LoadTexture(const std::string& file_path, GLuint& textureID)
//	//{
//	//	int width, height, nrChannels;
//	//	unsigned char* data=stbi_load(file_path.c_str(), &width, &height, &nrChannels, 0);
//
//	//	if (data)
//	//	{
//	//		glGenTextures(1, &textureID);
//	//		glBindTexture(GL_TEXTURE_2D, textureID);
//	//		glTexImage2D(GL_TEXTURE_2D,0, GL_RGB,width,height,0,GL_RGB, GL_UNSIGNED_BYTE, data);
//	//		glGenerateMipmap(GL_TEXTURE_2D);
//	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	//		stbi_image_free(data);	
//	//	}
//	//	else
//	//	{
//	//		throw ("Falha ao carregar a textura");
//	//	}
//	//}
//	//void Ball:: Install() 
//	//{
//	//	/*ShaderInfo shaders[] = {
//	//		{ GL_VERTEX_SHADER, "triangles.vert" },
//	//		{ GL_FRAGMENT_SHADER, "triangles.frag" },
//	//		{ GL_NONE, NULL }
//	//	};*/
//	//	
//	//	glGenVertexArrays(1, &VAO);
//	//	glBindVertexArray(VAO);
//
//	//	glGenBuffers(1, &VBO_vertices);
//	//	glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
//	//	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
//	//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	//	glEnableVertexAttribArray(0);
//
//	//	glGenBuffers(1, &VBO_normals);
//	//	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
//	//	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
//	//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
//	//	glEnableVertexAttribArray(1);
//
//	//	glGenBuffers(1, &VBO_textcoords);
//	//	glBindBuffer(GL_ARRAY_BUFFER, VBO_textcoords);
//	//	glBufferData(GL_ARRAY_BUFFER, textcoords.size() * sizeof(float), textcoords.data(), GL_STATIC_DRAW);
//	//	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
//	//	glEnableVertexAttribArray(2);
//
//	//	glGenBuffers(1, &EBO);
//	//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//	//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indiceVert.size() * sizeof(unsigned int), indiceVert.data(), GL_STATIC_DRAW);
//
//
//	//	glBindVertexArray(0); // Desvincular o VAO atual para prevenir alterações acidentais
//
//	//}
//
//	//void Ball::Render(glm::vec3 position, glm::vec3 orientation) 
//	//{
//	//	glBindVertexArray(VAO);
//
//	//	glm::mat4 model = glm::mat4(1.0f);
//	//	model = glm::translate(model, position);
//	//	model = glm::rotate(model, glm::radians(orientation.x), glm::vec3(1, 0, 0));
//	//	model = glm::rotate(model, glm::radians(orientation.y), glm::vec3(0, 1, 0));
//	//	model = glm::rotate(model, glm::radians(orientation.z), glm::vec3(0, 0, 1));
//	//}
// 
//}