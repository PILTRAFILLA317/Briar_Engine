#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 camMatrix;
uniform mat4 modelMatrix;

void main() {
    FragPos = vec3(modelMatrix * vec4(aPos, 1.0));
    gl_Position = camMatrix * modelMatrix * vec4(aPos, 1.0);

    ourColor = aColor;
	TexCoord = aTexCoord;
    Normal = normalize(mat3(transpose(inverse(modelMatrix))) * aNormal);

}
