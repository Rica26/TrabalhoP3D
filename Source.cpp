#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")


#include <iostream>
#include <string>
#include <vector>
using namespace std;

#define GLEW_STATIC
#include <GL\glew.h>

#define GLFW_USE_DWM_SWAP_INTERVAL
#include <GLFW\glfw3.h>

#include "LoadShaders.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp> // glm::inverseTranspose()


void print_error(int error, const char* description);
void print_gl_info(void);
void load_textures(vector<string> textureFiles);
void init(void);
void display(void);


#define WIDTH 800
#define HEIGHT 600


GLuint VAO;
GLuint Buffer;
const GLuint NumVertices = 6 * 2 * 3; // 6 faces * 2 tri�ngulos/face * 3 v�rtices/tri�ngulo

GLuint programa;

glm::mat4 Model, View, Projection;
glm::mat3 NormalMatrix;
GLfloat angle = 3.0f;

int main(void) {
	GLFWwindow* window;

	glfwSetErrorCallback(print_error);

	if (!glfwInit()) return -1;

	window = glfwCreateWindow(WIDTH, HEIGHT, "P3D Trabalho", NULL, NULL);
	if (window == NULL) {
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	// Inicia o gestor de extens�es GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	print_gl_info();

	vector<string> textureFiles{
		"Texture/texturemesa.jpg",
		"Texture/texturemesa.jpg",
		"Texture/texturemesa.jpg",
		"Texture/texturemesa.jpg",
		"Texture/texturemesa.jpg",
		"Texture/texturemesa.jpg",
	};
	//load_textures(textureFiles);

	init();

	// Indica��o da Unidade de Textura a ligar ao sampler 'cubeMap'.
	//GLint location_textureArray = glGetProgramResourceLocation(programa, GL_UNIFORM, "cubeMap");
	//glProgramUniform1i(programa, location_textureArray, 0 /* Unidade de Textura #0 */);

	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		display();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void print_gl_info(void) {
	GLint major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);
	cout << "\nOpenGL version " << major << '.' << minor << endl;

	const GLubyte* glversion = glGetString(GL_VERSION);
	const GLubyte* glvendor = glGetString(GL_VENDOR);
	const GLubyte* glrenderer = glGetString(GL_RENDERER);
	cout << "\nVersion:  " << glversion << endl <<
		"Vendor:   " << glvendor << endl <<
		"Renderer: " << glrenderer << endl;

	cout << "\nSupported GLSL versions:\n";
	const GLubyte* glslversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
	cout << "Higher supported version:\n\t" << glslversion << endl;
	GLint numglslversions;
	cout << "Other supported versions:\n";
	glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &numglslversions);
	for (int n = 0; n < numglslversions; n++) {
		cout << '\t' << glGetStringi(GL_SHADING_LANGUAGE_VERSION, n) << endl;
	}
}

void print_error(int error, const char* description) {
	cout << description << endl;
}

