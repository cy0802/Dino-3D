#version 330 core

out vec4 FragColor;
in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

struct Light {
	vec3 position;
    vec3 color;
};

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

vec3 calcLight(Light light, vec3 normal, vec3 viewDir);

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Light light;
uniform sampler2D texture_;
uniform Material material;

void main(){
    vec3 normalized = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    
    vec3 result = calcLight(light, normalized, viewDir);
    
	FragColor = texture(texture_, texCoord) * vec4(result * objectColor, 1.0);
}

vec3 calcLight(Light light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * material.diffuse; // * light.color
    
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = 0.5 * spec * material.specular * light.color;  

    vec3 ambient = light.color * material.ambient;
    vec3 result = (ambient + diffuse + specular);
    return result;
}