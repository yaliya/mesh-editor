#pragma once

#include <vector>
#include <glad/glad.h>

#include "base_system.h"
#include "../util/shader.h"

class RenderSystem final : public BaseSystem {
    GLuint framebuffer{};
    GLuint renderBuffer{};
    GLuint intermediateFBO{};

    unsigned int quadVAO{}, quadVBO{};
    std::shared_ptr<Shader> screenShader;
    std::shared_ptr<Shader> projectionShader;

    unsigned int renderedTexture{};
    unsigned int textureColorBufferMultiSampled{};
    std::vector<float> quadVertices;
public:
    explicit RenderSystem(const std::shared_ptr<Window>&window);
    ~RenderSystem() override;

    void reload();
    void update(const double& deltaTime) override;

    [[nodiscard]] unsigned int renderTexture() const;
};