#ifdef COMPILING_VS
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec3 Pos = vec3(model * vec4(aPos, 1.0));
	Normal   = mat3(transpose(inverse(model))) * aNormal;     
	TexCoord = aTexCoord;

	gl_Position = projection * view * vec4(Pos, 1.0);
}

#endif

#ifdef COMPILING_FS

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

uniform vec4 color;

uniform sampler2D fontAtlas;

uniform float width;
uniform float edge;

uniform float outlineWidth;
uniform float outlineEdge;
uniform vec3 outlineColor;

uniform vec2 offset;

void main()
{
	float distance = 1.0 - texture(fontAtlas, TexCoord).a;
	float alpha = 1.0 - smoothstep(width, width + edge, distance);

	float outlineDistance = 1.0 - texture(fontAtlas, TexCoord + offset).a;
	float outlineAlpha = 1.0 - smoothstep(outlineWidth, outlineWidth + outlineEdge, outlineDistance);

	float overallAlpha = alpha + (1.0 - alpha) * outlineAlpha;
	vec3 overallColor = mix(outlineColor, color.xyz, alpha / overallAlpha);

	if (overallAlpha <= 0.1)
		discard;
	FragColor = vec4(overallColor, overallAlpha);
}

#endif