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
}