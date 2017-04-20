#version 330 core
layout (location = 0) in vec4 pos; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 proj;

void main()
{
    gl_Position = proj * vec4(pos.xy, 0.0, 1.0);
    TexCoords = pos.zw;
}  
