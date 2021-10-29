#ifdef COMPILING_VS

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 tex;
layout(location = 3) in ivec4 boneIds;
layout(location = 4) in vec4 weights;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 color;

const int MAX_BONES = 100;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBonesMatrices[MAX_BONES];

out vec2 TexCoord;
out vec4 Color;
out vec3 Normal;
out vec3 Pos;

void main()
{
    vec4 totalPosition = vec4(0.0f);
    for (int i = 0; i < MAX_BONE_INFLUENCE; i++)
    {
        if (boneIds[i] == -1)
            continue;
        if (boneIds[i] >= MAX_BONES)
        {
            totalPosition = vec4(pos, 1.0f);
            break;
        }
        vec4 localPosition = finalBonesMatrices[boneIds[i]] * vec4(pos, 1.0f);
        totalPosition += localPosition * weights[i];
        vec3 localNormal = mat3(finalBonesMatrices[boneIds[i]]) * norm;
    }

    Pos      = vec3(model * totalPosition);
    Normal   = mat3(transpose(inverse(model))) * norm;

    mat4 viewModel = view * model;
    gl_Position = projection * viewModel * totalPosition;
    TexCoord = tex;
    Color = color;
}

#endif

#ifdef COMPILING_FS

#define MAX_LIGHTS 3

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

in vec2 TexCoord;
in vec4 Color;
in vec3 Pos;
in vec3 Normal;

uniform sampler2D ourTexture;
uniform vec3      viewPos; 
uniform Light     lights[3];
uniform int nbLight;

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(Light light, vec3 normal, vec3 Pos, vec3 viewDir);
vec3 CalcSpotLight(Light light, vec3 normal, vec3 Pos, vec3 viewDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(Pos - viewPos);
    
    vec3 result;    
    result.x = 0;
    result.y = 0;
    result.z = 0;

    for (int i = 0; i < nbLight && i < MAX_LIGHTS; i++)
    {
        if (lights[i].type == 0)
            result += CalcDirLight(lights[i], norm, viewDir);

        if (lights[i].type == 1)
            result += CalcPointLight(lights[i], norm, Pos, viewDir);    

        if (lights[i].type == 2)
            result += CalcSpotLight(lights[i], norm, Pos, viewDir);    
    }

    FragColor = vec4(result, 1.0) * Color;
}

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir       = normalize(light.direction);

    float diff          = max(dot(normal, -lightDir), 0.0);

    vec3 reflectDir     = reflect(-lightDir, normal);
    float spec          = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    vec3 ambient        = light.ambient * vec3(texture(ourTexture, TexCoord));
    vec3 diffuse        = light.diffuse * diff * vec3(texture(ourTexture, TexCoord));
    vec3 specular       = light.specular * spec * vec3(texture(ourTexture, TexCoord));

    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(Light light, vec3 normal, vec3 Pos, vec3 viewDir)
{
    vec3 lightDir       = normalize(Pos - light.position);

    float diff          = max(dot(normal, -lightDir), 0.0);

    vec3 reflectDir     = reflect(-lightDir, normal);
    float spec          = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    float distance      = length(light.position - Pos);
    float attenuation   = 1.0 / (light.ConstLineQuad.x + 
                                 light.ConstLineQuad.y * distance + 
                                 light.ConstLineQuad.z * (distance * distance));    

    vec3 ambient        = light.ambient * vec3(texture(ourTexture, TexCoord));
    vec3 diffuse        = light.diffuse * diff * vec3(texture(ourTexture, TexCoord));
    vec3 specular       = light.specular * spec * vec3(texture(ourTexture, TexCoord));
    
    ambient            *= attenuation;
    diffuse            *= attenuation;
    specular           *= attenuation;

    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(Light light, vec3 normal, vec3 Pos, vec3 viewDir)
{
    vec3 lightDir       = normalize(Pos - light.position);

    float diff          = max(dot(normal, -lightDir), 0.0);

    vec3 reflectDir     = reflect(-lightDir, normal);
    float spec          = pow(max(dot(viewDir, reflectDir), 0.0), 32);

    float distance      = length(light.position - Pos);
    float attenuation   = 1.0 / (light.ConstLineQuad.x + 
                                 light.ConstLineQuad.y * distance + 
                                 light.ConstLineQuad.z * (distance * distance));    

    float theta         = dot(lightDir, normalize(-light.direction)); 
    float epsilon       = light.cutOffOuterCutOff.x - light.cutOffOuterCutOff.y;
    float intensity     = clamp((theta - light.cutOffOuterCutOff.y) / epsilon, 0.0, 1.0);

    vec3 ambient        = light.ambient * vec3(texture(ourTexture, TexCoord));
    vec3 diffuse        = light.diffuse * diff * vec3(texture(ourTexture, TexCoord));
    vec3 specular       = light.specular * spec * vec3(texture(ourTexture, TexCoord));
    ambient            *= attenuation * intensity;
    diffuse            *= attenuation * intensity;
    specular           *= attenuation * intensity;

    return (ambient + diffuse + specular);
}

#endif