#version 330 core
out vec4 FragColor;

in vec3 Normal; 
in vec3 FragPos;
  
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    //ambient
    float ambientIntensity = 0.1;
    vec3 ambientLight = ambientIntensity * lightColor;
    vec3 ambientColor = ambientLight * objectColor;

    //diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float theta = max(dot(norm, lightDir),0);
    vec3 diffuseLight = theta*lightColor;
    vec3 diffuseColor = diffuseLight*objectColor;

    //specular
    float specularIntensity = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir,reflectDir), 0), 32);
    vec3 specularLight = spec*lightColor;
    vec3 specularColor = specularLight * objectColor;

    //Final color
    FragColor = vec4(ambientColor + diffuseColor + specularColor, 1.0);
}