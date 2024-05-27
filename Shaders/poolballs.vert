#version 440 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vTexCoords;

out vec3 vPositionEyeSpace;
out vec3 vNormalEyeSpace;
out vec2 vTexCoord;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat3 NormalMatrix;

void main()
{
    // Transformações de posição
    vPositionEyeSpace = vec3(View * Model * vec4(vPosition, 1.0));
    vNormalEyeSpace = normalize(NormalMatrix * vNormal);
    vTexCoord = vTexCoords;
    gl_Position = Projection * View * Model * vec4(vPosition, 1.0);
}
