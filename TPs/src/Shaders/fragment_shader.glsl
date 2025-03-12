#version 330 core

// Ouput data
out vec4 color;

in vec3 fragColor;
in vec4 fragPosition;
in vec2 fragUV;
in vec3 fragNormal;

uniform sampler2D u_moonTexture;
uniform sampler2D u_earthTexture;
uniform int objectID; // 0 = earth, 1 = moon, etc.

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(){
    // Appliquer la texture en fonction de l'ID de l'objet
    if(objectID == 0) {
        // Terre
        color = texture(u_earthTexture, fragUV);
    }
    else if(objectID == 1) {
        // Lune
        color = texture(u_moonTexture, fragUV);
    }
    else {
        // Couleur par défaut
        color = vec4(fragColor, 1.0);
    }
}