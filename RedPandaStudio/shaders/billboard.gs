#version 450 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 VPMatrix;
uniform vec3 cameraPosition;

out vec2 TextCoord;

//The technique used for developing the billboards in our engine is based in the tutorials provided by ogldev.atspace.co.uk
//More precisely, it's based on this tutorial: http://ogldev.atspace.co.uk/www/tutorial27/tutorial27.html
//This also includes our shaders, used for rendering the billboards
void main()
{
    
    vec3 p = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(cameraPosition - p);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up);

    //We are emitting the four vertex here in order, assigning the texture coordinates previously
    p -= (0.5 * right);
    gl_Position = VPMatrix * vec4(p, 1.0);
    TextCoord = vec2(1.0, 1.0);
    EmitVertex();

    p.y += 1.0;
    gl_Position = VPMatrix * vec4(p, 1.0);
    TextCoord = vec2(1.0, 0.0);
    EmitVertex();

    p.y -= 1.0;
    p += right;
    gl_Position = VPMatrix * vec4(p, 1.0);
    TextCoord = vec2(0.0, 1.0);
    EmitVertex();
    

    p.y += 1.0;
    gl_Position = VPMatrix * vec4(p, 1.0);
    TextCoord = vec2(0.0, 0.0);
    EmitVertex();

    EndPrimitive();

}