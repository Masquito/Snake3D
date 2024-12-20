#version 460 core


out vec4 FragColor;


in vec3 color;
in vec2 TexCoord;
in vec3 Normal;
in vec3 crntPos;


uniform sampler2D tex0; 
uniform sampler2D tex1;
uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

void main()
{
	float ambient = 0.2f;
	
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);

	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.5f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16dddddd);
	float specular = specAmount * specularLight;

	FragColor = texture(tex0, TexCoord) * lightColor * (diffuse + ambient) + texture(tex1, TexCoord).r * specular;
}