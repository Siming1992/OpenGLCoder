#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

uniform mat4 transform;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
	gl_Position = transform * vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}

#shader fragment
#version 330 core

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixNum;

out vec4 FragColor;

void main()
{
	//FragColor = vec4(ourColor, 1.0);
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1- TexCoord.x, TexCoord.y)), mixNum);
	//FragColor = texture(texture2, TexCoord);
}