#pragma once

#include <string>
#include "asset.h"

enum TextureType
{
    DIFFUSE_MAP,
    NORMAL_MAP,
    SPECULAR_MAP
};

class Texture final : public Asset {
    unsigned int textureId{};
    TextureType textureType;
public:
    explicit Texture(const std::string& path);
    Texture(const std::string& path, TextureType type);

    bool load() override;
    void setTextureType(TextureType type);

    [[nodiscard]] unsigned int getTextureId() const;
    [[nodiscard]] TextureType getTextureType() const;
};