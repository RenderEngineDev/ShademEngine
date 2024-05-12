#ifndef FONG_LIGHT_SHADER
#define FONG_LIGHT_SHADER

//struct Material {
//	vec3 ambient;
//	vec3 diffuse;
//	vec3 specular;
//	float shininess;
//};
struct Material {
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
	float shininess;
};

struct PositionLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
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

// zostawić tutaj te uniformy czy wrzucić do fragment shadera????
uniform vec3 viewPos;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

vec3 calculateFongLights(vec3 normal, vec3 viewPos, vec3 fragPos, DirLight dirLight, PointLight pointLights[NR_POINT_LIGHTS], SpotLight spotLight)
{
	// properties
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(viewPos - fragPos);

	vec3 result = vec3(0.0f);
	// phase 1: Directional lighting
	result = CalcDirLight(dirLight, norm, viewDir);

	// phase 2: Point lights
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
		result += CalcPointLight(pointLights[i], norm, fragPos, viewDir);

	// phase 3: Spot light
	//result += CalcSpotLight(spotLight, norm, fragPos, viewDir);

	return result;
};

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
   	vec3 lightDir = normalize(-light.direction);
    
	// diffuse shading
    	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
    	vec3 reflectDir = reflect(-lightDir, normal);
    	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    	// combine results
    	/*vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));*/
		vec3 ambient = light.ambient * vec3(1.0f); //materialDiffuseColor;
		vec3 diffuse = light.diffuse * diff * vec3(1.0f); //materialDiffuseColor;
		vec3 specular = light.specular * spec * vec3(1.0f); //materialSpecularColor;

    	return (ambient + diffuse + specular);
}
// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);

	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// combine results
	/*vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));*/
	vec3 ambient = light.ambient * vec3(1.0f); //materialDiffuseColor;
	vec3 diffuse = light.diffuse * diff * vec3(1.0f); //materialDiffuseColor;
	vec3 specular = light.specular * spec * vec3(1.0f); //materialSpecularColor;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}
// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    /*vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));*/
	vec3 ambient = light.ambient * vec3(1.0f); //materialDiffuseColor;
	vec3 diffuse = light.diffuse * diff * vec3(1.0f); //materialDiffuseColor;
	vec3 specular = light.specular * spec * vec3(1.0f); //materialSpecularColor;

	ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

#endif