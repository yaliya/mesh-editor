#version 330 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 texCoords;

out vec3 fragmentPosition; // Fragment position
out vec3 fragmentNormal; // Fragment normal
out vec2 fragmentTexCoords; // Fragment texture coordinates

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    fragmentPosition = vec3(model * vec4(vertexPosition, 1.0));
    fragmentNormal = mat3(transpose(inverse(model))) * vertexNormal;
    fragmentTexCoords = vec2(texCoords.x, 1.0f - texCoords.y);
    gl_Position = projection * view * model * vec4(fragmentPosition, 1.0);
}