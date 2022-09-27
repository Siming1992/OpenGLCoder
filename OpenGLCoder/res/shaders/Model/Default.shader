#shader vertex				//--------------vertex--------------------
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec2 TexCoords;

void main()
{
	TexCoords = aTexCoords;
	gl_Position = proj * view *	model * vec4(aPos, 1.0);
}

#shader fragment			//--------------fragment--------------------
#version 330 core

in vec2 TexCoords;

struct Material {
	sampler2D texture_diffuse1;
};

uniform Material material;

out vec4 FragColor;

void main()
{
	FragColor = texture(material.texture_diffuse1, TexCoords);
}
