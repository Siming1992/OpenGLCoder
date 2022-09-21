#shader vertex				//--------------vertex--------------------
#version 330 core

layout(location = 0) in vec3 aPos;
//layout(location = 1) in vec2 aTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

//out vec2 TexCoord;

void main()
{
	gl_Position = proj * view *	model * vec4(aPos, 1.0);
}

#shader fragment			//--------------fragment--------------------
#version 330 core

uniform vec3 lightColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(lightColor, 1.0);
}
