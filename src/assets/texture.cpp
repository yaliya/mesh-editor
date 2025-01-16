#include "texture.h"
#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../../vendor/stbi_image.h"

Texture::Texture(const std::string& path) : Asset(path) {
    this->textureType = TextureType::DIFFUSE_MAP;
}

Texture::Texture(const std::string& path, const TextureType type): Asset(path) {
    this->textureType = type;
}

bool Texture::load() {
    glGenTextures(1, &textureId);

    int width, height, channels;
    unsigned char *data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (data) {
        const GLint format = channels == 1 ? GL_RED : channels == 3 ? GL_RGB : channels == 4 ? GL_RGBA : GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureId);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
        return true;
    }

    stbi_image_free(data);
    return false;
}

unsigned int Texture::getTextureId() const {
    return textureId;
}

TextureType Texture::getTextureType() const {
    return textureType;
}

void Texture::setTextureType(const TextureType type) {
    this->textureType = type;
}