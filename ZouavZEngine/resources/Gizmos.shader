#ifdef COMPILING_VS
layout (location = 0) in vec3 aPos;

uniform mat4 view;
uniform mat4 projection;
uniform mat4 matrix;
uniform vec4 color;

out vec3 Pos;
out vec4 Color;

void main()
{
	Color = color;
	Pos = vec3(matrix * vec4(aPos, 1.0));

	gl_Position = projection * view * vec4(Pos, 1.0);
}
#endif

#ifdef COMPILING_FS


in vec3 Pos;
in vec4 Color;

out vec4 FragColor;

void main()
{
	FragColor = Color;
}

#endif