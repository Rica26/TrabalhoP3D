#version 440 core

in vec3 vPositionEyeSpace; //Posição do fragmento no espaço de câmera
in vec3 vNormalEyeSpace; //Normal do fragmento no espaço de câmera
in vec2 textureVector; //Coordenadas de textura do fragmento

uniform mat4 View; //Matriz de visualização
uniform sampler2D ballSampler; //Sampler para a textura da bola

// Estrutura da fonte de luz ambiente global
struct AmbientLight {
    vec3 ambient;
};
uniform AmbientLight ambientLight; //Fonte de luz ambiente global

// Estrutura de uma fonte de luz direcional
struct DirectionalLight {
    vec3 direction; //Direção da fonte de luz
    vec3 ambient; //Componente de luz ambiente
    vec3 diffuse; //Componente de luz difusa
    vec3 specular; //Componente de luz especular
};
uniform DirectionalLight directionalLight; //Fonte de luz direcional

// Estrutura de uma fonte de luz pontual
struct PointLight {
    vec3 position; //Posição da fonte de luz
    vec3 ambient;  //Componente de luz ambiente
    vec3 diffuse;  //Componente de luz difusa
    vec3 specular; //Componente de luz especular
    float constant; //Coeficiente de atenuação constante
    float linear;  //Coeficiente de atenuação linear
    float quadratic; //Coeficiente de atenuação quadrática
};
uniform PointLight pointLight[2]; //Duas fontes de luz pontual

// Estrutura de uma fonte de luz cônica
struct SpotLight {
    vec3 position; //Posição da fonte de luz
    vec3 ambient; //Componente de luz ambiente
    vec3 diffuse; //Componente de luz difusa
    vec3 specular; //Componente de luz especular
    float constant; //Coeficiente de atenuação constante
    float linear;  //Coeficiente de atenuação linear
    float quadratic;  //Coeficiente de atenuação quadrática
    float spotCutoff, spotExponent; //Angulo de corte e expoente da luz cônica
    vec3 spotDirection; //Direção da luz cônica
};
uniform SpotLight spotLight; //Fonte de luz cônica

// Estrutura do material
struct Material {
    vec3 ambient; //Coeficiente de reflexão ambiente
    vec3 diffuse; //Coeficiente de reflexão difusa
    vec3 specular; //Coeficiente de reflexão especular
    float shininess; //Componente especular
};
uniform Material material; //Material da superfície

layout(location = 0) out vec4 fColor; //Cor final do fragmento

vec4 calcAmbientLight(AmbientLight light) {
    // calcula a contribuição da luz ambiente para a cor do objeto
    // multiplicando o coeficiente de reflexão ambiente do material (Ka)
    // pela componente de luz ambiente da fonte de luz (Ia)
    return vec4(material.ambient * light.ambient, 1.0);
}

vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambient) {
    //calculo da contribuição da luz ambiente
    //que depois é retornada em cima separadamente
    //para acumular as contribuiçoes
    //da luz ambiente de todas as fontes de luz
    
    ambient = vec4(material.ambient * light.ambient, 1.0);
    
    //Transforma a direção da luz usando a matriz View para o espaço da câmera
    //porque as direções das luzes e das normais têm que estar no mesmo espaço de coordenadas
    
    vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz; 
    
    //Inverte a direção da luz para obter um vetor que aponta da superfície para a fonte de luz
    //para calcular corretamente como a luz atinge e interage com a superfície
    
    vec3 L = normalize(-lightDirectionEyeSpace);
    
    //Normaliza a normal do fragmento
    
    vec3 N = normalize(vNormalEyeSpace);
    
    //Produto escalar entre a normal e a direção da luz
    //determina o cosseno do ângulo entre N e L para calcular a intensidade da luz difusa
    //e aplica clamp para não ter valores negativos e não contribuir se estiver atrás da superfície
    float NdotL = max(dot(N, L), 0.0);
    
    //Cálculo da contribuição da luz difusa
    //multiplicando o coeficiente de reflexão difusa do material (Kd)
    //pela componente de luz difusa da fonte de luz (Id) e pelo cosseno do ângulo entre a normal e a direção da luz
    
    vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
    
    //Calcula a direção do vetor de visão
    vec3 V = normalize(-vPositionEyeSpace);
    //Reflete a direção da luz em relação à normal
    vec3 R = reflect(-L, N);
    //Produto escalar entre a direção refletida e a direção do vetor de visão
    //Calcula a intensidade da luz especular
    float RdotV = max(dot(R, V), 0.0);
    //Cálculo da contribuição da luz especular elevando o produto escalar ao brilho do material
    //e multiplicando pelo coeficiente de reflexão especular do material (Ks) e pela componente de luz especular da fonte de luz (Is)
    vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
    
    //Retorna a soma da contribuição da luz difusa e da luz especular
    return (diffuse + specular);
}

