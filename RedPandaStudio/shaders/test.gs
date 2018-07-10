#version 450 core

layout(triangles_adjacency) in;
layout(triangle_strip) out;
layout(max_vertices = 18) out;

in vec2 UV_Coordinates[];
in vec4 v_Color[];

float EdgeWidth;
float PctExtend;

out vec4 G_Color;
out vec2 GUV_Coordinates;

flat out int GIsEdge;

bool isFrontFacing(vec3 a, vec3 b, vec3 c)
{
    if(((a.x * b.y - b.x * a.y) + (b.x * c.y - c.x * b.y) + (c.x * a.y - a.x * c.y)) > 0 )
    {
        return true;
    }
    return false;
}

void emitEdgeQuad(vec3 e0, vec3 e1)
{
    EdgeWidth = 0.004;
    PctExtend = 0.00001;
 
    vec2 ext = PctExtend * (e1.xy - e0.xy);
    vec2 v = normalize(e1.xy - e0.xy);
    vec2 n = vec2(-v.y, v.x) * EdgeWidth;

    GIsEdge = 1;

    gl_Position = vec4(e0.xy - ext, e0.z, 1.0);
    EmitVertex();
    gl_Position = vec4(e0.xy - n - ext, e0.z, 1.0);
    EmitVertex();
    gl_Position = vec4(e1.xy + ext, e1.z, 1.0);
    EmitVertex();
    gl_Position = vec4(e1.xy - n + ext, e1.z, 1.0);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    vec3 p0 = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;
    vec3 p1 = gl_in[1].gl_Position.xyz / gl_in[1].gl_Position.w;
    vec3 p2 = gl_in[2].gl_Position.xyz / gl_in[2].gl_Position.w;
    vec3 p3 = gl_in[3].gl_Position.xyz / gl_in[3].gl_Position.w;
    vec3 p4 = gl_in[4].gl_Position.xyz / gl_in[4].gl_Position.w;
    vec3 p5 = gl_in[5].gl_Position.xyz / gl_in[5].gl_Position.w;

/*
    if( isFrontFacing(p0, p2, p4))
    {
        if(!isFrontFacing(p0, p1, p2))
        {
            emitEdgeQuad(p0, p2);
        }
        
        if(!isFrontFacing(p2, p3, p4))
        {
            emitEdgeQuad(p2, p4);
        }

        if(!isFrontFacing(p4, p5, p0))
        {
            emitEdgeQuad(p4, p0);
        }
    }
*/

    int i;
    GIsEdge = 0;
    for(i = 0; i < gl_in.length(); i+=2)
    {
        G_Color = v_Color[i];
        GUV_Coordinates = UV_Coordinates[i];
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}


/*#version 450 core

layout( triangles_adjacency ) in;
layout(triangle_strip, max_vertices = 18) out;

out vec3 GNormal;
out vec3 GPosition;

flat out int GIsEdge;


in vec4 v_Color;
in vec2 UV_Coordinates;


out vec4 G_Color;
out vec2 GUV_Coordinates;

in vec3 VNormal[];
in vec3 VPosition[];

float EdgeWidth;
float PctExtend;

bool isFrontFacing(vec3 a, vec3 b, vec3 c)
{
    return ((a.x * b.y - b.x * a.y) + (b.x * c.y - c.x * b.y) + (c.x * a.y - a.x * c.y)) > 0 ;
}

void emitEdgeQuad(vec3 e0, vec3 e1)
{
    EdgeWidth = 50;
    PctExtend = 10;
 
    vec2 ext = PctExtend * (e1.xy - e0.xy);
    vec2 v = normalize(e1.xy - e0.xy);
    vec2 n = vec2(-v.y, v.x) * EdgeWidth;

    GIsEdge = 1;

    gl_Position = vec4(e0.xy - ext, e0.z, 1.0);
    EmitVertex();
    gl_Position = vec4(e0.xy - n - ext, e0.z, 1.0);
    EmitVertex();
    gl_Position = vec4(e1.xy + ext, e1.z, 1.0);
    EmitVertex();
    gl_Position = vec4(e1.xy - n + ext, e1.z, 1.0);
    EmitVertex();

    EndPrimitive();
}

void main()
{
    
    G_Color = v_Color;
    GUV_Coordinates = UV_Coordinates;
    

    vec3 p0 = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w;
    vec3 p1 = gl_in[1].gl_Position.xyz / gl_in[1].gl_Position.w;
    vec3 p2 = gl_in[2].gl_Position.xyz / gl_in[2].gl_Position.w;
    vec3 p3 = gl_in[3].gl_Position.xyz / gl_in[3].gl_Position.w;
    vec3 p4 = gl_in[4].gl_Position.xyz / gl_in[4].gl_Position.w;
    vec3 p5 = gl_in[5].gl_Position.xyz / gl_in[5].gl_Position.w;

    if( isFrontFacing(p0, p2, p4))
    {
        if(!isFrontFacing(p0, p1, p2))
        {
            emitEdgeQuad(p0, p2);
        }
        
        if(!isFrontFacing(p2, p3, p4))
        {
            emitEdgeQuad(p2, p4);
        }

        if(!isFrontFacing(p0, p4, p5))
        {
            emitEdgeQuad(p4, p0);
        }
    }

    GIsEdge = 0;
    GNormal = VNormal[0];
    GPosition = VPosition[0];
    gl_Position = gl_in[0].gl_Position;
    EmitVertex();

    GNormal = VNormal[2];
    GPosition = VPosition[2];
    gl_Position = gl_in[2].gl_Position;
    EmitVertex();

    GNormal = VNormal[4];
    GPosition = VPosition[4];
    gl_Position = gl_in[4].gl_Position;
    EmitVertex();

    EndPrimitive();
}
*/