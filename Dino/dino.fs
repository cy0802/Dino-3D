#version 330 core

out vec4 FragColor;
in vec3 fragPos;
in vec3 normal;

struct Light {
	vec3 position;
    vec3 color;
    vec3 constants; // for attenuation
    
    vec3 ambient;
};

vec3 calcLight(Light light, vec3 normal, vec3 viewDir);

uniform vec3 objectColor;
uniform vec3 viewPos;
uniform Light light[2];

void main(){
    // vec3 objectColor = vec3(0.478431, 0.517647, 0.552941);
    vec3 normalized = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);
    
    vec3 result = calcLight(light[0], normalized, viewDir);
    result += calcLight(light[1], normalized, viewDir);
    
    // FragColor = vec4(light[0].color, 1.0);
	FragColor = vec4(result * objectColor, 1.0);
	// FragColor = texture(_texture, TexCoord);
}
vec3 calcLight(Light light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(light.position - fragPos);
    
    vec3 diffuse = max(dot(normal, lightDir), 0.0) * light.color;
    
    float specularStrength = 0.5;
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * light.color;  

    // float distance = length(light.position - fragPos);
    // float attenuation = 0.05f;
    // float attenuation = 1.0 / (light.constants.x + light.constants.y * distance + light.constants.z * (distance * distance));
    vec3 ambient = light.ambient;
    // diffuse = attenuation * diffuse;
    // specular = attenuation * specular;
    return ambient + diffuse + specular;
}