#ifdef COMPILING_VS

layout(location = 0) in vec3 aPosition;

out vec3 vPosition;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    vPosition = aPosition;
    gl_Position = projection * mat4(mat3(view)) * vec4(aPosition, 1.0);
}

#endif

#ifdef COMPILING_FS

in vec3 vPosition;

out vec4 fragColor;

uniform samplerCube cubemap;

void main()
{
    fragColor = texture(cubemap, vPosition);
}

#endif