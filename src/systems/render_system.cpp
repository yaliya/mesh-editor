#include "render_system.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../window.h"
#include "../managers/event_manager.h"
#include "../core/state/edge_selection_state.h"
#include "../core/state/face_selection_state.h"
#include "../core/event/scene_rendered_event.h"
#include "../core/state/mesh_selection_state.h"
#include "../core/state/vertex_selection_state.h"

RenderSystem::RenderSystem(const std::shared_ptr<Window>& window) : BaseSystem(window) {
    const auto scene = window->getScene();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_LINE_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

    glEnable(GL_POINT_SMOOTH);
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    //glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    // Load shaders
    projectionShader = std::make_shared<Shader>(
        "data/shaders/projection_vertex_shader.glsl",
        "data/shaders/projection_fragment_shader.glsl"
    );

    screenShader = std::make_shared<Shader>(
        "data/shaders/screen_vertex_shader.glsl",
        "data/shaders/screen_fragment_shader.glsl"
    );

    // Set up quad vertices for fullscreen quad rendering
    quadVertices = {
        -1.0f,  1.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 0.0f,

        -1.0f,  1.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        1.0f,  1.0f, 1.0f, 1.0f
    };

    // Set up the fullscreen quad VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(quadVertices.size() * sizeof(float)), &quadVertices.front(), GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), static_cast<void *>(nullptr));

    // glEnableVertexAttribArray(0);
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glBindVertexArray(0);

    this->reload();
}

