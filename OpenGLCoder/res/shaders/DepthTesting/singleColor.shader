#shader vertex				//--------------vertex--------------------
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = proj * view * model * vec4(aPos, 1.0);
}

#shader fragment			//--------------fragment--------------------
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

float near = 0.1;
float far = 100.0;

//http://www.songho.ca/opengl/gl_projectionmatrix.html
float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));
}

void main()
{
    FragColor = vec4(0.04, 0.28, 0.26, 1.0);
}
