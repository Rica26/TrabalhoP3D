#include "LoadObj.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "LoadShaders.h"

namespace LoadObj {

#define CHECK_GL_ERROR() { \
    GLenum err = glGetError(); \
    if (err != GL_NO_ERROR) { \
        std::cerr << "OpenGL error: " << err << " at " << __FILE__ << ":" << __LINE__ << std::endl; \
    } \
}

	const float Ball::BALL_RADIUS = 1.0f;

	Ball::Ball(glm::vec3 position, GLuint shaderProgram, const Camera& camera, glm::vec3 orientation, GLfloat scale):position(position),rotationMatrix(rotationMatrix), shaderProgram(shaderProgram), camera(camera), orientation(orientation), scale(scale), animating(false) {
	


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

		//inicialização dos vetores temporários para armazenar os vértices, 
		//normais e coordenadas de textura para ser mais organizado e promover a segurança dos dados, 
		//processando tudo antes de adicionar aos vetores correspondentes finais
		std::vector< glm::vec3 > temp_vertices;
		std::vector< glm::vec2 > temp_textCoords;
		std::vector< glm::vec3 > temp_normals;

		while (1) {
			char lineHeader[128];
			int res = fscanf_s(file, "%s", lineHeader, (unsigned int)_countof(lineHeader));
			if (res == EOF)
				break;

			// Se a linha começa com "mtllib", carrega o material
			if (strcmp(lineHeader, "mtllib") == 0) {
			
				//inicializa~ção de um array de char para armazenar o nome do ficheiro de material
				char materialsFilename[128];

				//usamos fscanf_s para para ter mais segurança e evitar buffer overflow para ler o nome do ficheiro de material e armaenar no array materialsFilename
				fscanf_s(file, "%s\n", materialsFilename, (unsigned int)_countof(materialsFilename));
				LoadMaterial(materialsFilename);
				
			}

			//se a linha começa com "v", carrega as coordenadas dos vértices
			if (strcmp(lineHeader, "v") == 0) {
				//inicialização de um vec3 para armazenar as coordenadas do vértice
				glm::vec3 vertex;
				//usamos fscanf_s para para ter mais segurança e evitar buffer overflow para ler as coordenadas do vértice e armazenar no vec3 vertex
				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				//adicionamos o vertex ao vetor de vértices temporários
				temp_vertices.push_back(vertex);
			}

			//se a linha começa com "vt", carrega as coordenadas de textura
			else if (strcmp(lineHeader, "vt") == 0) {
				//inicialização de um vec2 para armazenar as coordenadas de textura
				glm::vec2 textCoords;
				//usamos fscanf_s para para ter mais segurança e evitar buffer overflow para ler as coordenadas de textura e armazenar no vec2 textCoords
				fscanf_s(file, "%f %f\n", &textCoords.x, &textCoords.y);
				//adicionamos o textCoords ao vetor de coordenadas de textura temporárias
				temp_textCoords.push_back(textCoords);
			}

			//se a linha começa com "vn", carrega as normais
			else if (strcmp(lineHeader, "vn") == 0) {
				//inicialização de um vec3 para armazenar as normais
				glm::vec3 normal;
				//usamos fscanf_s para para ter mais segurança e evitar buffer overflow para ler as normais e armazenar no vec3 normal
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				//adicionamos o normal ao vetor de normais temporárias
				temp_normals.push_back(normal);
			}

			//se a linha começa com "f", carrega as faces
			else if (strcmp(lineHeader, "f") == 0) {
				//inicialização de 3 arrays de inteiros(unsigned porque indices não podem ser negativos) para armazenar os índices dos vértices, coordenadas de textura e normais sendo 3 porque triangulos têm 3 vértices cada
				unsigned int vertexIndex[3], textCoordsIndex[3], normalIndex[3];
				//usamos fscanf_s para para ter mais segurança e evitar buffer overflow para ler os índices dos vértices, coordenadas de textura e normais e armazenar nos arrays
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &textCoordsIndex[0], &normalIndex[0], &vertexIndex[1], &textCoordsIndex[1], &normalIndex[1], &vertexIndex[2], &textCoordsIndex[2], &normalIndex[2]);
				//se não for possível ler os 9 valores, lança uma exceção
				if (matches != 9) {
					throw("Failed to read face information\n");
					return;
				}

				//adicionamos os vértices, coordenadas de textura e normais aos vetores correspondentes, subtraindo 1 para ajustar aos índices de C++ que começam em 0 (os índices OBJ começam em 1).
				for (int i = 0; i < 3; i++)
				{
					vertices.push_back(temp_vertices.at(vertexIndex[i] - 1));
					textcoords.push_back(temp_textCoords.at(textCoordsIndex[i] - 1));
					normals.push_back(temp_normals.at(normalIndex[i] - 1));
				}

			};
		}
		//fechamos o ficheiro
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
			throw("Impossible to open the mtl file !\n");
			return;
		}



		while (lineHeader[0] != EOF) {

			int res = fscanf_s(mtlFile, "%s", lineHeader, (unsigned int)_countof(lineHeader));
			if (res == EOF)
				break;

			//se a linha começa com Ka, carrega o coeficiente de reflexão ambiente do material
			if (strcmp(lineHeader, "Ka") == 0) {
				//usamos fscanf_s para para ter mais segurança e evitar buffer overflow para ler o coeficiente de reflexão ambiente do material e armazenar no vec3 ambientColor
				fscanf_s(mtlFile, "%f %f %f", &material.ambientColor.r, &material.ambientColor.g, &material.ambientColor.b);
			}
			//se a linha começa com Kd, carrega o coeficiente de reflexão difusa do material
			else if (strcmp(lineHeader, "Kd") == 0) {
				//usamos fscanf_s para para ter mais segurança e evitar buffer overflow para ler o coeficiente de reflexão difusa do material e armazenar no vec3 diffuseColor
				fscanf_s(mtlFile, "%f %f %f", &material.diffuseColor.r, &material.diffuseColor.g, &material.diffuseColor.b);
			}
			//se a linha começa com Ks, carrega o coeficiente de reflexão especular do material
			else if (strcmp(lineHeader, "Ks") == 0) {
				//usamos fscanf_s para para ter mais segurança e evitar buffer overflow para ler o coeficiente de reflexão especular do material e armazenar no vec3 specularColor
				fscanf_s(mtlFile, "%f %f %f", &material.specularColor.r, &material.specularColor.g, &material.specularColor.b);
			}
			//se a linha começa com Ns, carrega a componente especular
			else if (strcmp(lineHeader, "Ns") == 0) {
				//usamos fscanf_s para para ter mais segurança e evitar buffer overflow para ler a componente especular do material e armazenar no float shininess
				fscanf_s(mtlFile, "%f", &material.shininess);
			}
			//se a linha começa com map_Kd, carrega a textura
			else if (strcmp(lineHeader, "map_Kd") == 0) {
				//inicialização de um array de char para armazenar o nome do ficheiro de textura
				char textureFilename[128];
				//usamos fscanf_s para para ter mais segurança e evitar buffer overflow para ler o nome do ficheiro de textura e armaenar no array textureFilename
				fscanf_s(mtlFile, "%s", textureFilename, (unsigned int)_countof(textureFilename));
				//chama a função LoadTexture para carregar a textura
				LoadTexture(textureFilename);
			}
		}
		//fechamos o ficheiro
		fclose(mtlFile);
	}
	
	void Ball::LoadTexture(const char* file_path)
	{
		// Gera um nome válido para a textura
		glGenTextures(1, &textureID);
		// Ativa a unidade de textura 0 (GL_TEXTURE0)
		glActiveTexture(GL_TEXTURE0);
		//Vincula a textura ao target GL_TEXTURE_2D da Unidade de Textura ativa
		glBindTexture(GL_TEXTURE_2D, textureID);

		int width, height, nChannels;
		// Ativa a inversão vertical da imagem, aquando da sua leitura para memória.
		stbi_set_flip_vertically_on_load(true);
		// Leitura da imagem para memória do CPU
		// width e height armazenam as dimensões da imagem.
		// nChannels armazena o número de canais de cor da imagem (por exemplo, 3 para RGB, 4 para RGBA).
		unsigned char* imageData = stbi_load(file_path, &width, &height, &nChannels, 0);
		if (imageData) {
			// Carrega os dados da imagem para o objeto de textura vinculado ao target GL_TEXTURE_2D.
			// Especifica a largura, altura e o formato dos dados de imagem.
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, nChannels == 4 ? GL_RGBA : GL_RGB, GL_UNSIGNED_BYTE, imageData);


			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			// GL_LINEAR_MIPMAP_LINEAR usa interpolação linear para minimização com mipmaps.
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
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
		//Geração e vinculação do VAO
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//Geração e vinculação dos VBOs para os vertices
		glGenBuffers(1, &VBO_vertices);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
		//Carrega os dados dos vértices para o VBO
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

		//Geração e vinculação do VBO para as normais
		glGenBuffers(1, &VBO_normals);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
		//Carrega os dados das normais para o VBO
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
	

		//Geração e vinculação do VBO para as coordenadas de textura
		glGenBuffers(1, &VBO_textcoords);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_textcoords);
		//Carrega os dados das coordenadas de textura para o VBO
		glBufferData(GL_ARRAY_BUFFER, textcoords.size() * sizeof(glm::vec2), textcoords.data(), GL_STATIC_DRAW);
		CHECK_GL_ERROR();

		//Configuração dos atributos dos vértices para a posição
		glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
		//Define o layout dos dados dos vértices no VBO.
		//Índice 0: usado no shader para acessar os dados da posição dos vértices.
		//3 componentes por vértice (x, y, z), tipo float, não normalizado,
		//espaçamento entre dados é 3 * sizeof(GLfloat), pois cada vértice possui 3 floats (x, y, z) consecutivos
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(GLfloat), (void*)0);
		//Habilita o atributo de vértice no indice 0 (posição)
		glEnableVertexAttribArray(0);
		

		//Configuração dos atributos dos vértices para as normais
		glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
		//Define o layout dos dados das normais no VBO.
		//Índice 1: usado no shader para acessar os dados das normais dos vértices.
		//3 componentes por vértice (x, y, z), tipo float, não normalizado,
		//espaçamento entre dados é 3 * sizeof(GLfloat), pois cada normal possui 3 floats (x, y, z) consecutivos
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (void*)0);
		//Habilita o atributo de vértice no indice 1 (normais)
		glEnableVertexAttribArray(1);
		

		//Configuração dos atributos dos vértices para as coordenadas de textura
		glBindBuffer(GL_ARRAY_BUFFER, VBO_textcoords);
		//Define o layout dos dados das coordenadas de textura no VBO.
		//Índice 2: usado no shader para acessar os dados das coordenadas de textura dos vértices.
		//2 componentes por vértice (s, t), tipo float, não normalizado,
		//espaçamento entre dados é 2 * sizeof(GLfloat), pois cada coordenada de textura possui 2 floats (s, t) consecutivos
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2*sizeof(GLfloat), (void*)0);
		//Habilita o atributo de vértice no indice 2 (coordenadas de textura)
		glEnableVertexAttribArray(2);
		


		//Localiza o sampler de textura no shader e define o valor da unidade de textura (0)
		GLint textura = glGetProgramResourceLocation(shaderProgram, GL_UNIFORM, "ballSampler");
		glProgramUniform1i(shaderProgram, textura, 0);
		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			std::cout << "OpenGL Error: " << error << std::endl;
		}

		glBindVertexArray(0); // Desvincula o VAO para evitar modificações acidentais
		CHECK_GL_ERROR();
	}

	

	void Ball::UpdateRotationMatrix(const glm::mat4& rotationMatrix) {
		this->rotationMatrix = rotationMatrix;
	}


	void Ball::Render(glm::vec3 position, glm::vec3 orientation) 
	{
		//Vincula o VAO
		glBindVertexArray(VAO);
		
		//Ativa a unidade de textura 0 (GL_TEXTURE0) e vincula à bola
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Cria a matriz modelo
		model = glm::mat4(1.0f);
		//Aplica a escala à matriz modelo para as bolas ficarem mais pequenas
		model= glm::scale(model, glm::vec3(scale));
		//Aplica a matriz de rotação(que vamos buscar à table) à matriz modelo
		model = model * rotationMatrix;
		//Aplica a translação à matriz modelo
		model = glm::translate(model, position);
		//Aplica as rotações à matriz modelo
		model = glm::rotate(model, glm::radians(orientation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(orientation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(orientation.z), glm::vec3(0, 0, 1));

		//cria a matriz view e a matriz de projeção através de valores da camara
		view = camera.getViewMatrix();
		projection = camera.getProjectionMatrix(4.0f / 3.0f);

		//Cria a matriz modelview para transformar os vértices do espaço do objeto para o espaço da camera facilitando o cálculo da iluminação
		glm::mat4 modelView = view  * model;
		//Calcula a matriz normal 
		//(usada para transformações normais que são cruciais para o cálculo correto da iluminação,
		//porque as normais definem como a superficie reflete a luz) através da inversa da transposta da matriz modelview
		normalMatrix = glm::inverseTranspose(glm::mat3(modelView));

		//Usa o programa de shader
		glUseProgram(shaderProgram);

		//Localiza os uniformes no shader
		GLint modelLoc = glGetUniformLocation(shaderProgram, "Model");
		GLint viewLoc = glGetUniformLocation(shaderProgram, "View");
		GLint modelViewLoc = glGetUniformLocation(shaderProgram, "ModelView");
		GLint projectionLoc = glGetUniformLocation(shaderProgram, "Projection");
		GLint normalMatrixLoc = glGetUniformLocation(shaderProgram, "NormalMatrix");

		
		//define os valores dos uniformes no shader
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
		glUniformMatrix4fv(modelViewLoc, 1, GL_FALSE, glm::value_ptr(modelView));
		
		//define os valores da luz ambiente dependendo se está "apagada" ou não
		if(activeAmbientLight)
			glUniform3fv(glGetUniformLocation(shaderProgram, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(5.0f, 5.0f, 5.0f)));
		else
			glUniform3fv(glGetUniformLocation(shaderProgram, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));

		//define os valores da luz direcional dependendo se está "apagada" ou não
		if (activeDirectionalLight) {
			glUniform3fv(glGetUniformLocation(shaderProgram, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
			glUniform3fv(glGetUniformLocation(shaderProgram, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(0.2f, 0.2f, 0.2f)));
			glUniform3fv(glGetUniformLocation(shaderProgram, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
			glUniform3fv(glGetUniformLocation(shaderProgram, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		}
		else {
			glUniform3fv(glGetUniformLocation(shaderProgram, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(1.0f, 0.0f, 0.0f)));
			glUniform3fv(glGetUniformLocation(shaderProgram, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
			glUniform3fv(glGetUniformLocation(shaderProgram, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
			glUniform3fv(glGetUniformLocation(shaderProgram, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));
		}

		//define os valores das luzes pontuais dependendo se estão "apagadas" ou não
		if (activePointLight) {
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
			
		}
		else {
			
			glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].position"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 5.0)));
			glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].ambient"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].diffuse"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].specular"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].constant"), 1.0f);
			glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].linear"), 0.06f);
			glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[0].quadratic"), 0.02f);

			glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].position"), 1, glm::value_ptr(glm::vec3(-2.0, 2.0, 5.0)));
			glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].ambient"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].diffuse"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].specular"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
			glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].constant"), 1.0f);
			glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].linear"), 0.06f);
			glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "pointLight[1].quadratic"), 0.02f);
		}

		//define os valores das componentes dos materiais dependendo dos valores lidos do ficheiro de material
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(material.ambientColor));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(material.diffuseColor));
		glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(material.specularColor));
		glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "material.shininess"), material.shininess);
		
		//Desenha a bola usando a primitva GL_TRIANGLES, começando no índice 0 e usando todos os vértices
		glDrawArrays(GL_TRIANGLES, 0, vertices.size());

		

		glBindVertexArray(0);

	
	}


	void Ball::StartAnimation() {
		animating = true;
		velocity = glm::vec3(0.0f, 0.0f, -SPEED); //Define a velocidade da bola
		angularVelocity = SPEED/BALL_RADIUS; //Define a velocidade angular da bola
	}

	void Ball::Update(float deltaTime, const std::vector<Ball>& balls) {
		//Verifica se está a animar
		if (animating) {
			//Verifica se a bola está a colidir com outra bola
			if (CheckCollision(balls)) {
				animating = false; //Para o movimento em caso de colisão
				return;
			}

			//Atualiza a posição da bola com base na velocity e no deltaTime(tempo percorrido) 
			//para garantir um movimento suave e consistente
			position += velocity * deltaTime; 

			//Calcula a distância percorrida pela bola
			float distance = glm::length(velocity * deltaTime);
			//Calcula o ângulo percorrido pela bola
			float angle = glm::degrees(distance / BALL_RADIUS);
			//Atualiza a orientação da bola para simular o movimento de rotação
			orientation.x -= angle;
		}
		
	}



	bool Ball::CheckCollision(const std::vector<Ball>& balls) {
		//Itera sobre todas as bolas do vetor de bolas fornecido
		for (const Ball& ball : balls) {
			//Verifica se a bola não é a mesma que a bola atual
			if (&ball != this) {
				//Calcula a distância entre a bola atual e a bola fornecida
				float distance = glm::distance(position, ball.getPosition());
				//Verifica se a distância é menor ou igual a 2 vezes o raio da bola(diametro)
				//Se for, as bolas estão a colidir
				if (distance <= (2.0f * BALL_RADIUS)) {
					return true; //Colisão detetada
				}
			}
		}
		return false; //Sem colisão detetada
	}

 
}