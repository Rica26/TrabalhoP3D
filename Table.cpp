#include "Table.h"


Table::Table() : fov(45.0f), currentRotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)) {
    // Constructor
}

Table::~Table() {
    // Destructor
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_vertices);
    glDeleteBuffers(1, &VBO_normals);
    glDeleteProgram(shaderProgram);
}

void Table::init() {
    setupShaders();
    setupVBO();
    setupUniforms();
}

void Table::setupShaders() {
    ShaderInfo tableShaders[] = {
		{ GL_VERTEX_SHADER, "Shaders/light.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/light.frag" },
		{ GL_NONE, NULL }
	};

	shaderProgram = LoadShaders(tableShaders);
}

void Table::setupVBO() 
{
    GLfloat width = 2.0f;
    GLfloat height = 0.2f;
    GLfloat depth = 4.0f;

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

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glGenBuffers(1, &VBO_normals);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertices);
    GLint vertexPosition = glGetProgramResourceLocation(shaderProgram, GL_PROGRAM_INPUT, "vPosition");
    glVertexAttribPointer(vertexPosition, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(vertexPosition);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    GLint vertexNormal = glGetProgramResourceLocation(shaderProgram, GL_PROGRAM_INPUT, "vNormal");
    glVertexAttribPointer(vertexNormal, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(vertexNormal);

}

void Table::setupUniforms() {
    // Configurar as matrizes de transformação
    projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    view = glm::lookAt(
        glm::vec3(2.0f, 3.0f, 5.0f),    // Posição da câmera
        glm::vec3(0.0f, 0.0f, 0.0f),    // Ponto para onde a câmera está olhando
        glm::vec3(0.0f, 1.0f, 0.0f)     // Vetor "up"
    );
    model = glm::mat4(1.0f);

    glm::mat4 modelView = view * model;
    normalMatrix = glm::inverseTranspose(glm::mat3(modelView));

    glUseProgram(shaderProgram);

    GLint projectionId = glGetUniformLocation(shaderProgram, "Projection");
    glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));

    GLint viewId = glGetUniformLocation(shaderProgram, "View");
    glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));

    GLint modelViewId = glGetUniformLocation(shaderProgram, "ModelView");
    glUniformMatrix4fv(modelViewId, 1, GL_FALSE, glm::value_ptr(modelView));

    GLint normalMatrixId = glGetUniformLocation(shaderProgram, "NormalMatrix");
    glUniformMatrix3fv(normalMatrixId, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "ambientLight.ambient"), 1, glm::value_ptr(glm::vec3(0.1, 0.1, 0.1)));
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

    glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.emissive"), 1, glm::value_ptr(glm::vec3(0.0, 0.5, 0.0)));
    glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.ambient"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
    glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.diffuse"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
    glProgramUniform3fv(shaderProgram, glGetUniformLocation(shaderProgram, "material.specular"), 1, glm::value_ptr(glm::vec3(1.0, 1.0, 1.0)));
    glProgramUniform1f(shaderProgram, glGetUniformLocation(shaderProgram, "material.shininess"), 12.0f);
}

void Table::drawTable() {
    glUseProgram(shaderProgram);
    glm::mat4 RotationMatrix = glm::mat4_cast(currentRotation);
    glm::mat4 ModelView = view * RotationMatrix * model;
    normalMatrix = glm::inverseTranspose(glm::mat3(ModelView));
    projection = glm::perspective(glm::radians(fov), 4.0f / 3.0f, 0.1f, 100.0f);

    GLint projectionId = glGetUniformLocation(shaderProgram, "Projection");
    glUniformMatrix4fv(projectionId, 1, GL_FALSE, glm::value_ptr(projection));
    GLint modelId = glGetUniformLocation(shaderProgram, "Model");
    glUniformMatrix4fv(modelId, 1, GL_FALSE, glm::value_ptr(RotationMatrix * model));
    GLint viewId = glGetUniformLocation(shaderProgram, "View");
    glUniformMatrix4fv(viewId, 1, GL_FALSE, glm::value_ptr(view));
    GLint modelViewId = glGetUniformLocation(shaderProgram, "ModelView");
    glUniformMatrix4fv(modelViewId, 1, GL_FALSE, glm::value_ptr(ModelView));
    GLint normalMatrixId = glGetUniformLocation(shaderProgram, "NormalMatrix");
    glUniformMatrix3fv(normalMatrixId, 1, GL_FALSE, glm::value_ptr(normalMatrix));

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}

void Table::Rotation(double dx) {
    float angleY = glm::radians((float)dx);
    glm::quat qy = glm::angleAxis(angleY, glm::vec3(0.0f, 1.0f, 0.0f));
    currentRotation = qy * currentRotation;
    currentRotation = glm::normalize(currentRotation);
}

void Table::Zoom(double yoffset) {
    if (yoffset == -1) {
        fov += fabs(fov) * 0.1f;
    }
    else if (yoffset == 1) {
        fov -= fabs(fov) * 0.1f;
    }
}


