#ifdef COMPILING_VS

layout (location = 0) in vec3 aPos;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 projection;
uniform mat4 view;

void main()
{
	TexCoord = aTexCoord;

	gl_Position = projection * vec4(aPos, 1.0);
}

#endif

#ifdef COMPILING_FS


out vec4 FragColor;

in vec2 TexCoord;

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