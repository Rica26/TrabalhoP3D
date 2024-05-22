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
#include <glm\gtc\quaternion.hpp>


void print_error(int error, const char* description);
void print_gl_info(void);
void load_textures(vector<string> textureFiles);
void init(void);
void display(void);
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

#define WIDTH 800
#define HEIGHT 600

GLuint VAO;
GLuint Buffer;
const GLuint NumVertices = 6 * 2 * 3; // 6 faces * 2 triângulos/face * 3 vértices/triângulo
GLuint programa;
glm::mat4 Model, View, Projection;
glm::mat3 NormalMatrix;
GLfloat angle = 3.0f;
bool mousePressed = false;
double lastMouseX, lastMouseY;
glm::quat currentRotation;
GLfloat ZOOM = 10.0f;

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

    // Inicia o gestor de extensões GLEW
    glewExperimental = GL_TRUE;
    glewInit();

    print_gl_info();

    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    init();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

//Função callback para o zoom do rato
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {

    // Se faz zoom in
    if (yoffset == 1) {

        // Incremento no zoom, varia com a distância da câmara
        ZOOM += fabs(ZOOM) * 0.1f;
    }

    // Senão, se faz zoom out
    else if (yoffset == -1) {

        // Incremento no zoom, varia com a distância da câmara
        ZOOM -= fabs(ZOOM) * 0.1f;
    }

}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true;
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
        }
        else if (action == GLFW_RELEASE) {
            mousePressed = false;
        }
    }
}

void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        //double dx = xpos - lastMouseX;
        double dy = ypos - lastMouseY;
        //lastMouseX = xpos;
        lastMouseY = ypos;

        //float angleX = glm::radians((float)dx);
        float angleX = glm::radians((float)dy);

        glm::quat qx = glm::angleAxis(angleX, glm::vec3(0.0f, 1.0f, 0.0f));
        //glm::quat qy = glm::angleAxis(angleY, glm::vec3(1.0f, 0.0f, 0.0f));

        currentRotation = qx * currentRotation;
        currentRotation = glm::normalize(currentRotation);
    }
}

void init(void) {
    // Criar arrays de dados na RAM
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

    // VAOs - Vertex Array Objects
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // VBOs - Vertex Buffer Objects
    glGenBuffers(1, &Buffer);
    glBindBuffer(GL_ARRAY_BUFFER, Buffer);
    glBufferStorage(GL_ARRAY_BUFFER, sizeof(vertices), vertices, 0);

    // Shaders
    ShaderInfo  shaders[] = {
        { GL_VERTEX_SHADER,   "Shaders/light.vert" },
        { GL_FRAGMENT_SHADER, "Shaders/light.frag" },
        { GL_NONE, NULL }
    };

    programa = LoadShaders(shaders);
    if (!programa) exit(EXIT_FAILURE);
    glUseProgram(programa);

    // Ligar Atributos aos Shaders
    GLint coordsId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vPosition");
    GLint normalId = glGetProgramResourceLocation(programa, GL_PROGRAM_INPUT, "vNormal");

    glBindBuffer(GL_ARRAY_BUFFER, Buffer);
    glVertexAttribPointer(coordsId, 3, GL_FLOAT, GL_FALSE, (3 + 3) * sizeof(float), (void*)0);
    glVertexAttribPointer(normalId, 3, GL_FLOAT, GL_TRUE, (3 + 3) * sizeof(float), (void*)(3 * sizeof(float)));

    glEnableVertexAttribArray(coordsId);
    glEnableVertexAttribArray(normalId);

    // Matrizes de transformação
    Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    View = glm::lookAt(
        glm::vec3(2.0f, 3.0f, 5.0f),    // eye (posição da câmara).
        glm::vec3(0.0f, 0.0f, 0.0f),    // center (para onde está a "olhar")
        glm::vec3(0.0f, 1.0f, 0.0f)        // up
    );
    Model = glm::mat4(1.0f);
    glm::mat4 ModelView = View * Model;
    NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));

    // Uniforms
    GLint modelId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Model");
    glProgramUniformMatrix4fv(programa, modelId, 1, GL_FALSE, glm::value_ptr(Model));
    GLint viewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "View");
    glProgramUniformMatrix4fv(programa, viewId, 1, GL_FALSE, glm::value_ptr(View));
    GLint modelViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "ModelView");
    glProgramUniformMatrix4fv(programa, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
    GLint projectionId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Projection");
    glProgramUniformMatrix4fv(programa, projectionId, 1, GL_FALSE, glm::value_ptr(Projection));
    GLint normalViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "NormalMatrix");
    glProgramUniformMatrix3fv(programa, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
    glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
    glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
    glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
    glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));

    glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].position"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 5.0)));
    glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
    glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
    glProgramUniform3fv(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
    glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].constant"), 1.0f);
    glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].linear"), 0.06f);
    glProgramUniform1f(programa, glGetProgramResourceLocation(programa, GL_UNIFORM, "pointLight[0].quadratic"), 0.02f);

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

    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}

void display(void) {
    static const GLfloat black[] = {
        0.0f, 0.0f, 0.0f, 0.0f
    };
    glClearBufferfv(GL_COLOR, 0, black);
    glClear(GL_DEPTH_BUFFER_BIT);

    glm::mat4 RotationMatrix = glm::mat4_cast(currentRotation);
    glm::mat4 ModelView = View * RotationMatrix * Model;
    NormalMatrix = glm::inverseTranspose(glm::mat3(ModelView));
    GLint modelId = glGetProgramResourceLocation(programa, GL_UNIFORM, "Model");
    glProgramUniformMatrix4fv(programa, modelId, 1, GL_FALSE, glm::value_ptr(RotationMatrix * Model));
    GLint viewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "View");
    glProgramUniformMatrix4fv(programa, viewId, 1, GL_FALSE, glm::value_ptr(View));
    GLint modelViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "ModelView");
    glProgramUniformMatrix4fv(programa, modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
    GLint normalViewId = glGetProgramResourceLocation(programa, GL_UNIFORM, "NormalMatrix");
    glProgramUniformMatrix3fv(programa, normalViewId, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

    glBindVertexArray(VAO);
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


