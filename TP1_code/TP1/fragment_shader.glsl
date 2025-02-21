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

void main(){

        //color = fragNormal;
        vec4 grassT = texture(u_grassTexture, fragUV);
        vec4 rockT = texture(u_rockTexture, fragUV);
        vec4 snowrockT = texture(u_snowrockTexture, fragUV);
        
        vec4 color1 = mix(grassT, rockT, step(0.2, fragPosition.y));

        color = mix(color1, snowrockT, step(0.4, fragPosition.y));

        //color = texture(u_heightMap, fragUV);
        
}
