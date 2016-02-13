#version 330 core
out vec4 color;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light {
    vec3 Position;
    vec3 Color;
    
    float Linear;
    float Quadratic;
	float Radius;
};

uniform Light light;
uniform vec3 viewPos;
uniform vec2 screen;

void main()
{             
	vec2 TexCoords = gl_FragCoord.xy / screen;
    vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    vec3 norm = normalize(Normal);
	vec3 viewDir  = normalize(viewPos - FragPos);

	vec3 lightDir = normalize(light.Position - FragPos);
    // Diffuse shading
    float diff = max(dot(norm, lightDir), 0.0);
    // Specular shading
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16.);
    // Combine results
    vec3 diffuse = light.Color * diff * Diffuse;
    vec3 specular = light.Color * spec * Specular;
	float distance = length(light.Position - FragPos);
	float attenuation = 1.0 / (1.0 + light.Linear * distance + light.Quadratic * distance * distance);
	
	color.rgb = (diffuse + specular) * attenuation;
}