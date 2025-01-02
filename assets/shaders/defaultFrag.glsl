#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D tex0;
uniform bool isTextured = false;

in vec3 Normal;
in vec3 FragPos;

uniform vec3 lightPos;   // Posición de la luz
uniform vec3 lightColor; // Color de la luz
uniform float lightIntensity; // Intensidad de la luz

uniform vec3 ambientColor; // Color ambiental
uniform float ambientIntensity; // Intensidad ambiental

void main() {
    // Normalización
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // Componente difusa
    float diff = max(dot(norm, lightDir), 0.0);

    vec3 diffuse = diff * lightColor * lightIntensity;

    // Propiedades de la luz
    // float specularStrength = 0.5; // Factor especular
    // vec3 viewDir = normalize(viewPos - FragPos);
    // vec3 reflectDir = reflect(-lightDir, norm);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // vec3 specular = specularStrength * spec * lightColor;

    vec3 ambient = ambientIntensity * lightColor * ambientColor;
    // vec3 result = (ambient + diffuse + specular) * ourColor;
    vec3 result = (ambient + diffuse) * ourColor;

    if (isTextured) {
        // FragColor = texture(tex0, TexCoord);
        FragColor = texture(tex0, TexCoord) * vec4(result, 1.0);

    } else {
        // FragColor = vec4(ourColor, 1.0) * vec4(lightColor, 1.0);
        FragColor = vec4(result, 1.0);
        // FragColor = vec4(norm, 1.0);
    }
}
