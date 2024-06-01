#version 440 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 ModelView;		// View * Model
uniform mat4 Projection;
uniform mat3 NormalMatrix;

in vec3 vPosition;			// Coordenadas locais do v�rtice
in vec3 vNormal;
in vec2 vTexCoords;			// Normal do v�rtice

out vec3 vPositionEyeSpace;
out vec3 vNormalEyeSpace;
out vec2 textureVector;

void main()
{ 
	// Posi��o do v�rtice em coordenadas do olho.
	vPositionEyeSpace = (ModelView * vec4(vPosition, 1.0)).xyz;

	// Transformar a normal do v�rtice.
	vNormalEyeSpace = normalize(NormalMatrix * vNormal);

	// Coordenada de textura para o Sampler2D
	textureVector = vTexCoords;

	// Posi��o final do v�rtice (em coordenadas de clip)
	gl_Position = Projection * ModelView * vec4(vPosition, 1.0f);
}