void init(void) {
	// ****************************************************
	// Criar arrays de dados na RAM
	// ****************************************************

	GLfloat width = 2.0f;
	GLfloat height = 0.2f;
	GLfloat depth = 4.0f;	

	GLfloat vertices[NumVertices * (3 + 3)] = {
		// Face X+
		width / 2, -height / 2,  depth / 2,  1.0f, 0.0f, 0.0f,
		width / 2, -height / 2, -depth / 2,  1.0f, 0.0f, 0.0f,
		width / 2,  height / 2,  depth / 2,  1.0f, 0.0f, 0.0f,
		width / 2,  height / 2,  depth / 2,  1.0f, 0.0f, 0.0f,
		width / 2, -height / 2, -depth / 2,  1.0f, 0.0f, 0.0f,
		width / 2,  height / 2, -depth / 2,  1.0f, 0.0f, 0.0f,

		// Face X-
		-width / 2, -height / 2, -depth / 2, -1.0f, 0.0f, 0.0f,
		-width / 2, -height / 2,  depth / 2, -1.0f, 0.0f, 0.0f,
		-width / 2,  height / 2, -depth / 2, -1.0f, 0.0f, 0.0f,
		-width / 2,  height / 2, -depth / 2, -1.0f, 0.0f, 0.0f,
		-width / 2, -height / 2,  depth / 2, -1.0f, 0.0f, 0.0f,
		-width / 2,  height / 2,  depth / 2, -1.0f, 0.0f, 0.0f,

		// Face Y+
		-width / 2,  height / 2,  depth / 2, 0.0f, 1.0f, 0.0f,
		 width / 2,  height / 2,  depth / 2, 0.0f, 1.0f, 0.0f,
		-width / 2,  height / 2, -depth / 2, 0.0f, 1.0f, 0.0f,
		-width / 2,  height / 2, -depth / 2, 0.0f, 1.0f, 0.0f,
		 width / 2,  height / 2,  depth / 2, 0.0f, 1.0f, 0.0f,
		 width / 2,  height / 2, -depth / 2, 0.0f, 1.0f, 0.0f,

		 // Face Y-
		 -width / 2, -height / 2, -depth / 2, 0.0f, -1.0f, 0.0f,
		  width / 2, -height / 2, -depth / 2, 0.0f, -1.0f, 0.0f,
		 -width / 2, -height / 2,  depth / 2, 0.0f, -1.0f, 0.0f,
		 -width / 2, -height / 2,  depth / 2, 0.0f, -1.0f, 0.0f,
		  width / 2, -height / 2, -depth / 2, 0.0f, -1.0f, 0.0f,
		  width / 2, -height / 2,  depth / 2, 0.0f, -1.0f, 0.0f,

		  // Face Z+
		  -width / 2, -height / 2,  depth / 2, 0.0f, 0.0f, 1.0f,
		   width / 2, -height / 2,  depth / 2, 0.0f, 0.0f, 1.0f,
		  -width / 2,  height / 2,  depth / 2, 0.0f, 0.0f, 1.0f,
		  -width / 2,  height / 2,  depth / 2, 0.0f, 0.0f, 1.0f,
		   width / 2, -height / 2,  depth / 2, 0.0f, 0.0f, 1.0f,
		   width / 2,  height / 2,  depth / 2, 0.0f, 0.0f, 1.0f,

		   // Face Z-
			width / 2, -height / 2, -depth / 2, 0.0f, 0.0f, -1.0f,
		   -width / 2, -height / 2, -depth / 2, 0.0f, 0.0f, -1.0f,
			width / 2,  height / 2, -depth / 2, 0.0f, 0.0f, -1.0f,
			width / 2,  height / 2, -depth / 2, 0.0f, 0.0f, -1.0f,
		   -width / 2, -height / 2, -depth / 2, 0.0f, 0.0f, -1.0f,
		   -width / 2,  height / 2, -depth / 2, 0.0f, 0.0f, -1.0f
	};

	// ****************************************************
	// VAOs - Vertex Array Objects
	// ****************************************************

	// Gerar nomes para VAOs.
	// Neste caso gera apenas 1 nome.
	glGenVertexArrays(1, &VAO);
	// Faz bind do VAO, cujo nome est� definido em 'VAO', com o contexto do OpenGL.
	// Um VAO � criado no primero bind que lhe seja feito.
	// Este VAO passa a estar ativo at� que seja feito o bind a outro VAO, ou seja feito o bind com valor 0.
	glBindVertexArray(VAO);

	// ****************************************************
	// VBOs - Vertex Buffer Objects
	// ****************************************************

	// Gera 'NumBuffers' nomes para VBOs.
	// Neste caso gera 1 nome
	// Esta fun��o pode ser chamada antes da cria��o de VAOs.
	glGenBuffers(1, &Buffer);
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	// Inicializa o VBO (que est� ativo) com dados imut�veis.
	glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ****************************************************
	// Shaders
	// ****************************************************

	ShaderInfo  shaders[] = {
		{ GL_VERTEX_SHADER,   "Shaders/light.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/light.frag" },
		{ GL_NONE, NULL }
	};

	programa = LoadShaders(shaders);
	if (!programa) exit(EXIT_FAILURE);
	glUseProgram(programa);

	// ****************************************************
	// Ligar Atributos aos Shaders
	// ****************************************************

	// Obt�m a localiza��o do atributo 'vPosition' no 'programa'.
	GLint coordsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition");
	// Obt�m a localiza��o do atributo 'vNormal' no 'programa'.
	GLint normalId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vNormal");

	// Ativa o VBO 'Buffer'.
	glBindBuffer(GL_ARRAY_BUFFER, Buffer);
	// Liga a localiza��o do atributo 'vPosition' dos shaders do 'programa', ao VBO e VAO (ativos).
	// Especifica tamb�m como � que a informa��o do atributo 'coordsId' deve ser interpretada.
	// Neste caso, o atributo ir� receber, por v�rtice, 3 elementos do tipo float. Stride de 6 floats e offset de 0 bytes.
	glVertexAttribPointer(coordsId, 3 /*3 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_FALSE, (3 + 3) * sizeof(float) /*stride*/, (void*)0);
	glVertexAttribPointer(normalId, 3 /*3 elementos por v�rtice*/, GL_FLOAT/*do tipo float*/, GL_TRUE, (3 + 3) * sizeof(float) /*stride*/, (void*)(3 * sizeof(float)));

	// Habitita o atributo com localiza��o 'coresId' para o VAO ativo.
	glEnableVertexAttribArray(coordsId);
	// Habitita o atributo com localiza��o 'normalId' para o VAO ativo.
	glEnableVertexAttribArray(normalId);

	// ****************************************************
	// Matrizes de transforma��o
	// ****************************************************

	Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
	View = glm::lookAt(
		glm::vec3(0.0f, 3.0f, 5.0f),	// eye (posi��o da c�mara).
		glm::vec3(0.0f, 0.0f, 0.0f),	// center (para onde est� a "olhar")
		glm::vec3(0.0f, 1.0f, 0.0f)		// up
	);
	Model = glm::rotate(glm::mat4(5.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 ModelView = View * Model;
	NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));

	// ****************************************************
	// Uniforms
	// ****************************************************

	// Atribui valor ao uniform Model
	GLint modelId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Model");
	glProgramUniformMatrix4fv(programa, modelId, 1, GL_FALSE, glm::value_ptr(Model));
	// Atribui valor ao uniform View
	GLint viewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "View");
	glProgramUniformMatrix4fv(programa, viewId, 1, GL_FALSE, glm::value_ptr(View));
	// Atribui valor ao uniform ModelView
	GLint modelViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "ModelView");
	glProgramUniformMatrix4fv(programa, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
	// Atribui valor ao uniform Projection
	GLint projectionId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Projection");
	glProgramUniformMatrix4fv(programa, projectionId, 1, GL_FALSE, glm::value_ptr(Projection));
	// Atribui valor ao uniform NormalMatrix
	GLint normalViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "NormalMatrix");
	glProgramUniformMatrix3fv(programa, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

	// Fonte de luz ambiente global
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));

	// Fonte de luz direcional
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));

	// Fonte de luz pontual #1
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].position"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 5.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].constant"), 1.0f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].linear"), 0.06f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].quadratic"), 0.02f);

	// Fonte de luz pontual #2
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[1].position"), 1, glm::value_ptr(glm::vec3(-2.0, 2.0, 5.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[1].ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[1].diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[1].specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[1].constant"), 1.0f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[1].linear"), 0.06f);
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[1].quadratic"), 0.02f);

	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.emissive"), 1, glm::value_ptr(glm::vec3(0.0, 0.5, 0.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
	glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "material.shininess"), 12.0f);

	// ****************************************************
	// Definir a janela de visualiza��o (viewport)
	// ****************************************************

	glViewport(0, 0, WIDTH, HEIGHT);

	// ****************************************************
	// Outros par�metros do OpenGL
	// ****************************************************
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); // Por defeito est� desativado
	glCullFace(GL_BACK); // GL_FRONT, [GL_BACK], GL_FRONT_AND_BACK
}

