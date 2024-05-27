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

//#include "LoadShaders.h"
#include "Table.h"
#include "LoadObj.h"
#include "Camera.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp> // glm::inverseTranspose()
#include <glm\gtc\quaternion.hpp>

void print_error(int error, const char* description);
void print_gl_info(void);
void init(GLuint tableProgram);
//void display(GLuint tableProgram);
//void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);

#define WIDTH 800
#define HEIGHT 600

bool mousePressed = false;
double lastMouseX, lastMouseY;
Table table;
std::vector<glm::vec3> initialPositions = LoadObj::Ball::getInitialBallPositions();
Camera camera;

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

    glewExperimental = GL_TRUE;
    glewInit();

    print_gl_info();


    //glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    ShaderInfo tableShaders[] = {
            
        { GL_VERTEX_SHADER, "Shaders/light.vert" },
        { GL_FRAGMENT_SHADER, "Shaders/light.frag" },
        { GL_NONE, NULL }
    };

    ShaderInfo ballShaders[] = {
		{ GL_VERTEX_SHADER, "Shaders/poolballs.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/poolballs.frag" },
		{ GL_NONE, NULL }
	};

    

    GLuint tableProgram = LoadShaders(tableShaders);
    GLuint ballProgram = LoadShaders(ballShaders);

    if (!ballProgram) {
        cout << "Failed to load ball shaders" << endl;
        return -1;
    }

    std::cout << "Table program: " << tableProgram << std::endl;
    std::cout << "Ball program: " << ballProgram << std::endl;

    init(tableProgram);
    LoadObj::Ball ball1(glm::vec3(0, 0, 0), 3, ballProgram, camera);
    ball1.Load("Ball15.obj");
    ball1.Install();
    LoadObj::Ball ball2(glm::vec3(0, 0, 0), 2, ballProgram, camera);
    ball2.Load("Ball2.obj");
    ball2.Install();

    std::cout << ball1.position.x << std::endl;
    
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static const GLfloat black[] =
        {
              0.0f, 0.0f, 0.0f, 0.0f
        };
        glClearBufferfv(GL_COLOR, 0, black);
        glClear(GL_DEPTH_BUFFER_BIT);
        table.drawTable(tableProgram, camera);
        ball1.Render(glm::vec3(1, 5, 0), glm::vec3(0, 0, 0));
        ball1.Render(glm::vec3(5, 5, 0), glm::vec3(0, 0, 0));

        

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

//void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
//    table.Zoom(yoffset);
//}

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
        double dx = xpos - lastMouseX;
        lastMouseX = xpos;

        table.Rotation(dx);
    }
}

void init(GLuint tableProgram) {

    table.init(tableProgram);
    //LoadObj::Ball ball;
    //ball.Load("Balls/ball2.obj");
    //ball.Install();
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}


