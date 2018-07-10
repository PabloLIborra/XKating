#version 450 core

//Pixel color and texture variables
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D screenTexture;

//Final color
vec4 color;

//=================================
// Post processing values
//=================================
//Type of postprocessing applied
uniform int option = 0;

//Pixel offset for the kernel operations
uniform float offset = 1.0 / 300.0;

//Neon factor
uniform float neonFactor = 1;

//Blur parameters
uniform float blur_x = 0.0;
uniform float blur_y = 0.0;
uniform float blur_strength = 2.2;
uniform float blur_dist = 1.0;

void main()
{
    //Default assignment
    color = texture(screenTexture, TexCoords);
    
    //Black and white
    if(option == 1)
    {
        float average = 0.2 * texture(screenTexture, TexCoords).r + 0.7 * texture(screenTexture, TexCoords).b + 0.1 * texture(screenTexture, TexCoords).g;
        color = vec4(average, average, average, 1.0 );
    }

    //Negative
    if(option == 2)
    {
        color = vec4(1 - texture(screenTexture, TexCoords).r, 1 - texture(screenTexture, TexCoords).g, 1 - texture(screenTexture, TexCoords).b, 1.0);
    }

    //Kernel operations
    if(option >= 3 && option <= 6)
    {
        //Pixel offset
        vec2 offsets[9];

        //Kernel we are going to use for the operation
        float kernel[9];

        //High contrast
        if(option == 3)
        {
            kernel = float[](
                -2, -2, -2,
                -2,  18, -2,
                -2, -2, -2
            );

            offsets = vec2[](
                vec2(-offset,  offset), // top-left
                vec2( 0.0f,    offset), // top-center
                vec2( offset,  offset), // top-right
                vec2(-offset,  0.0f),   // center-left
                vec2( 0.0f,    0.0f),   // center-center
                vec2( offset,  0.0f),   // center-right
                vec2(-offset, -offset), // bottom-left
                vec2( 0.0f,   -offset), // bottom-center
                vec2( offset, -offset)  // bottom-right    
            );
        }

        //Median filter
        if(option == 4)
        {
            kernel = float[](
                1/9.0, 1/9.0, 1/9.0,
                1/9.0, 1/9.0, 1/9.0,
                1/9.0, 1/9.0, 1/9.0
            );

            offsets = vec2[](
                vec2(-offset,  offset), // top-left
                vec2( 0.0f,    offset), // top-center
                vec2( offset,  offset), // top-right
                vec2(-offset,  0.0f),   // center-left
                vec2( 0.0f,    0.0f),   // center-center
                vec2( offset,  0.0f),   // center-right
                vec2(-offset, -offset), // bottom-left
                vec2( 0.0f,   -offset), // bottom-center
                vec2( offset, -offset)  // bottom-right    
            );
        }

        //Neon filter
        if(option == 5)
        {
            kernel = float[](
                1, 1,   1,
                1, 100, 1,
                1, 1,   1
            );
            
            offsets = vec2[](
                vec2(-offset,  offset), // top-left
                vec2( 0.0f,    offset), // top-center
                vec2( offset,  offset), // top-right
                vec2(-offset,  0.0f),   // center-left
                vec2( 0.0f,    0.0f),   // center-center
                vec2( offset,  0.0f),   // center-right
                vec2(-offset, -offset), // bottom-left
                vec2( 0.0f,   -offset), // bottom-center
                vec2( offset, -offset)  // bottom-right    
            );
        }

        //Little square of pixels where I operate
        vec3 square[9];
        for(int i = 0; i < 9; i++)
            square[i] = vec3(texture(screenTexture, TexCoords.st + offsets[i]));

        //Averaging the values of the sample texture with the matrix
        vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++)
            col += vec3(square[i] * kernel[i]);

        //final color value
        color = vec4(col, 1.0);
    }

    if(option == 6)
    {
        //Emanation of the blur
        vec2 blur_center = vec2(blur_x, blur_y);

        //radial position pixel samples
        float radial_samples[10];
        radial_samples[0] = -0.08;
        radial_samples[1] = -0.05;
        radial_samples[2] = -0.03;
        radial_samples[3] = -0.02;
        radial_samples[4] = -0.01;
        radial_samples[5] =  0.01;
        radial_samples[6] =  0.02;
        radial_samples[7] =  0.03;
        radial_samples[8] =  0.05;
        radial_samples[9] =  0.08;

        //Distance from the center of the blur action
        vec2 dir = 0.5 - TexCoords + blur_center; 
        float dist = sqrt(dir.x*dir.x + dir.y*dir.y); 
        dir = dir/dist; 

        //Added value of the pixels
        vec4 original_value = texture2D(screenTexture,TexCoords);
        vec4 sum = original_value;

        for (int i = 0; i < 10; i++)
            sum += texture2D( screenTexture, TexCoords + dir * radial_samples[i] * blur_dist); //Adding values of the color

        //Average value of displaced pixels
        sum *= 1.0/11.0;

        //Clamping light value to be between 0 and 1
        float t = dist * blur_strength;
        t = clamp( t ,0.0,1.0);

        //Mixing vectors (original_value, added value, clamped light)
        color = mix( original_value, sum, t );
    }

    FragColor = color;
} 