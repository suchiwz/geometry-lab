#version 330 core
layout (location = 0) in vec3 v_pos;
layout (location = 1) in vec3 v_normal;
layout (location = 2) in vec3 v_color;

out vec3 pos;
out vec3 normal;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    pos = (model * vec4(v_pos, 1.0)).xyz;
    normal = (model * vec4(v_normal, 0.0)).xyz;  
    color = v_color;
    gl_Position = projection * view * vec4(pos, 1.0);
}