void display(void) {
	static const GLfloat black[] = {
		0.0f, 0.0f, 0.0f, 0.0f
	};
	// Limpa o buffer de cor
	glClearBufferfv(GL_COLOR, 0, black);
	// Limpa o buffer de profundidade
	glClear(GL_DEPTH_BUFFER_BIT);

	// Atualiza os dados do Uniform
	//Model = glm::rotate(glm::mat4(1.0f), angle += 0.0002f, glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f)));
	glm::mat4 ModelView = View * Model;
	NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));
	// Atribui valor ao uniform Model
	GLint modelId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Model");
	glProgramUniformMatrix4fv(programa, modelId, 1, GL_FALSE, glm::value_ptr(Model));
	GLint viewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "View");
	glProgramUniformMatrix4fv(programa, viewId, 1, GL_FALSE, glm::value_ptr(View));
	GLint modelViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "ModelView");
	glProgramUniformMatrix4fv(programa, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
	// Atribui valor ao uniform NormalMatrix
	GLint normalViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "NormalMatrix");
	glProgramUniformMatrix3fv(programa, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

	// Vincula (torna ativo) o VAO
	glBindVertexArray(VAO);
	// Envia comando para desenho de primitivas GL_TRIANGLES, que utilizar� os dados do VAO vinculado.
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void load_textures(vector<string> textureFiles) {
	GLuint textureName = 0;

	// Gera um nome de textura
	glGenTextures(1, &textureName);

	// Ativa a Unidade de Textura #0
	// A Unidade de Textura 0 est� ativa por defeito.
	// S� uma Unidade de Textura pode estar ativa.
	glActiveTexture(GL_TEXTURE0);

	// Vincula esse nome de textura ao target GL_TEXTURE_CUBE_MAP da Unidade de Textura ativa.
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureName);

	// NOTA:
	// Num cube map de texturas, todas as texturas devem:
	// - ter a mesma resolu��o;
	// - possuir o mesmo n�mero de n�veis de mipmap; e,
	// - partilhar os mesmos par�metros.

	// Define os par�metros de filtragem (wrapping e ajuste de tamanho)
	// para a textura que est� vinculada ao target GL_TEXTURE_CUBE_MAP da Unidade de Textura ativa.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

#ifdef _D_STORAGE
	// Aloca mem�ria para o cube map de texturas
	// Textura imut�vel, i.e., apenas � poss�vel alterar a imagem.
	{
		// Leitura da resolu��o e n�mero de canais da imagem.
		int width, height, nChannels;
		// Ativa a invers�o vertical da imagem, aquando da sua leitura para mem�ria.
		stbi_set_flip_vertically_on_load(true);
		unsigned char* imageData = stbi_load(textureFiles[0].c_str(), &width, &height, &nChannels, 0);
		if (imageData) {
			stbi_image_free(imageData);

			// Aloca��o de mem�ria
			glTexStorage2D(GL_TEXTURE_CUBE_MAP,
				1,					// N�mero de n�veis de Mipmap para as texturas. 1 se n�o forem utilizados Mipmaps.
				nChannels == 4 ? GL_RGBA8 : GL_RGB8,	// Formato interno da imagem de textura
				width, height		// width, height
			);
		}
		else {
			cout << "Error loading texture!" << endl;
		}
	}

	// Para cada face do cubo
	GLint face = 0;
	for (auto file : textureFiles) {
		// Leitura/descompress�o do ficheiro com imagem de textura
		int width, height, nChannels;
		unsigned char* imageData = stbi_load(file.c_str(), &width, &height, &nChannels, 0);
		if (imageData) {
			// Carrega os dados da imagem para o Objeto de Textura vinculado ao target da face
			glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
				0,					// N�vel do Mipmap
				0, 0,				// xoffset, yoffset
				width, height,		// width, height
				nChannels == 4 ? GL_RGBA : GL_RGB,	// Formato da imagem
				GL_UNSIGNED_BYTE,	// Tipos dos dados da imagem
				imageData);			// Apontador para os dados da imagem de textura

			face++;

			// Liberta a imagem da mem�ria do CPU
			stbi_image_free(imageData);
		}
		else {
			cout << "Error loading texture!" << endl;
		}
	}
#else
	// Ativa a invers�o vertical da imagem, aquando da sua leitura para mem�ria.
	stbi_set_flip_vertically_on_load(true);

	// Para cada face do cubo
	GLint face = 0;

	for (auto file : textureFiles) {
		// Leitura/descompress�o do ficheiro com imagem de textura
		int width, height, nChannels;
		unsigned char* imageData = stbi_load(file.c_str(), &width, &height, &nChannels, 0);
		if (imageData) {
			// Carrega os dados da imagem para o Objeto de Textura vinculado ao target da face
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face,
				0,					// N�vel do Mipmap
				GL_RGB,				// Formato interno do OpenGL
				width, height,		// width, height
				0,					// border
				nChannels == 4 ? GL_RGBA : GL_RGB,	// Formato da imagem
				GL_UNSIGNED_BYTE,	// Tipos dos dados da imagem
				imageData);			// Apontador para os dados da imagem de textura

			face++;

			// Liberta a imagem da mem�ria do CPU
			stbi_image_free(imageData);
		}
		else {
			cout << "Error loading texture!" << endl;
		}
	}
#endif
}


