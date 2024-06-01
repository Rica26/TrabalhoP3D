#include "Table.h"


Table::Table() : VAO(0), VBO_vertices(0), VBO_normals(0), model(glm::mat4(1.0f)),
view(glm::mat4(1.0f)), projection(glm::mat4(1.0f)), normalMatrix(glm::mat3(1.0f)),
currentRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {
}

Table::~Table() {
    // Destructor
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_vertices);
    glDeleteBuffers(1, &VBO_normals);
}



void Table::installLoadTable(GLuint shaderProgram) 
{
    // Configurar valores para ser mais f�cil "manobrar" o tamanho da mesa
    GLfloat width = 2.0f;
    GLfloat height = 0.2f;
    GLfloat depth = 4.0f;

    // Definir os v�rtices da mesa
    GLfloat vertices[] = {
        // Face X+
        width / 2, -height / 2,  depth / 2,
        width / 2, -height / 2, -depth / 2,
        width / 2,  height / 2,  depth / 2,
        width / 2,  height / 2,  depth / 2,
        width / 2, -height / 2, -depth / 2,
        width / 2,  height / 2, -depth / 2,

        // Face X-
        -width / 2, -height / 2, -depth / 2,
        -width / 2, -height / 2,  depth / 2,
        -width / 2,  height / 2, -depth / 2,
        -width / 2,  height / 2, -depth / 2,
        -width / 2, -height / 2,  depth / 2,
        -width / 2,  height / 2,  depth / 2,

        // Face Y+
        -width / 2,  height / 2,  depth / 2,
        width / 2,  height / 2,  depth / 2,
        -width / 2,  height / 2, -depth / 2,
        -width / 2,  height / 2, -depth / 2,
        width / 2,  height / 2,  depth / 2,
        width / 2,  height / 2, -depth / 2,

        // Face Y-
        -width / 2, -height / 2, -depth / 2,
        width / 2, -height / 2, -depth / 2,
        -width / 2, -height / 2,  depth / 2,
        -width / 2, -height / 2,  depth / 2,
        width / 2, -height / 2, -depth / 2,
        width / 2, -height / 2,  depth / 2,

        // Face Z+
        -width / 2, -height / 2,  depth / 2,
        width / 2, -height / 2,  depth / 2,
        -width / 2,  height / 2,  depth / 2,
        -width / 2,  height / 2,  depth / 2,
        width / 2, -height / 2,  depth / 2,
        width / 2,  height / 2,  depth / 2,

        // Face Z-
        width / 2, -height / 2, -depth / 2,
        -width / 2, -height / 2, -depth / 2,
        width / 2,  height / 2, -depth / 2,
        width / 2,  height / 2, -depth / 2,
        -width / 2, -height / 2, -depth / 2,
        -width / 2,  height / 2, -depth / 2
    };

    //Definir as normais da mesa
    GLfloat normals[] = {
        // Face X+
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 0.0f,

        // Face X-
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,
        -1.0f, 0.0f, 0.0f,

        // Face Y+
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,

        // Face Y-
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,
        0.0f, -1.0f, 0.0f,

        // Face Z+
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f,

        // Face Z-
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f,
        0.0f, 0.0f, -1.0f
    };

    //Gera��o e vincula��o do VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Gera��o e vincula��o do VBO dos v�rtices
    glGenBuffers(1, &VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
    //Carrega os dados dos v�rtices para o VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //Gera��o e vincula��o do VBO das normais
    glGenBuffers(1, &VBO_normals);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    //Carrega os dados das normais para o VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    //Configura��o dos atributos dos v�rtices
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
    //Recupera o identificador do atributo de entrada do shader
    GLint vertexPosition = glGetProgramResourceLocation(shaderProgram, GL_PROGRAM_INPUT, "vPosition");
    //Associa o VBO ao atributo de entrada do shader
    //3 componentes (x, y, z), do tipo float, n�o normalizado
    //espa�amento entre dados � 3 * sizeof(GLfloat), pois cada v�rtice possui 3 floats (x, y, z) consecutivos
    glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    //Habilita o atributo de entrada do shader(posi��o do vertice)
    glEnableVertexAttribArray(vertexPosition);
    
    //Configura��o dos atributos das normais
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    //Recupera o identificador do atributo de entrada do shader
    GLint vertexNormal = glGetProgramResourceLocation(shaderProgram, GL_PROGRAM_INPUT, "vNormal");
    //Associa o VBO ao atributo de entrada do shader
    //3 componentes (x, y, z), do tipo float, n�o normalizado
    //espa�amento entre dados � 3 * sizeof(GLfloat), pois cada normal possui 3 floats (x, y, z) consecutivos
    glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    //Habilita o atributo de entrada do shader(normal do vertice)
    glEnableVertexAttribArray(vertexNormal);

    glBindVertexArray(0);

}


void Table::renderTable(GLuint shaderProgram, const Camera& camera) {
    

    // Vincula o VAO
    glBindVertexArray(VAO);

    // Cria a matriz de proje��o e a matriz de vis�o atrav�s de valores da camera
    projection = camera.getProjectionMatrix(4.0f / 3.0f);
    view = camera.getViewMatrix();

    // Usa o programa de shader
    glUseProgram(shaderProgram);

    // Recupera e define as matrizes uniformes do shader
    GLint projectionId = glGetUniformLocation(shaderProgram, "Projection");
    GLint viewId = glGetUniformLocation(shaderProgram, "View");
    GLint modelViewId = glGetUniformLocation(shaderProgram, "ModelView");
    GLint normalMatrixId = glGetUniformLocation(shaderProgram, "NormalMatrix");
    GLint modelId = glGetUniformLocation(shaderProgram, "Model");

    //Cria a matriz de rota��o a partir do quaternion currentRotation
    //e transforma a currentRotation numa matriz 4x4
    glm::mat4 RotationMatrix = glm::mat4_cast(currentRotation);

    //Cria a matriz modelview para transformar os v�rtices do espa�o do objeto
    //para o espa�o da camera aplicando a RotationMatrix tamb�m para 
    //assegurar que a mesa � corretamente posicionada e rotacionada no espa�o de camera
    //facilitando o c�lculo da ilumina��o tambem
    glm::mat4 modelView = view * RotationMatrix * model;
    //Calcula a matriz normal 
    //(usada para transforma��es normais que s�o cruciais para o c�lculo correto da ilumina��o,
    //porque as normais definem como a superficie reflete a luz) atrav�s da inversa da transposta da matriz modelview
    normalMatrix = glm::inverseTranspose(glm::mat3(modelView));

    //Define os valores dos uniformes no shader
    glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(modelViewId, 1, GL_FALSE, glm::value_ptr(modelView));
    glUniformMatrix3fv(normalMatrixId, 1, GL_FALSE, glm::value_ptr(normalMatrix));
    glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(model));

    //Define os valores da luz ambiente dependendo se est� "apagada" ou n�o
    if (activeAmbientLight)
        glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0.5)));
    else
        glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));

    //Define os valores da luz direcional dependendo se est� "apagada" ou n�o
    if (activeDirectionalLight) {
        glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
        glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(0.2, 0.2, 0.2)));
        glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
        glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
    }
    else {
        glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.direction"), 1, glm::value_ptr(glm::vec3(1.0, 0.0, 0.0)));
        glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.ambient"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
        glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.diffuse"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
        glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "directionalLight.specular"), 1, glm::value_ptr(glm::vec3(0.0, 0.0, 0.0)));
    }

    //Define os valores da luz pontual dependendo se est� "apagada" ou n�o
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

    //Define os valores dos materiais da mesa
    glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.emissive"), 1, glm::value_ptr(glm::vec3(0.0, 0.5, 0.0)));
    glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
    glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
    glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
    glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "material.shininess"), 12.0f);

    //Desenha a mesa usando a primitiva GL_TRIANGLES come�ando no indice 0 e desenhando todos os vertices
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);

    glBindVertexArray(0); // Desvincula o VAO para evitar altera��es acidentais
}


void Table::Rotation(double dx) {
    
    //Converte o deslocamento do rato num �ngulo em radianos, 
    //porque as fun��es da biblioteca GLM funciona com radianos
    float angleY = glm::radians((float)dx);
    //Cria um quaternion de rota��o em torno do eixo Y com o �ngulo calculado
    glm::quat qy = glm::angleAxis(angleY, glm::vec3(0.0f, 1.0f, 0.0f));
    //Atualiza a rota��o atual multiplicando o quaternion de rota��o qy
    currentRotation = qy * currentRotation;
    //Normaliza a rota��o atual para evitar a acumula��o de erros numericos
    //que podem ocorrer com multiplas rota��es
    currentRotation = glm::normalize(currentRotation);
}

const glm::mat4& Table::GetRotationMatrix() const {
    //Retorna a matriz de rota��o atual
    return glm::mat4_cast(currentRotation);
}



