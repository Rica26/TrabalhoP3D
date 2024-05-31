﻿#pragma comment(lib, "glew32s.lib")
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
void scrollCallback(GLFWwindow* window,double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void updateBalls(float deltaTime);

#define WIDTH 800
#define HEIGHT 600

bool mousePressed = false;
double lastMouseX, lastMouseY;
Table table;
glm::mat4 rotationMatrix = glm::mat4(1.0f);
LoadObj::Ball* animatedBall = nullptr;
//std::vector<glm::vec3> initialPositions = LoadObj::Ball::getInitialBallPositions();
std::vector<LoadObj::Ball> balls;   
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


    glfwSetScrollCallback(window, scrollCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetKeyCallback(window, keyCallback);

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
    //rotationMatrix = table.GetRotationMatrix();
    std::vector<glm::vec3> ballPositions = {
        glm::vec3(0.0f, 2.0f, 7.0f),   // Bola de partida (mais distante)

        // Formação triangular
        glm::vec3(-1.0f, 2.0f, -2.0f),  
        glm::vec3(1.0f, 2.0f, -2.0f),   

        glm::vec3(-2.0f, 2.0f, -4.0f),  
        glm::vec3(0.0f, 2.0f, -4.0f),   
        glm::vec3(2.0f, 2.0f, -4.0f),   

        glm::vec3(-3.0f, 2.0f, -6.0f),  
        glm::vec3(-1.0f, 2.0f, -6.0f),  
        glm::vec3(1.0f, 2.0f, -6.0f),   
        glm::vec3(3.0f, 2.0f, -6.0f),   

        glm::vec3(-4.0f, 2.0f, -8.0f),  
        glm::vec3(-2.0f, 2.0f, -8.0f),  
        glm::vec3(0.0f, 2.0f, -8.0f),   
        glm::vec3(2.0f, 2.0f, -8.0f),   
        glm::vec3(4.0f, 2.0f, -8.0f)    
    };
    for (int i = 0; i < ballPositions.size(); ++i) {
        LoadObj::Ball ball(ballPositions[i], ballProgram, camera);
        ball.Load("Ball" + std::to_string(i + 1) + ".obj");
        ball.Install();
        balls.push_back(ball);
    }
   // LoadObj

    //std::cout << ball1.position.x << std::endl;
    
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    animatedBall = &balls[0];
    float lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        static const GLfloat black[] =
        {
              0.0f, 0.0f, 0.0f, 0.0f
        };
        glClearBufferfv(GL_COLOR, 0, black);
        glClear(GL_DEPTH_BUFFER_BIT);

        rotationMatrix = table.GetRotationMatrix();

        
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Atualizar animações
        

        table.drawTable(tableProgram, camera);
        

        //table.drawTable(tableProgram, camera);

        for (LoadObj::Ball& ball : balls) {
            ball.UpdateRotationMatrix(rotationMatrix);
            //ball.Update(deltaTime, balls);
            ball.Render(ball.position, ball.orientation); // Renderize as bolas com a rotação aplicada
        }
        updateBalls(deltaTime);
        

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

void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ZOOM(yoffset);
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
        double dx = xpos - lastMouseX;
        lastMouseX = xpos;

        table.Rotation(dx);
        /*for (auto& ball : balls) {
            ball.Rotation(dx);
        }*/
    }
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_SPACE && animatedBall != nullptr) {
            animatedBall->StartAnimation();
        }
        if (key == GLFW_KEY_1) {
        
            for (LoadObj::Ball& ball : balls) {
                ball.activeAmbientLight = !ball.activeAmbientLight;
            }
            table.activeAmbientLight = !table.activeAmbientLight;
            
        }
        if (key == GLFW_KEY_2) {
            for (LoadObj::Ball& ball : balls) {
                ball.activeDirectionalLight = !ball.activeDirectionalLight;
            }
            table.activeDirectionalLight = !table.activeDirectionalLight;
        }
        if (key == GLFW_KEY_3) {
            for (LoadObj::Ball& ball : balls) {
                ball.activePointLight= !ball.activePointLight;
            }
            table.activePointLight = !table.activePointLight;
        }
      /*  if (key == GLFW_KEY_4) {
            for (LoadObj::Ball& ball : balls) {
                if (ball.activeAmbientLight)
                    ball.activeAmbientLight = false;
                else
                    ball.activeAmbientLight = true;
            }
        }*/
    }
}

void updateBalls(float deltaTime) {
    for (auto& ball : balls) {
        ball.Update(deltaTime, balls);
    }
}
void init(GLuint tableProgram) {

    table.init(tableProgram);
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
}


