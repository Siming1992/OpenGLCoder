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
    //͸�ӳ������ڶ�����ɫ������֮��ִ�еģ���gl_Position��xyz�������w��������������z�������ڶ�������ֵ��
    //���ǿ��Խ����λ�õ�z������������w��������z������Զ����1.0�������ӵĻ�����͸�ӳ���ִ��֮��z�������Ϊw / w = 1.0��
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