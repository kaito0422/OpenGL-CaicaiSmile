#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glew.h>
#include <glfw3.h>
#include <string>
#include <fstream>
#include <iostream>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#endif

#include "stb_image.h"

class Texture {
public:
	Texture(const std::string &filepath);
	~Texture();

	unsigned int getTextureId() const { return textureId; }

private:
	unsigned int textureId;

};

#endif