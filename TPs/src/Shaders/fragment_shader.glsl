#version 330 core

// Ouput data
out vec4 color;

in vec3 fragColor;
in vec4 fragPosition;
in vec2 fragUV;
in vec3 fragNormal;

uniform bool u_hasTexture;
uniform sampler2D u_texture;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(){
    if(u_hasTexture) {
        color = texture(u_texture, fragUV);
    }
    else {
        color = vec4(fragColor, 1.0);
    }
}