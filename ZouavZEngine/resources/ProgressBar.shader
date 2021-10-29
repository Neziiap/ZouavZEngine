#ifdef COMPILING_VS

layout (location = 0) in vec3 aPos;

uniform mat4 projection;

void main()
{
	gl_Position = projection * vec4(aPos.x, aPos.y, 1.0, 1.0);
}

#endif

#ifdef COMPILING_FS

out vec4 FragColor;

uniform vec4 color;

void main()
{
    FragColor = color;
}

#endif