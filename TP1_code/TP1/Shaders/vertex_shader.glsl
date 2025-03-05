#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertices_position_modelspace;

layout(location = 1) in vec3 color;

layout(location = 2) in vec3 normal;

layout(location = 3) in vec2 uv;

out vec3 fragColor;
out vec3 fragNormal;
out vec2 fragUV;
out vec4 fragPosition;

//TODO create uniform transformations matrices Model View Projection
uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform sampler2D u_heightMap;

// Values that stay constant for the whole mesh.

void main(){
        mat4 mvp = u_projection * u_view * u_model;
        vec3 modified_position = vertices_position_modelspace;
        modified_position.y = texture(u_heightMap, uv).r;
        gl_Position = mvp * vec4(vertices_position_modelspace, 1.0); // modified_position pour les terrains
        fragPosition = u_model * vec4(modified_position, 1.0);
        fragColor = color;
        fragNormal = normal;
        fragUV = uv;
}

