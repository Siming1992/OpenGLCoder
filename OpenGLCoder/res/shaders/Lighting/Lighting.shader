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
	//如果模型矩阵执行了不等比缩放，顶点的改变会导致法向量不再垂直于表面了。因此，我们不能用这样的模型矩阵来变换法向量。
	//法线矩阵被定义为「模型矩阵左上角3x3部分的逆矩阵的转置矩阵」
	//矩阵求逆是一项对于着色器开销很大的运算，因为它必须在场景中的每一个顶点上进行，所以应该尽可能地避免在着色器中进行求逆运算。
	//以学习为目的的话这样做还好，但是对于一个高效的应用来说，你最好先在CPU上计算出法线矩阵，再通过uniform把它传递给着色器（就像模型矩阵一样）。
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

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform DirLight dirLight;

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
uniform PointLight pointLights[NR_POINT_LIGHTS];

// function prototypes
vec3 GetDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 GetPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
//vec3 GetSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	//vec3 ambient = texture(material.diffuse, TexCoords).rgb * light.ambient;

	//vec3 normal = normalize(Normal);
	//vec3 lightDir = normalize(LightPos - FragPos);
	//float diff = max(dot(lightDir, normal), 0);
	//vec3 diffuse = light.diffuse * (diff * texture(material.diffuse, TexCoords).rgb);

	//vec3 viewDir = normalize(-FragPos);
	//vec3 reflectDir = reflect(-lightDir, normal);
	//float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess);
	////float spec = max(dot(viewDir, reflectDir), 0.0);
	//vec3 specular = light.specular * (spec * texture(material.specular, TexCoords).rgb);

	//vec3 emission = texture(material.emission, TexCoords).rgb;

	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(-FragPos);
	vec3 dir = GetDirLight(dirLight, normal, viewDir);

	vec3 point = vec3(0.0, 0.0, 0.0);
	for (int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		//point = GetPointLight();
	}

	vec3 spot = vec3(0.0, 0.0, 0.0);

	FragColor = vec4((dir + point + spot), 1.0f);
}

vec3 GetDirLight(DirLight light,vec3 normal,vec3 viewDir) {
	vec3 lightDir = normalize(-light.direction);
	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);
	// 镜面光着色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// 合并结果
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
	return (ambient + diffuse + specular);
}

vec3 GetPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
	return vec3(0.0, 0.0, 0.0);
}
