#version 330 core

out vec4 FragColor;
in vec3 fragPos;
in vec3 normal;

struct Light {
	vec3 position;
    vec3 color;
    
    vec3 ambient;
};

vec3 calcLight(Light light, vec3 normal, vec3 viewDir);

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Light light[2];

void main(){
    vec3 normalized = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    
    vec3 result = calcLight(light[0], normalized, viewDir);
    result += calcLight(light[1], normalized, viewDir);
    
	FragColor = vec4(result * objectColor, 1.0);
}
vec3 calcLight(Light light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * light.color;
    
    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light.color;  

    vec3 ambient = light.ambient;
    return ambient + diffuse + specular;
}