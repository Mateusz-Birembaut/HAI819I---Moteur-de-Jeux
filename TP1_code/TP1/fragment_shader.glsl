#version 330 core

// Ouput data
out vec4 color;

in vec3 fragColor;

in vec2 fragUV;
in vec3 fragNormal;

uniform sampler2D u_texture;

void main(){

        //color = fragNormal;
        color = texture(u_texture, fragUV);

}
