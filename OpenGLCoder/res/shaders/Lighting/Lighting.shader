#shader vertex				//--------------vertex--------------------
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMat;

uniform vec3 lightPos; 

out vec3 FragPos;
out vec3 Normal;
out vec3 LightPos;
out vec2 TexCoords;

void main()
{
	//���ģ�;���ִ���˲��ȱ����ţ�����ĸı�ᵼ�·��������ٴ�ֱ�ڱ����ˡ���ˣ����ǲ�����������ģ�;������任��������
	//���߾��󱻶���Ϊ��ģ�;������Ͻ�3x3���ֵ�������ת�þ���
	//����������һ�������ɫ�������ܴ�����㣬��Ϊ�������ڳ����е�ÿһ�������Ͻ��У�����Ӧ�þ����ܵر�������ɫ���н����������㡣
	//��ѧϰΪĿ�ĵĻ����������ã����Ƕ���һ����Ч��Ӧ����˵�����������CPU�ϼ�������߾�����ͨ��uniform�������ݸ���ɫ��������ģ�;���һ������
	//Normal = mat3(transpose(inverse(model))) * aNormal;
	Normal = normalMat  * aNormal;
	//world position
	FragPos = vec3(view * model * vec4(aPos, 1.0));

	LightPos = vec3(view * vec4(lightPos, 1.0f));

	TexCoords = aTexCoords;

	gl_Position = proj * view * model * vec4(aPos, 1.0);
}

#shader fragment			//--------------fragment--------------------
#version 330 core

in vec3 FragPos;
in vec3 LightPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

//uniform vec3 objectColor;
//uniform vec3 lightColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

uniform Material material;

struct Light {
	//vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main()
{
	vec3 ambient = texture(material.diffuse, TexCoords).rgb * light.ambient;

	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(LightPos - FragPos);
	float diff = max(dot(lightDir, normal), 0);
	vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

	vec3 viewDir = normalize(-FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	//float spec = max(dot(viewDir, reflectDir), 0.0);
	vec3 specular = light.specular * (spec * texture(material.specular, TexCoords).rgb);

	vec3 emission = texture(material.emission, TexCoords).rgb;

	FragColor = vec4((ambient + diffuse + specular + emission), 1.0f);
}
