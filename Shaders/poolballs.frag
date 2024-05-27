#version 440 core

in vec3 vPositionEyeSpace;
in vec3 vNormalEyeSpace;
in vec2 vTexCoord;

uniform sampler2D ballSampler;

layout(location = 0) out vec4 fColor;

void main()
{
    vec3 color = texture(ballSampler, vTexCoord).rgb;
    fColor = vec4(color, 1.0);
}

