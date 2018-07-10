#version 410 core

uniform samplerCube skybox;

in vec3 tc;

layout (location = 0) out vec4 color;

void main(void)
{
    color = texture(skybox, tc);
    //color = vec4(tc.x, tc.y, tc.z, 1.0);
}
