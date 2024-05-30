#version 440 core

in vec3 vPositionEyeSpace;
in vec3 vNormalEyeSpace;
in vec2 textureVector;

uniform mat4 View;
uniform sampler2D ballSampler;

// Estrutura da fonte de luz ambiente global
struct AmbientLight {
    vec3 ambient;
};
uniform AmbientLight ambientLight; 

// Estrutura de uma fonte de luz direcional
struct DirectionalLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight directionalLight;

// Estrutura de uma fonte de luz pontual
struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
};
uniform PointLight pointLight[2];

// Estrutura de uma fonte de luz cônica
struct SpotLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    float spotCutoff, spotExponent;
    vec3 spotDirection;
};
uniform SpotLight spotLight;

// Estrutura do material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

layout(location = 0) out vec4 fColor;

vec4 calcAmbientLight(AmbientLight light) {
    return vec4(material.ambient * light.ambient, 1.0);
}

vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambient) {
    ambient = vec4(material.ambient * light.ambient, 1.0);
    vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
    vec3 L = normalize(-lightDirectionEyeSpace);
    vec3 N = normalize(vNormalEyeSpace);
    float NdotL = max(dot(N, L), 0.0);
    vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
    vec3 V = normalize(-vPositionEyeSpace);
    vec3 R = reflect(-L, N);
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
    return (diffuse + specular);
}

vec4 calcPointLight(PointLight light, out vec4 ambient) {
    ambient = vec4(material.ambient * light.ambient, 1.0);
    vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
    vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
    vec3 N = normalize(vNormalEyeSpace);
    float NdotL = max(dot(N, L), 0.0);
    vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
    vec3 V = normalize(-vPositionEyeSpace);
    vec3 R = reflect(-L, N);
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
    float dist = length(mat3(View) * light.position - vPositionEyeSpace);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    return (attenuation * (diffuse + specular));
}

vec4 calcSpotLight(SpotLight light, out vec4 ambient) {
    // Adicione aqui o código necessário para calcular a contribuição da fonte de luz cônica para a cor final do fragmento.
    ambient = vec4(0.0);
    return vec4(0.0);
}

void main()
{
    vec4 ambient;
    vec4 light[4];
    vec4 ambientTmp;
    ambient = calcAmbientLight(ambientLight);
    light[0] = calcDirectionalLight(directionalLight, ambientTmp);
    ambient += ambientTmp;
    for(int i = 0; i < 2; i++) {
        light[i+1] = calcPointLight(pointLight[i], ambientTmp);
        ambient += ambientTmp;
    }
    light[3] = calcSpotLight(spotLight, ambientTmp);
    ambient += ambientTmp;

    vec3 color = texture(ballSampler, textureVector).rgb;
    vec3 finalColor = color * (ambient + light[0] + light[1] + light[2] + light[3]).rgb;

    fColor = vec4(finalColor, 1.0);
}