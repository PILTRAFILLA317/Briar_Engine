#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D tex0;
uniform bool isTextured;

void main() {
    if (isTextured) {
        FragColor = texture(tex0, TexCoord);
    } else {
        FragColor = vec4(ourColor, 1.0);
    }
    // FragColor = texture(tex0, TexCoord);
    // FragColor = vec4(ourColor, 1.0);
}
