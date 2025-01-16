#pragma once

#include <string>

class Shader final {
public:
    unsigned int program;

    Shader(const char* vertexShader, const char* fragmentShader, const char* geometryShader = nullptr);
    ~Shader();

    void bind() const;
    [[nodiscard]] unsigned int getProgram() const;
    void setInt(const std::string &uniform, int value) const;
    void setBool(const std::string &uniform, bool value) const;
    void setFloat(const std::string &uniform, float value) const;
};