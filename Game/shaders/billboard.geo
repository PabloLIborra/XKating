#version 450 core

layout (points) in;
layout (triangle_strip) out;
layout (max_vertices = 4) out;

uniform mat4 VPMatrix;
uniform vec3 cameraPosition;

out vec2 TextCoord;

void main()
{
    
    vec3 p = gl_in[0].gl_Position.xyz;
    vec3 toCamera = normalize(cameraPosition - p);
    vec3 up = vec3(0.0, 1.0, 0.0);
    vec3 right = cross(toCamera, up);


    //p -= (right * 0.5);
    //p = right;
    gl_Position = VPMatrix * vec4(p, 1.0);
    TextCoord = vec2(0.0, 0.0);
    EmitVertex();

    //p.y += 20.0;
    //p.y += 1.0;
    gl_Position = VPMatrix * vec4(p, 1.0);
    TextCoord = vec2(0.0, 1.0);
    EmitVertex();
    
    //p.y -= 20.0;
    //p.x += 20.0;
    //p.y -= 1.0;
    //p += right;
    gl_Position = VPMatrix * vec4(p, 1.0);
    EmitVertex();
    TextCoord = vec2(1.0, 0.0);

    p.y += 20.0;
    //p.y += 1.0;
    gl_Position = VPMatrix * vec4(p, 1.0);
    TextCoord = vec2(1.0, 1.0);
    EmitVertex();

    EndPrimitive();

}