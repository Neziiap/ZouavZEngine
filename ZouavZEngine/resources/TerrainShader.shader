#ifdef COMPILING_VS

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 Pos;
out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	Pos      = vec3(model * vec4(aPos, 1.0));
	Normal   = mat3(transpose(inverse(model))) * aNormal;     
	TexCoord = aTexCoord;
	
	gl_Position = projection * view * vec4(Pos, 1.0);
}
#endif

#ifdef COMPILING_FS

#define MAX_LIGHTS 1
#define MAX_COLOR 8

struct Light
{
    vec3 color;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 position;
    vec3 direction;
    vec3 ConstLineQuad;
    vec2 cutOffOuterCutOff;
    int  type;
};

out vec4 FragColor;
  
in vec3 Pos;
in vec2 TexCoord;
in vec3 Normal;

uniform vec3      viewPos; 
uniform Light     lights[1];

uniform int colorCount;
uniform float colorHeight[MAX_COLOR];
uniform float colorBlend[MAX_COLOR];
uniform float colorStrength[MAX_COLOR];
uniform vec3 colors[MAX_COLOR];

uniform float textureScale[MAX_COLOR];
uniform sampler2D textures[MAX_COLOR];

uniform float minHeight;
uniform float maxHeight;

vec3 triplanarMultiTexturing(vec3 pos, float scale, vec3 blendAxes, int textureIndex);
vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec3 color);
vec3 CalcPointLight(Light light, vec3 normal, vec3 Pos, vec3 viewDir, vec3 color);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 Pos, vec3 viewDir, vec3 color);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - Pos);
    
    vec3 result = vec3(0.0, 0.0, 0.0);    

    vec3 color = colors[0];
    vec3 blendAxes = abs(Normal);
    blendAxes /= blendAxes.x + blendAxes.y + blendAxes.z;
    float drawStrength = 0;
    float heightPercent = smoothstep(minHeight, maxHeight, Pos.y);
    
    for(int i = 0; i < colorCount; i++)
    {
        drawStrength = smoothstep(-colorBlend[i]/2 - 0.0001, colorBlend[i]/2, heightPercent - colorHeight[i]);
    
        vec3 colorTint = colors[i] * colorStrength[i];
        vec3 colorTexture = triplanarMultiTexturing(Pos, textureScale[i], blendAxes, i) * (1 - colorStrength[i]);
    
        color = color * (1 - drawStrength) + (colorTint + colorTexture) * drawStrength;
    }

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (lights[i].type == 0)
            result += CalcDirLight(lights[i], norm, viewDir, color);

        if (lights[i].type == 1)
            result += CalcPointLight(lights[i], norm, Pos, viewDir, color);    

        if (lights[i].type == 2)
            result += CalcSpotLight(lights[i], norm, Pos, viewDir, color);    
    }
    
    FragColor = vec4(result, 1.0);
}

vec3 triplanarMultiTexturing(vec3 pos, float scale, vec3 blendAxes, int textureIndex)
{
    vec3 scaledPos = pos / scale;
    vec3 xProj = vec3(texture(textures[textureIndex], vec2(scaledPos.y, scaledPos.z)) * blendAxes.x);
    vec3 yProj = vec3(texture(textures[textureIndex], vec2(scaledPos.x, scaledPos.z)) * blendAxes.y);
    vec3 zProj = vec3(texture(textures[textureIndex], vec2(scaledPos.x, scaledPos.y)) * blendAxes.z);

    return xProj + yProj + zProj;
}

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir, vec3 color)
{
    vec3 lightDir       = normalize(-light.direction);

    float diff          = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir     = reflect(-lightDir, normal);
    float spec          = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 ambient        = light.ambient * color;
    vec3 diffuse        = light.diffuse * diff * color;
    //vec3 specular       = light.specular * spec * color;

    return (ambient + diffuse);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 Pos, vec3 viewDir, vec3 color)
{
    vec3 lightDir       = normalize(light.position - Pos);

    float diff          = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir     = reflect(-lightDir, normal);
    float spec          = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    float distance      = length(light.position - Pos);
    float attenuation   = 1.0 / (light.ConstLineQuad.x + 
                                 light.ConstLineQuad.y * distance + 
                                 light.ConstLineQuad.z * (distance * distance));    

    vec3 ambient        = light.ambient * color;
    vec3 diffuse        = light.diffuse * diff * color;
    //vec3 specular       = light.specular * spec * color;
    
    ambient            *= attenuation;
    diffuse            *= attenuation;
    //specular           *= attenuation;

    return (ambient + diffuse);
}

vec3 CalcSpotLight(Light light, vec3 normal, vec3 Pos, vec3 viewDir, vec3 color)
{
    vec3 lightDir       = normalize(light.position - Pos);

    float diff          = max(dot(normal, lightDir), 0.0);

    vec3 reflectDir     = reflect(-lightDir, normal);
    float spec          = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    float distance      = length(light.position - Pos);
    float attenuation   = 1.0 / (light.ConstLineQuad.x + 
                                 light.ConstLineQuad.y * distance + 
                                 light.ConstLineQuad.z * (distance * distance));    

    float theta         = dot(lightDir, normalize(-light.direction)); 
    float epsilon       = light.cutOffOuterCutOff.x - light.cutOffOuterCutOff.y;
    float intensity     = clamp((theta - light.cutOffOuterCutOff.y) / epsilon, 0.0, 1.0);

    vec3 ambient        = light.ambient * color;
    vec3 diffuse        = light.diffuse * diff * color;
    //vec3 specular       = light.specular * spec * color;
    ambient            *= attenuation * intensity;
    diffuse            *= attenuation * intensity;
    //specular           *= attenuation * intensity;

    return (ambient + diffuse);
}
#endif