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

#include "Table.h"
#include "LoadObj.h"
#include "Camera.h"

#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\matrix_inverse.hpp> // glm::inverseTranspose()
#include <glm\gtc\quaternion.hpp>

void print_error(int error, const char* description);
void print_gl_info(void);
void scrollCallback(GLFWwindow* window,double xoffset, double yoffset);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

#define WIDTH 800 
#define HEIGHT 600

bool mousePressed = false;
double lastMouseX, lastMouseY; 
Table table;
glm::mat4 rotationMatrix = glm::mat4(1.0f);
LoadObj::Ball* animatedBall = nullptr;
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

    //Define os shaders para a mesa
    ShaderInfo tableShaders[] = {
            
        { GL_VERTEX_SHADER, "Shaders/light.vert" },
        { GL_FRAGMENT_SHADER, "Shaders/light.frag" },
        { GL_NONE, NULL }
    };

    //Define os shaders para as bolas
    ShaderInfo ballShaders[] = {
		{ GL_VERTEX_SHADER, "Shaders/poolballs.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/poolballs.frag" },
		{ GL_NONE, NULL }
	};

    

    GLuint tableProgram = LoadShaders(tableShaders); //Carrega os shaders da mesa para a variável tableProgram
    GLuint ballProgram = LoadShaders(ballShaders); //Carrega os shaders das bolas para a variável ballProgram

    if (!ballProgram) {
        cout << "Failed to load ball shaders" << endl;
        return -1;
    }

    table.installLoadTable(tableProgram); //Carrega a mesa com o programa tableProgram
    //Inicializa um vetor de posições para as bolas
    std::vector<glm::vec3> ballPositions = {
        glm::vec3(0.0f, 2.0f, 7.0f),   


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

    //Inicializa as bolas adicionando-as ao vetor balls
    for (int i = 0; i < ballPositions.size(); ++i) {
        LoadObj::Ball ball(ballPositions[i], ballProgram, camera);
        ball.Load("Ball" + std::to_string(i + 1) + ".obj");
        ball.Install();
        balls.push_back(ball);
    }

    
    glViewport(0, 0, WIDTH, HEIGHT); //Define a área de renderização
    glEnable(GL_DEPTH_TEST); //Ativa o teste de profundidade
    glEnable(GL_CULL_FACE); //Ativa o culling para melhorar o desempenho removendo faces que não são visíveis
    glCullFace(GL_BACK); //Define que apenas as faces de trás serão removidas

    animatedBall = &balls[0]; //Define a bola que será animada
    float lastTime = glfwGetTime(); //Tempo da última iteração
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //Limpa o buffer de cor e o buffer de profundidade no inicio de cada iteracao

        //Define a cor de fundo
        static const GLfloat black[] =
        {
              0.0f, 0.0f, 0.0f, 0.0f
        };

        //Atualiza a matriz de rotação usando a matriz de rotação da mesa
        rotationMatrix = table.GetRotationMatrix();

        //Calcula o tempo decorrido desde a última iteração
        float currentTime = glfwGetTime();
        float deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        
        

        table.renderTable(tableProgram, camera); //Renderiza a mesa usando o programa tableProgram e a camera
        

        //Renderiza as bolas, atualizando a matriz de rotação e a posição da bola
        for (LoadObj::Ball& ball : balls) {
            ball.UpdateRotationMatrix(rotationMatrix); //Atualiza a matriz de rotação da bola para seguir a rotação da mesa
            ball.Update(deltaTime, balls); //Verifica se houve colisão e atualiza a posição e orientação da bola animada
            ball.Render(ball.position, ball.orientation); //Renderiza a bola
        }
        

        glfwSwapBuffers(window); //Troca os buffers de cor
        glfwPollEvents(); 
    }

    glfwDestroyWindow(window); //Destroi a janela
    glfwTerminate(); //Finaliza o GLFW
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

//Usa o scroll do rato para dar zoom mudando o fov da camera
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
  camera.ZOOM(yoffset);
}

//Usa o botão esquerdo do rato para rodar a mesa
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            mousePressed = true; //Define que o botão do rato está pressionado
            glfwGetCursorPos(window, &lastMouseX, &lastMouseY); //Obtém a posição do rato
        }
        else if (action == GLFW_RELEASE) {
            mousePressed = false; //Define que o botão do rato não está pressionado
        }
    }
}

//Usa o movimento do rato para rodar a mesa
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (mousePressed) {
        double dx = xpos - lastMouseX; //Calcula a diferença entre a posição atual e a última posição do rato
        lastMouseX = xpos; //Atualiza a última posição do rato

        table.Rotation(dx); //Roda a mesa usando a função Rotation da mesa e com base na diferença calculada
    }
}

//Usa as teclas para começar a animação, ativar e desativar as luzes
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        //Se a tecla pressionada for a barra de espaço, começa a animação
        if (key == GLFW_KEY_SPACE && animatedBall != nullptr) {
            animatedBall->StartAnimation(); //Começa a animação da bola animada
        }
        //Se a tecla pressionada for 1 ativa/desativa a luz ambiente
        if (key == GLFW_KEY_1) {
        
            for (LoadObj::Ball& ball : balls) {
                ball.activeAmbientLight = !ball.activeAmbientLight;
            }
            table.activeAmbientLight = !table.activeAmbientLight;
            
        }
        //Se a tecla pressionada for 2 ativa/desativa a luz direcional
        if (key == GLFW_KEY_2) {
            for (LoadObj::Ball& ball : balls) {
                ball.activeDirectionalLight = !ball.activeDirectionalLight;
            }
            table.activeDirectionalLight = !table.activeDirectionalLight;
        }
        //Se a tecla pressionada for 3 ativa/desativa as luzes pontuais
        if (key == GLFW_KEY_3) {
            for (LoadObj::Ball& ball : balls) {
                ball.activePointLight= !ball.activePointLight;
            }
            table.activePointLight = !table.activePointLight;
        }
    }
}


