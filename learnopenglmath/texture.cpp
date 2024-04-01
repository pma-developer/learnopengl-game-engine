
#include <iostream>
#include <string>
#include <stb_image.h>
#include "texture.h"

Texture::Texture(GLuint textureUnit, GLuint textureType) : textureUnit(textureUnit), textureType(textureType)
{
    textureId = genTexture();
}
Texture::~Texture()
{
    glDeleteTextures(1, &textureId);
}

void Texture::bind() const
{
    glActiveTexture(textureUnit);
    glBindTexture(textureType, textureId);
}

GLuint Texture::genTexture() const
{

    GLint prevTextureUnit;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &prevTextureUnit);

    glActiveTexture(textureUnit);
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(textureType, texture);

    glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTexture(prevTextureUnit);

    return texture;
}

void Texture::setImage2DFromFile(std::string path) const
{
    if (textureType != GL_TEXTURE_2D)
    {
        std::cout << "Error, texture type should be GL_TEXTURE_2D. Actual textureType: " << textureType << std::endl;
        return;
    }
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    setImageData(width, height, nrChannels, data);
    stbi_image_free(data);
}

void Texture::setImageData(int width, int height, int nrChannels, unsigned char* data) const
{
    GLenum format = 0;
    GLint internalFormat = 0;

    GLint currentActiveTexture;
    glGetIntegerv(GL_ACTIVE_TEXTURE, &currentActiveTexture);

    glActiveTexture(textureUnit);
    glBindTexture(textureType, textureId);

    switch (nrChannels)
    {
    case 1:
        format = GL_RED;
        internalFormat = GL_RED;
        break;
    case 2:
        format = GL_RG;
        internalFormat = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        internalFormat = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        internalFormat = GL_RGBA;
        break;
    }
    glTexImage2D(textureType, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(textureType);

    glBindTexture(textureType, 0);
}