vec4 calcPointLight(PointLight light, out vec4 ambient) {
    //calculo da contribuição da luz ambiente
    //que depois é retornada em cima separadamente
    //para acumular as contribuiçoes
    //da luz ambiente de todas as fontes de luz
    ambient = vec4(material.ambient * light.ambient, 1.0);
    //Transforma a posição da luz usando a matriz View para o espaço da câmera
    vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
    
    // Calcula a direção da luz (L) do fragmento até a fonte de luz
    // Subtrai a posição do fragmento no espaço da câmera da posição da luz no espaço da câmera
    // Isso nos dá o vetor que aponta do fragmento para a luz
    // Em seguida, normaliza o vetor resultante para obter um vetor de direção unitário
    vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
    //Normaliza a normal do fragmento
    vec3 N = normalize(vNormalEyeSpace);
    
    //Produto escalar entre a normal e a direção da luz
    //determina o cosseno do ângulo entre N e L para calcular a intensidade da luz difusa
    //e aplica clamp para não ter valores negativos e não contribuir se estiver atrás da superfície
    float NdotL = max(dot(N, L), 0.0);
    
    //Calcula a contribuição da luz difusa
    //Multiplica o coeficiente de reflexão difusa do material (Kd)
    //pela componente de luz difusa da fonte de luz (Id) e pelo cosseno do ângulo entre a normal e a direção da luz
    vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
    //Calcula a direção do vetor de visão
    vec3 V = normalize(-vPositionEyeSpace);
    //Reflete a direção da luz em relação à normal
    vec3 R = reflect(-L, N);
    //Produto escalar entre a direção refletida e a direção do vetor de visão
    //Calcula a intensidade da luz especular
    float RdotV = max(dot(R, V), 0.0);
    //Cálculo da contribuição da luz especular elevando o produto escalar ao brilho do material
    //e multiplicando pelo coeficiente de reflexão especular do material (Ks) e pela componente de luz especular da fonte de luz (Is)
    vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

    //Calcula a distância entre o ponto de luz e o fragmento no espaço da camera
    float dist = length(mat3(View) * light.position - vPositionEyeSpace);
    //Calcula a atenuação da luz com a distância
    //Utiliza uma fórmula de atenuação que inclui termos constante, linear e quadrático
    //light.constant: atenuação constante
    //light.linear: atenuação proporcional à distância
    //light.quadratic: atenuação proporcional ao quadrado da distância
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    //Retorna a contribuição total da luz pontual considerando a atenuação.
    return (attenuation * (diffuse + specular));
}

vec4 calcSpotLight(SpotLight light, out vec4 ambient) {
    // Adicione aqui o código necessário para calcular a contribuição da fonte de luz cônica para a cor final do fragmento.
    ambient = vec4(0.0);
    return vec4(0.0);
}

void main()
{
    vec4 ambient; //Variável para a luz ambiente
    vec4 light[4]; //Array para armazenar as contribuições das diferentes fontes de luz
    vec4 ambientTmp; //Variável temporária para a luz ambiente
    ambient = calcAmbientLight(ambientLight); //Calcula a luz ambiente global
    light[0] = calcDirectionalLight(directionalLight, ambientTmp); //Calcula a luz direcional adicionando a contribuição da luz ambiente à variável temporária
    ambient += ambientTmp;

    //Calcula a contribuição das duas fontes de luz pontual adicionando a contribuição da luz ambiente à variável temporária
    for(int i = 0; i < 2; i++) {
        light[i+1] = calcPointLight(pointLight[i], ambientTmp);
        ambient += ambientTmp;
    }
    light[3] = calcSpotLight(spotLight, ambientTmp); //Calcula a luz cônica adicionando a contribuição da luz ambiente à variável temporária
    ambient += ambientTmp;

    vec3 color = texture(ballSampler, textureVector).rgb; //Obtem a cor da textura aplicada ao fragmento
    vec3 finalColor = color * ((ambient/4) + light[0] + light[1] + light[2] + light[3]).rgb; //Calcula a cor final do fragmento

    //Define a cor final do fragmento, com o componente alpha igual a 1.0 (totalmente opaco)
    fColor = vec4(finalColor, 1.0);
}