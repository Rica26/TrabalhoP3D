#include "LoadObj.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "stb_image.h"
#include "LoadShaders.h"

namespace LoadObj {

#define CHECK_GL_ERROR() { \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) { \
        std::cerr << "OpenGL error: " << err << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
}


	Ball::Ball(glm::vec3 position, GLuint shaderProgram, const Camera& camera, glm::vec3 orientation, GLfloat scale):position(position),rotationMatrix(rotationMatrix), shaderProgram(shaderProgram), camera(camera), orientation(orientation), scale(scale) {
	


	}
		


	void Ball::Load(const std::string obj_model_filepath)
	{
		FILE* file;
		errno_t err;
		err = fopen_s(&file, obj_model_filepath.c_str(), "r");
		if (file == NULL) {
			throw("Impossible to open the obj file !\n");
			return;
		}

		std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_uvs;
		std::vector< glm::vec3 > temp_normals;

		while (1) {
			char lineHeader[128];
			int res = fscanf_s(file, "%s", lineHeader, (unsigned int)_countof(lineHeader));
			if (res == EOF)
				break;

			if (strcmp(lineHeader, "mtllib") == 0) {
			
				char materialsFilename[128];

				fscanf_s(file, "%s\n", materialsFilename, (unsigned int)_countof(materialsFilename));
				LoadMaterial(materialsFilename);
				//std::cout << "Abrindo lineHeader mtllib" << std::endl;
			}

			if (strcmp(lineHeader, "v") == 0) {
				glm::vec3 vertex;
				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
				//std::cout<<"abrindo lineHeader v"<<std::endl;
			}

			else if (strcmp(lineHeader, "vt") == 0) {
				glm::vec2 uv;
				fscanf_s(file, "%f %f\n", &uv.x, &uv.y);
				temp_uvs.push_back(uv);
			}

			else if (strcmp(lineHeader, "vn") == 0) {
				glm::vec3 normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}

			else if (strcmp(lineHeader, "f") == 0) {
				std::string vertex1, vertex2, vertex3;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					throw("Failed to read face information\n");
					return;
				}

				for (int i = 0; i < 3; i++)
				{
					vertices.push_back(temp_vertices.at(vertexIndex[i] - 1));
					textcoords.push_back(temp_uvs.at(uvIndex[i] - 1));
					normals.push_back(temp_normals.at(normalIndex[i] - 1));
				}

			};
		}
		fclose(file);
		return;
		
	}
	void Ball::LoadMaterial(char* mtl_file_path) {
		std::cout << "Attempting to load material file: " << mtl_file_path << std::endl; // Debug message
		char lineHeader[128];
		FILE* mtlFile;
		errno_t err;
		err = fopen_s(&mtlFile, mtl_file_path, "r");
		if (mtlFile == NULL) {
			std::cerr << "Impossible to open the Material file: " << mtl_file_path << std::endl; // Error message
			return;
		}

		//char lineHeader[128];
		while (lineHeader[0] != EOF) {

			int res = fscanf_s(mtlFile, "%s", lineHeader, sizeof(lineHeader));
			if (res == EOF)
				break; // EOF = End Of File. Quit the loop

			if (strcmp(lineHeader, "Ka") == 0) {
				glm::vec3 ambientColor;
				fscanf_s(mtlFile, "%f %f %f", &ambientColor.r, &ambientColor.g, &ambientColor.b);
			}
			else if (strcmp(lineHeader, "Kd") == 0) {
				glm::vec3 diffuseColor;
				fscanf_s(mtlFile, "%f %f %f", &diffuseColor.r, &diffuseColor.g, &diffuseColor.b);
			}
			else if (strcmp(lineHeader, "Ks") == 0) {
				glm::vec3 specularColor;
				fscanf_s(mtlFile, "%f %f %f", &specularColor.r, &specularColor.g, &specularColor.b);
			}
			else if (strcmp(lineHeader, "Ns") == 0) {
				float shininess;
				fscanf_s(mtlFile, "%f", &shininess);
			}
			else if (strcmp(lineHeader, "map_Kd") == 0) {
				char textureFilename[128];
				fscanf_s(mtlFile, "%s", textureFilename, sizeof(textureFilename));
				LoadTexture(textureFilename);
			}
		}
		fclose(mtlFile);
	}
	void Ball::LoadTexture(const char* file_path)
	{
		//GLuint textureID;
		glGenTextures(1, &textureID);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		int width, height, nChannels;
		// Ativa a inversão vertical da imagem, aquando da sua leitura para memória.
		stbi_set_flip_vertically_on_load(true);
		// Leitura da imagem para memória do CPU
		unsigned char* imageData = stbi_load(file_path, &width, &height, &nChannels, 0);
		if (imageData) {
			// Carrega os dados da imagem para o Objeto de Textura vinculado ao target GL_TEXTURE_2D da Unidade de Textura ativa.
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);

			// Gera o Mipmap para essa textura
			glGenerateMipmap(GL_TEXTURE_2D);

			// Liberta a imagem da memória do CPU
			stbi_image_free(imageData);
		}
		else {
			std::cout << "Error loading texture!" << std::endl;
		}
	}

	

	


	void Ball::Install()
	{
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		CHECK_GL_ERROR();

		glGenBuffers(1, &VBO_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
		CHECK_GL_ERROR();

		glGenBuffers(1, &VBO_normals);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
		CHECK_GL_ERROR();

		glGenBuffers(1, &VBO_textcoords);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_textcoords);
		glBufferData(GL_ARRAY_BUFFER, textcoords.size() * sizeof(glm::vec2), textcoords.data(), GL_STATIC_DRAW);
		CHECK_GL_ERROR();

	
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(0);
		CHECK_GL_ERROR();

		
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(1);
		CHECK_GL_ERROR();

		// Coordenadas de Textura
		glBindBuffer(GL_ARRAY_BUFFER, VBO_textcoords);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glEnableVertexAttribArray(2);
		CHECK_GL_ERROR();

		GLint textura = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "ballSampler");
		glProgramUniform1i(shaderProgram, textura, 0);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cout << "OpenGL Error: " << error << std::endl;
		}

		glBindVertexArray(0);
		CHECK_GL_ERROR();
	}


	void Ball::UpdateRotationMatrix(const glm::mat4& rotationMatrix) {
		this->rotationMatrix = rotationMatrix;
	}


	void Ball::Render(glm::vec3 position, glm::vec3 orientation) 
	{
		this->position = position;
		//GLuint textureID=0;
		//this->rotationMatrix = rotationMatrix;
		UpdateRotationMatrix(rotationMatrix);

		glBindVertexArray(VAO);
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//glActiveTexture(GL_TEXTURE0);
		
		//glm::mat4 RotationMatrix = glm::mat4_cast(currentRotation);
		model = glm::mat4(1.0f);
		model= glm::scale(model, glm::vec3(scale));
		model = model * rotationMatrix;
		model = glm::translate(model, position);
		
		//model = glm::rotate(model, glm::radians(orientation.x), glm::vec3(1, 0, 0));
		//model = glm::rotate(model, glm::radians(orientation.y), glm::vec3(0, 1, 0));
		//model = glm::rotate(model, glm::radians(orientation.z), glm::vec3(0, 0, 1));

		view = camera.getViewMatrix();
		projection = camera.getProjectionMatrix(4.0f / 3.0f);
		glm::mat4 modelView = view  * model;
		normalMatrix = glm::inverseTranspose(glm::mat3(modelView));

		glUseProgram(shaderProgram);

		GLint modelLoc = glGetUniformLocation(shaderProgram, "Model");
		GLint viewLoc = glGetUniformLocation(shaderProgram, "View");
		GLint projectionLoc = glGetUniformLocation(shaderProgram, "Projection");
		GLint normalMatrixLoc = glGetUniformLocation(shaderProgram, "NormalMatrix");

		CHECK_GL_ERROR();

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		/*glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));

		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].position"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 5.0)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].constant"), 1.0f);
		glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].linear"), 0.06f);
		glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].quadratic"), 0.02f);

		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].position"), 1, glm::value_ptr(glm::vec3(-2.0, 2.0, 5.0)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].constant"), 1.0f);
		glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].linear"), 0.06f);
		glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].quadratic"), 0.02f);

		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.emissive"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
		glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "material.shininess"), 32.0f);*/

		//glBindTexture(GL_TEXTURE_2D, textureID);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		CHECK_GL_ERROR();

		glBindVertexArray(0);
		//std::cout << "Rendering ball at position: " << position.x << ", " << position.y << ", " << position.z << std::endl;

	
	}

	//std::vector<glm::vec3> Ball::getInitialBallPositions() {
	//	std::vector<glm::vec3> positions = {
	//		glm::vec3(0.0f, 0.0f, 0.0f), // Ball 1
	//		glm::vec3(0.1f, 0.0f, 0.0f), // Ball 2
	//		glm::vec3(0.2f, 0.0f, 0.0f), // Ball 3
	//		glm::vec3(0.3f, 0.0f, 0.0f), // Ball 4
	//		glm::vec3(0.4f, 0.0f, 0.0f), // Ball 5
	//		glm::vec3(0.5f, 0.0f, 0.0f), // Ball 6
	//		glm::vec3(0.6f, 0.0f, 0.0f), // Ball 7
	//		glm::vec3(0.7f, 0.0f, 0.0f), // Ball 8
	//		glm::vec3(0.8f, 0.0f, 0.0f), // Ball 9
	//		glm::vec3(0.9f, 0.0f, 0.0f), // Ball 10
	//		glm::vec3(1.0f, 0.0f, 0.0f), // Ball 11
	//		glm::vec3(1.1f, 0.0f, 0.0f), // Ball 12
	//		glm::vec3(1.2f, 0.0f, 0.0f), // Ball 13
	//		glm::vec3(1.3f, 0.0f, 0.0f), // Ball 14
	//		glm::vec3(1.4f, 0.0f, 0.0f)  // Ball 15
	//	};

	//	return positions;
	//}
	/*void Ball::Rotation(double dx) {
		float angleY = glm::radians((float)dx);
		glm::quat qy = glm::angleAxis(angleY, glm::vec3(0.0f, 1.0f, 0.0f));
		currentRotation = qy * currentRotation;
		currentRotation = glm::normalize(currentRotation);
	}*/
 
}