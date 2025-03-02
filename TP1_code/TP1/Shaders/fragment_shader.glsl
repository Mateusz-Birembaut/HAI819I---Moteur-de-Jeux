#version 330 core

// Ouput data
out vec4 color;

in vec3 fragColor;
in vec4 fragPosition;
in vec2 fragUV;
in vec3 fragNormal;

uniform sampler2D u_grassTexture;
uniform sampler2D u_rockTexture;
uniform sampler2D u_snowrockTexture;

//uniform sampler2D u_heightMap;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(){

        vec4 grassT = texture(u_grassTexture, fragUV);
        vec4 rockT = texture(u_rockTexture, fragUV);
        vec4 snowrockT = texture(u_snowrockTexture, fragUV);

        vec4 color1 = mix(grassT, rockT, smoothstep(0.2, 0.3, fragPosition.y ));
        color = mix(color1, snowrockT, smoothstep(0.4, 0.5, fragPosition.y ));

        //color = texture(u_heightMap, fragUV);
        
}