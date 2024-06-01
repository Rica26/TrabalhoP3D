#version 440 core

in vec3 vPositionEyeSpace; //Posi��o do fragmento no espa�o de c�mera
in vec3 vNormalEyeSpace; //Normal do fragmento no espa�o de c�mera
in vec2 textureVector; //Coordenadas de textura do fragmento

uniform mat4 View; //Matriz de visualiza��o
uniform sampler2D ballSampler; //Sampler para a textura da bola

// Estrutura da fonte de luz ambiente global
struct AmbientLight {
    vec3 ambient;
};
uniform AmbientLight ambientLight; //Fonte de luz ambiente global

// Estrutura de uma fonte de luz direcional
struct DirectionalLight {
    vec3 direction; //Dire��o da fonte de luz
    vec3 ambient; //Componente de luz ambiente
    vec3 diffuse; //Componente de luz difusa
    vec3 specular; //Componente de luz especular
};
uniform DirectionalLight directionalLight; //Fonte de luz direcional

// Estrutura de uma fonte de luz pontual
struct PointLight {
    vec3 position; //Posi��o da fonte de luz
    vec3 ambient;  //Componente de luz ambiente
    vec3 diffuse;  //Componente de luz difusa
    vec3 specular; //Componente de luz especular
    float constant; //Coeficiente de atenua��o constante
    float linear;  //Coeficiente de atenua��o linear
    float quadratic; //Coeficiente de atenua��o quadr�tica
};
uniform PointLight pointLight[2]; //Duas fontes de luz pontual

// Estrutura de uma fonte de luz c�nica
struct SpotLight {
    vec3 position; //Posi��o da fonte de luz
    vec3 ambient; //Componente de luz ambiente
    vec3 diffuse; //Componente de luz difusa
    vec3 specular; //Componente de luz especular
    float constant; //Coeficiente de atenua��o constante
    float linear;  //Coeficiente de atenua��o linear
    float quadratic;  //Coeficiente de atenua��o quadr�tica
    float spotCutoff, spotExponent; //Angulo de corte e expoente da luz c�nica
    vec3 spotDirection; //Dire��o da luz c�nica
};
uniform SpotLight spotLight; //Fonte de luz c�nica

// Estrutura do material
struct Material {
    vec3 ambient; //Coeficiente de reflex�o ambiente
    vec3 diffuse; //Coeficiente de reflex�o difusa
    vec3 specular; //Coeficiente de reflex�o especular
    float shininess; //Componente especular
};
uniform Material material; //Material da superf�cie

layout(location = 0) out vec4 fColor; //Cor final do fragmento

vec4 calcAmbientLight(AmbientLight light) {
    // calcula a contribui��o da luz ambiente para a cor do objeto
    // multiplicando o coeficiente de reflex�o ambiente do material (Ka)
    // pela componente de luz ambiente da fonte de luz (Ia)
    return vec4(material.ambient * light.ambient, 1.0);
}

vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambient) {
    //calculo da contribui��o da luz ambiente
    //que depois � retornada em cima separadamente
    //para acumular as contribui�oes
    //da luz ambiente de todas as fontes de luz
    
    ambient = vec4(material.ambient * light.ambient, 1.0);
    
    //Transforma a dire��o da luz usando a matriz View para o espa�o da c�mera
    //porque as dire��es das luzes e das normais t�m que estar no mesmo espa�o de coordenadas
    
    vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz; 
    
    //Inverte a dire��o da luz para obter um vetor que aponta da superf�cie para a fonte de luz
    //para calcular corretamente como a luz atinge e interage com a superf�cie
    
    vec3 L = normalize(-lightDirectionEyeSpace);
    
    //Normaliza a normal do fragmento
    
    vec3 N = normalize(vNormalEyeSpace);
    
    //Produto escalar entre a normal e a dire��o da luz
    //determina o cosseno do �ngulo entre N e L para calcular a intensidade da luz difusa
    //e aplica clamp para n�o ter valores negativos e n�o contribuir se estiver atr�s da superf�cie
    float NdotL = max(dot(N, L), 0.0);
    
    //C�lculo da contribui��o da luz difusa
    //multiplicando o coeficiente de reflex�o difusa do material (Kd)
    //pela componente de luz difusa da fonte de luz (Id) e pelo cosseno do �ngulo entre a normal e a dire��o da luz
    
    vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
    
    //Calcula a dire��o do vetor de vis�o
    vec3 V = normalize(-vPositionEyeSpace);
    //Reflete a dire��o da luz em rela��o � normal
    vec3 R = reflect(-L, N);
    //Produto escalar entre a dire��o refletida e a dire��o do vetor de vis�o
    //Calcula a intensidade da luz especular
    float RdotV = max(dot(R, V), 0.0);
    //C�lculo da contribui��o da luz especular elevando o produto escalar ao brilho do material
    //e multiplicando pelo coeficiente de reflex�o especular do material (Ks) e pela componente de luz especular da fonte de luz (Is)
    vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
    
    //Retorna a soma da contribui��o da luz difusa e da luz especular
    return (diffuse + specular);
}

