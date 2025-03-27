#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;

layout(location = 1) in vec2 uv;

layout(location = 2) in uvec2 pitchYaw;

out vec3 fragNormal;
out vec2 fragUV;
out vec4 fragPosition;

uniform bool u_hasHeightmap;
uniform sampler2D u_heightmap;

//TODO create uniform transformations matrices Model View Projection
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

// Function to reconstruct the normal from the pitchYaw
vec3 reconstructNormal(uvec2 pitchYaw){
    float pitch = (pitchYaw.x / 65535.0f) * 2.0 - 1.0; // [-1,1]
    float yaw = ((pitchYaw.y /65535.0f) - 0.5) * 3.141592 ; // [-π/2, π/2]

    float phi = pitch * 3.141592 - 3.141592/2.0f;
    float theta = yaw * 2.0f * 3.141592;

    return vec3(
        cos(theta) * cos(phi),
        sin(phi),
        sin(theta) * cos(phi)
    );
}

// Values that stay constant for the whole mesh.
void main(){
        mat4 mvp = u_projection * u_view * u_model;
        fragNormal =  reconstructNormal(pitchYaw);
        if(u_hasHeightmap){
            vec3 modified_position = vertices_position_modelspace + fragNormal * texture(u_heightmap, uv).r ;
            gl_Position = mvp * vec4(modified_position, 1.0);
            fragPosition = u_model * vec4(modified_position, 1.0);
        }else {
            gl_Position = mvp * vec4(vertices_position_modelspace, 1.0);
            fragPosition = vec4(vertices_position_modelspace, 1.0);
        }
        fragUV = uv;
}

