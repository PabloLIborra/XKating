#version 450 core

precision mediump float;	// Precisión media

varying vec4 G_Color;		// in: color del vertex shader

in vec2 GUV_Coordinates; //UV del vertex shader

flat in int GIsEdge;

uniform sampler2D sampler;
uniform bool textActive;

out vec4 FragColor;

//================================
in vec4 lightPos;
in vec4 viewPos;

uniform vec4 lightSpaceView;

in vec4 FragPos;
in vec4 FragLightPos;
in vec3 Normal;

uniform sampler2D shadowMap;
float calculateShadow()
{
    //Coordinates normalized from the light point of view (in the range of [0,1])
    vec3 projectionCoordinates = FragLightPos.xyz / FragLightPos.w;
    projectionCoordinates *= 0.5;
    projectionCoordinates += 0.5;

    //Get the most proximate value from the light's perspective
    float closestDepth = texture(shadowMap, projectionCoordinates.xy).r;

    //Get the current depth of the fragment
    float currentDepth = projectionCoordinates.z;

    //Get the shadow value depending on what you see first from the light's perspective
    float shadow = (currentDepth > closestDepth) ? 1.0f : 0.0f;

    return shadow;
}
//================================

void main()
{
    vec3 color = texture(sampler, UV_Coordinates).rgb;
    vec3 normal = normalize(Normal);
    vec3 lightColor = vec3(0.3);

    // ambient
    vec3 ambient = 0.3 * color;

    // diffuse
    vec3 lightDir = vec3(normalize(lightPos - FragPos));
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = vec3(normalize(viewPos - FragPos));
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;    
    // calculate shadow
    float shadow = calculateShadow();                      
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;    
  
    FragColor = vec4(lighting, 1.0);
    
}