vec4 calcPointLight(PointLight light, out vec4 ambient) {
    //calculo da contribui��o da luz ambiente
    //que depois � retornada em cima separadamente
    //para acumular as contribui�oes
    //da luz ambiente de todas as fontes de luz
    ambient = vec4(material.ambient * light.ambient, 1.0);
    //Transforma a posi��o da luz usando a matriz View para o espa�o da c�mera
    vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
    
    // Calcula a dire��o da luz (L) do fragmento at� a fonte de luz
    // Subtrai a posi��o do fragmento no espa�o da c�mera da posi��o da luz no espa�o da c�mera
    // Isso nos d� o vetor que aponta do fragmento para a luz
    // Em seguida, normaliza o vetor resultante para obter um vetor de dire��o unit�rio
    vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
    //Normaliza a normal do fragmento
    vec3 N = normalize(vNormalEyeSpace);
    
    //Produto escalar entre a normal e a dire��o da luz
    //determina o cosseno do �ngulo entre N e L para calcular a intensidade da luz difusa
    //e aplica clamp para n�o ter valores negativos e n�o contribuir se estiver atr�s da superf�cie
    float NdotL = max(dot(N, L), 0.0);
    
    //Calcula a contribui��o da luz difusa
    //Multiplica o coeficiente de reflex�o difusa do material (Kd)
    //pela componente de luz difusa da fonte de luz (Id) e pelo cosseno do �ngulo entre a normal e a dire��o da luz
    vec4 diffuse = vec4(material.diffuse * light.diffuse, 1.0) * NdotL;
    //Calcula a dire��o do vetor de vis�o
    vec3 V = normalize(-vPositionEyeSpace);
    //Reflete a dire��o da luz em rela��o � normal
    vec3 R = reflect(-L, N);
    //Produto escalar entre a dire��o refletida e a dire��o do vetor de vis�o
    //Calcula a intensidade da luz especular
    float RdotV = max(dot(R, V), 0.0);
    //C�lculo da contribui��o da luz especular elevando o produto escalar ao brilho do material
    //e multiplicando pelo coeficiente de reflex�o especular do material (Ks) e pela componente de luz especular da fonte de luz (Is)
    vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

    //Calcula a dist�ncia entre o ponto de luz e o fragmento no espa�o da camera
    float dist = length(mat3(View) * light.position - vPositionEyeSpace);
    //Calcula a atenua��o da luz com a dist�ncia
    //Utiliza uma f�rmula de atenua��o que inclui termos constante, linear e quadr�tico
    //light.constant: atenua��o constante
    //light.linear: atenua��o proporcional � dist�ncia
    //light.quadratic: atenua��o proporcional ao quadrado da dist�ncia
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    //Retorna a contribui��o total da luz pontual considerando a atenua��o.
    return (attenuation * (diffuse + specular));
}

vec4 calcSpotLight(SpotLight light, out vec4 ambient) {
    // Adicione aqui o c�digo necess�rio para calcular a contribui��o da fonte de luz c�nica para a cor final do fragmento.
    ambient = vec4(0.0);
    return vec4(0.0);
}

void main()
{
    vec4 ambient; //Vari�vel para a luz ambiente
    vec4 light[4]; //Array para armazenar as contribui��es das diferentes fontes de luz
    vec4 ambientTmp; //Vari�vel tempor�ria para a luz ambiente
    ambient = calcAmbientLight(ambientLight); //Calcula a luz ambiente global
    light[0] = calcDirectionalLight(directionalLight, ambientTmp); //Calcula a luz direcional adicionando a contribui��o da luz ambiente � vari�vel tempor�ria
    ambient += ambientTmp;

    //Calcula a contribui��o das duas fontes de luz pontual adicionando a contribui��o da luz ambiente � vari�vel tempor�ria
    for(int i = 0; i < 2; i++) {
        light[i+1] = calcPointLight(pointLight[i], ambientTmp);
        ambient += ambientTmp;
    }
    light[3] = calcSpotLight(spotLight, ambientTmp); //Calcula a luz c�nica adicionando a contribui��o da luz ambiente � vari�vel tempor�ria
    ambient += ambientTmp;

    vec3 color = texture(ballSampler, textureVector).rgb; //Obtem a cor da textura aplicada ao fragmento
    vec3 finalColor = color * ((ambient/4) + light[0] + light[1] + light[2] + light[3]).rgb; //Calcula a cor final do fragmento

    //Define a cor final do fragmento, com o componente alpha igual a 1.0 (totalmente opaco)
    fColor = vec4(finalColor, 1.0);
}