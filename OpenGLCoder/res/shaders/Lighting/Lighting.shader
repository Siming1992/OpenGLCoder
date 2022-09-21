#shader vertex				//--------------vertex--------------------
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
uniform mat3 normalMat;

out vec3 FragPos;
out vec3 Normal;
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

	TexCoords = aTexCoords;

	gl_Position = proj * view * model * vec4(aPos, 1.0);
}

#shader fragment			//--------------fragment--------------------
#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

#define NR_POINT_LIGHTS 4

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

// function prototypes
vec3 GetDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 GetPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 GetSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(-FragPos);
	vec3 dir = vec3(0.0, 0.0, 0.0);
	dir = GetDirLight(dirLight, normal, viewDir);

	vec3 point = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		point += GetPointLight(pointLights[i], normal, FragPos, viewDir);
	}

	vec3 spot = GetSpotLight(spotLight, normal, FragPos, viewDir);

	FragColor = vec4((dir + point + spot), 1.0f);

}

vec3 GetDirLight(DirLight light,vec3 normal,vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// ��������ɫ
	float diff = max(dot(normal, lightDir), 0.0);
	// �������ɫ
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// �ϲ����
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse + specular);
}

vec3 GetPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 viewLightPos = vec3(view * vec4(light.position, 1.0f));

	vec3 lightDir = normalize(viewLightPos - fragPos);
	// ��������ɫ
	float diff = max(dot(normal, lightDir), 0.0);
	// �������ɫ
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// ˥��
	float distance = length(viewLightPos - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance +
		light.quadratic * (distance * distance));
	// �ϲ����
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
	return (ambient + diffuse + specular);
}

vec3 GetSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	vec3 viewLightPos = vec3(view * vec4(light.position, 1.0f));
	vec3 viewLightDir = vec3(view * vec4(light.direction, 1.0f));

	vec3 lightDir = normalize(viewLightPos - fragPos);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// attenuation
	float distance = length(viewLightPos - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
	// spotlight intensity
	float theta = dot(lightDir, normalize(-viewLightDir));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// combine results
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	vec3 emission = vec3(texture(material.emission, vec2(gl_FragCoord.xy / vec2(800, 600))));
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;
	emission *= attenuation * intensity;
	
	return (ambient + diffuse + specular + emission);
	//return vec3(diffuse);
}