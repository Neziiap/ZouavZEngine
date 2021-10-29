#pragma once

#include <glad/glad.h>

class Framebuffer
{
private:
	unsigned int id = 0;
	unsigned int texture = 0;
	unsigned int depthRenderbuffer = 0;
	int width = 0;
	int height = 0;
public:
	Framebuffer() {};
	~Framebuffer();
	
	inline unsigned int getId() { return id; };
	inline unsigned int getTexture() { return texture; };
	inline unsigned int getDepthRenderBuffer() { return depthRenderbuffer; };
	inline int getWidth() { return width; };
	inline int getHeight() { return height; };

	void Generate(int _width, int _height, int _internalFormat, GLenum _type);
	void Resize(int _width, int _height);
	void Delete();
};