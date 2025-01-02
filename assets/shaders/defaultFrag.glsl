#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
uniform sampler2D tex0;
uniform bool isTextured = false;

in vec3 Normal;
in vec3 FragPos;

uniform int lightType;
uniform vec3 lightPos;   // Posición de la luz
uniform vec3 lightDir;   // Dirección de la luz
uniform vec3 lightColor; // Color de la luz
uniform float lightIntensity; // Intensidad de la luz
uniform float lightRange;

uniform vec3 ambientColor; // Color ambiental
uniform float ambientIntensity; // Intensidad ambiental

vec4 pointLight() {	
	// used in two variables so I calculate it here to not have to do it twice
    vec3 lightVec = lightPos-FragPos;

	// intensity of light with respect to distance
    float dist = length(lightVec);
    float inten = lightIntensity/(1.0+0.09*pow(dist, 2.0))*(1.0-clamp((dist/lightRange), 0.0, 1000.0));

	// diffuse lighting
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normal, lightDirection), 0.0);

    vec3 ambient = ambientIntensity*lightColor*ambientColor;
    vec3 result = (ambient+diffuse*inten)*ourColor;

    if(isTextured) {
        // FragColor = texture(tex0, TexCoord);
        FragColor = texture(tex0, TexCoord)*vec4(result, 1.0);

    } else {
        // FragColor = vec4(ourColor, 1.0) * vec4(lightColor, 1.0);
        FragColor = vec4(result, 1.0);
        // FragColor = vec4(norm, 1.0);
    }
    return FragColor;
}

vec4 direcLight() {
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightDir);
    float diffuse = max(dot(normal, lightDirection), 0.0);

    vec3 ambient = ambientIntensity*lightColor*ambientColor;

    if(isTextured) {
        return texture(tex0, TexCoord)*vec4((diffuse+ambient)*lightColor, 1.0);
    }

    return vec4(ourColor, 1.0)*vec4((diffuse+ambient)*lightColor, 1.0);
}

vec4 spotLight() {
    float outerCone = 0.90;
    float innerCone = 0.95;

    vec3 lightVec = lightPos-FragPos;
    float dist = length(lightVec);
    // float inten = lightIntensity/(1.0+0.09*pow(dist, 2.0))*(1.0-clamp((dist/lightRange), 0.0, 1000.0));

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normal, lightDirection), 0.0);

    float angle = dot(lightDirection, normalize(-lightDir));
    float inten = clamp((angle-outerCone)/(innerCone-outerCone), 0.0, 1.0);

    vec3 ambient = ambientIntensity*lightColor*ambientColor;
    vec3 result = (ambient+diffuse*inten)*ourColor;

    if(isTextured) {
        // FragColor = texture(tex0, TexCoord);
        FragColor = texture(tex0, TexCoord)*vec4(result, 1.0);

    } else {
        // FragColor = vec4(ourColor, 1.0) * vec4(lightColor, 1.0);
        FragColor = vec4(result, 1.0);
        // FragColor = vec4(norm, 1.0);
    }
    return FragColor;
}

void main() {
    // Normalización
    if(lightType==1) {
        FragColor = pointLight();
    } else if(lightType==0) {
        FragColor = direcLight();
    } else if(lightType==2) {
        FragColor = spotLight();
    }
}
