#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/quaternion.hpp>
#include "LoadShaders.h"
#include "Camera.h"
#include <vector>
#include <string>

class Table
{
public:
	Table();
	~Table();
	void installLoadTable(GLuint shaderProgram);
	void renderTable(GLuint shaderProgram, const Camera& camera);
	void Rotation(double dx);
	glm::quat currentRotation;
	const glm::mat4&GetRotationMatrix() const;
	bool activeAmbientLight = true;
	bool activeDirectionalLight = true;
	bool activePointLight = true;
	bool activeSpotLight = true;

private:
	GLuint VAO;
	GLuint VBO_vertices;
	GLuint VBO_normals;
	const GLuint NumVertices = 6 * 2 * 3;
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 projection;
	glm::mat3 normalMatrix;



};
