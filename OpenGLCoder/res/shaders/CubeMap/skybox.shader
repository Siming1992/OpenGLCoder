#shader vertex				//--------------vertex--------------------
#version 330 core

layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 view;
uniform mat4 proj;

void main()
{
    TexCoords = aPos;
    vec4 pos = proj * view * vec4(aPos, 1.0);
    //透视除法是在顶点着色器运行之后执行的，将gl_Position的xyz坐标除以w分量。相除结果的z分量等于顶点的深度值。
    //我们可以将输出位置的z分量等于它的w分量，让z分量永远等于1.0，这样子的话，当透视除法执行之后，z分量会变为w / w = 1.0。
    gl_Position = pos.xyww;
}

#shader fragment			//--------------fragment--------------------
#version 330 core

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, TexCoords);
}