void RenderSystem::reload() {
    const auto scene = window->getScene();
    const auto camera = scene->getCamera();

    // Generate framebuffer
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    // Create MultiSampled texture attachment
    unsigned int textureColorBufferMultiSampled;
    glGenTextures(1, &textureColorBufferMultiSampled);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 8, GL_RGB, camera->viewWidth, camera->viewHeight, GL_TRUE);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, textureColorBufferMultiSampled, 0);

    // Create MultiSampled render buffer object
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 8, GL_DEPTH24_STENCIL8, camera->viewWidth, camera->viewHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    // configure second post-processing framebuffer for depth and stencil
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);

    // Create target render texture
    glGenTextures(1, &renderedTexture);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, camera->viewWidth, camera->viewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);
    //glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, renderedTexture, 0);

    // // Create renderbuffer for depth and stencil
    // glGenRenderbuffers(1, &renderBuffer);
    // glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,camera->viewWidth, camera->viewHeight);
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

    // Unbind framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderSystem::update(const double &deltaTime) {
    const auto scene = window->getScene();

    // // 1. draw scene as normal in multi-sampled buffers
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    // 2. Bind the projection shader and set uniforms
    projectionShader->bind();

    const auto grid = scene->getGrid();
    const auto camera = scene->getCamera();

    // Set light properties
    glm::vec3 lightDir = glm::normalize(-camera->position);
    glUniform3f(glGetUniformLocation(projectionShader->getProgram(), "lightDir"), lightDir.x, lightDir.y, lightDir.z);
    glUniform3f(glGetUniformLocation(projectionShader->getProgram(), "lightColor"), 1.0f, 1.0f, 1.0f);

    // Set up camera transformations
    glm::mat4 modelMatrix = glm::mat4(1.0f); // Identity matrix for model
    glm::mat4 view = glm::lookAt(camera->position, camera->position + camera->front, camera->up);
    glm::mat4 projection = glm::perspective(camera->fov, static_cast<float>(camera->viewWidth) / static_cast<float>(camera->viewHeight), camera->near, camera->far);

    // Pass matrices to shader
    glUniform1i(glGetUniformLocation(projectionShader->getProgram(), "useLighting"), GL_FALSE);
    glUniformMatrix4fv(glGetUniformLocation(projectionShader->getProgram(), "view"), 1, GL_FALSE, &view[0][0]);
    //glUniformMatrix4fv(glGetUniformLocation(projectionShader->getProgram(), "model"), 1, GL_FALSE, &modelMatrix[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(projectionShader->getProgram(), "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // 3. Render scene
    int counter = 0;
    for (const auto& model : scene->getModels()) {
        //glUniformMatrix4fv(glGetUniformLocation(projectionShader->getProgram(), "model"), 1, GL_FALSE, &modelMatrix[0][0]);

        for(auto& mesh : model->getMeshes()) {
            glUniform1i(glGetUniformLocation(projectionShader->getProgram(), "useLighting"), GL_TRUE);
            glUniform1i(glGetUniformLocation(projectionShader->getProgram(), "useMaterial"), !mesh.textures.empty() && scene->texturesEnabled);
            glUniform3f(glGetUniformLocation(projectionShader->getProgram(), "objectColor"), 0.65f, 0.65f, 0.65f); // Grey default color
            // Set model matrix for mesh
            glUniformMatrix4fv(glGetUniformLocation(projectionShader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(mesh.transform.getTransform()));


            for (const auto& texture : mesh.textures) {
                glActiveTexture(GL_TEXTURE0 + counter);

                if (texture->getTextureType() == TextureType::DIFFUSE_MAP) {
                    glUniform1i(glGetUniformLocation(projectionShader->getProgram(), "material.diffuse"), counter);
                }
                else if (texture->getTextureType() == TextureType::SPECULAR_MAP) {
                    glUniform1i(glGetUniformLocation(projectionShader->getProgram(), "material.specular"), counter);
                }

                counter++;
                glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
            }

            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(.5f, .5f);
            glDepthFunc(GL_LESS);

            glBindVertexArray(mesh.vao);
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indices.size()), GL_UNSIGNED_INT, nullptr);
            glBindVertexArray(0);

            glDisable(GL_POLYGON_OFFSET_FILL);
            glDepthFunc(GL_LEQUAL);

            glUniform1i(glGetUniformLocation(projectionShader->getProgram(), "useLighting"), GL_FALSE);
            if (scene->getSelectionMode() == SelectionMode::FACES || scene->getSelectionMode() == SelectionMode::VERTICES || scene->getSelectionMode() == SelectionMode::EDGES) {
                glEnable(GL_POLYGON_OFFSET_LINE);
                glPolygonOffset(0.3f, 0.3f);
                glDepthFunc(GL_LESS);

                // Draw edges
                glUniform3f(glGetUniformLocation(projectionShader->getProgram(), "objectColor"), 0.3f, 0.3f, 0.3f); // Black color for edges

                glLineWidth(1.0f);
                glBindVertexArray(mesh.edgeVao);
                glDrawElements(GL_LINES, static_cast<GLsizei>(mesh.edges.size()), GL_UNSIGNED_INT, nullptr);
                glBindVertexArray(0);

                glDisable(GL_POLYGON_OFFSET_LINE);
                glDepthFunc(GL_LEQUAL);
            }

            if (scene->getSelectionMode() == SelectionMode::VERTICES) {
                glEnable(GL_POLYGON_OFFSET_POINT);
                glPolygonOffset(0.1f, 0.1f);
                glDepthFunc(GL_LESS);

                glUniform3f(glGetUniformLocation(projectionShader->getProgram(), "objectColor"), 0.1f, 0.1f, 0.1f);
                // Draw other vertices
                glPointSize(5.0f);
                glBindVertexArray(mesh.vao);
                glDrawArrays(GL_POINTS, 0, static_cast<GLsizei>(mesh.vertices.size()));
                glBindVertexArray(0);

                glDisable(GL_POLYGON_OFFSET_POINT);
                glDepthFunc(GL_LEQUAL);
            }
        }
    }

    glUniform1i(glGetUniformLocation(projectionShader->getProgram(), "useLighting"), GL_FALSE);

    // Render grid
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(0.3f, 0.3f);

    glLineWidth(1.0f);
    glBindVertexArray(grid->getMeshes().front().vao);
    glUniformMatrix4fv(glGetUniformLocation(projectionShader->getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    glUniform1i(glGetUniformLocation(projectionShader->getProgram(), "useMaterial"), GL_FALSE);
    glUniform3f(glGetUniformLocation(projectionShader->getProgram(), "objectColor"), 0.8f, 0.8f, 0.8f);
    glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(grid->getMeshes().front().indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glDisable(GL_POLYGON_OFFSET_FILL);

    glBindVertexArray(grid->getMeshes().front().vao);
    glUniform1i(glGetUniformLocation(projectionShader->getProgram(), "useMaterial"), GL_FALSE);
    glUniform3f(glGetUniformLocation(projectionShader->getProgram(), "objectColor"), 0.7f, 0.7f, 0.7f);
    glDrawElements(GL_LINES, static_cast<GLsizei>(grid->getMeshes().front().indices.size()), GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);

    glUniform3f(glGetUniformLocation(projectionShader->getProgram(), "objectColor"), 1.0f, 0.0f, 0.0f); // Red color for selections

    if (scene->getSelectionMode() == SelectionMode::VERTICES) {
        for(const auto& vertex : VertexSelectionState::get()) {
            VertexSelectionState::draw(vertex, MeshSelectionState::get()->transform.transform);
        }
    }

    if (scene->getSelectionMode() == SelectionMode::FACES) {
        for(const auto& face : FaceSelectionState::get()) {
            FaceSelectionState::draw(face);
        }
    }

    if (scene->getSelectionMode() == SelectionMode::EDGES) {
        for(const auto& edge : EdgeSelectionState::get()) {
            EdgeSelectionState::draw(edge);
        }
    }

    // 2. Blit multi-sampled buffer(s) to normal color buffer of intermediate FBO. Image is stored in screenTexture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
    glBlitFramebuffer(0, 0, camera->viewWidth, camera->viewHeight, 0, 0, camera->viewWidth, camera->viewHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    // 4. Unbind the framebuffer to render to the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Clear the default framebuffer
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    // 5. Bind the screen shader for post-processing
    screenShader->bind();

    // Bind the texture from the framebuffer
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    screenShader->setInt("screenTexture", 0);

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    EventManager::dispatch<SceneRenderedEvent>();
}

unsigned int RenderSystem::renderTexture() const {
    return this->renderedTexture;
}

RenderSystem::~RenderSystem() {
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);
    glDeleteFramebuffers(1, &framebuffer);
    glDeleteRenderbuffers(1, &renderBuffer);
}
