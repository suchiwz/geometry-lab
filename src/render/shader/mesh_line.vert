#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;

out vec3 pos;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float bias;

void main()
{
    pos = (model * vec4(v_pos - bias * v_normal, 1.0)).xyz;
    normal = (model * vec4(v_normal, 0.0)).xyz;  
    gl_Position = projection * view * vec4(pos, 1.0);
}