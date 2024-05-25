#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/quaternion.hpp>
#include "LoadShaders.h"
#include <vector>
#include <string>

class Table
{
public:
	Table();
	~Table();
	void init();
	void drawTable();
	void Rotation(double dx);
	void Zoom(double yoffset);

private:
	GLuint VAO;
	GLuint VBO_vertices;
	GLuint VBO_normals;
	const GLuint NumVertices = 6 * 2 * 3; // 6 faces, 2 triangulos por face, 3 vertices por triangulo
	//GLfloat vertices[numVertices * (3 + 3)];
	GLuint shaderProgram;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat3 normalMatrix;
	glm::quat currentRotation;
	GLfloat fov;
	void setupVBO();
	void setupShaders();
	void setupUniforms();



};
