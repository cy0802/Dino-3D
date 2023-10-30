#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 fragPos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 local;

void main(){
	gl_Position = projection * view * model * local * vec4(aPos, 1.0);

    fragPos = gl_Position.xyz;// vec3(model * vec4(aPos, 1.0));
    normal = mat3(transpose(inverse(local))) * aNormal;
    // normal = aNormal;
	// diffuse 
    // vec3 norm = normalize(normal);
    // vec3 lightDir = normalize(lightPos - pos);
    // vec3 diffuse = max(dot(norm, lightDir), 0.0) * lightColor;

	// specular
	// float specularStrength = 1.0; // this is set higher to better show the effect of Gouraud shading 
    // vec3 viewDir = normalize(viewPos - pos);
    // vec3 reflectDir = reflect(-lightDir, norm);  
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // vec3 specular = specularStrength * spec * lightColor;  

    // lightingColor = ambient + diffuse + specular;
}