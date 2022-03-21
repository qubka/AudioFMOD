#pragma once

class Shader {
public:
    Shader();
    ~Shader();

    void setUniform(const std::string& name, int value) const;
    void setUniform(const std::string& name, float value) const;
    void setUniform(const std::string& name, const glm::vec2& value) const;
    void setUniform(const std::string& name, const glm::vec3& value) const;
    void setUniform(const std::string& name, const glm::vec4& value) const;
    void setUniform(const std::string& name, const glm::mat2& value) const;
    void setUniform(const std::string& name, const glm::mat2& value, int count) const;
    void setUniform(const std::string& name, const glm::mat3& value) const;
    void setUniform(const std::string& name, const glm::mat3& value, int count) const;
    void setUniform(const std::string& name, const glm::mat4& value) const;
    void setUniform(const std::string& name, const glm::mat4& value, int count) const;

    bool link(const std::string& vertexPath,
              const std::string& fragmentPath,
              const std::string& tessControlPath = "",
              const std::string& tessEvalPath = "") const;

    void use() const;
    void unuse() const;

private:
    GLuint programId;

    GLint findUniform(const std::string& name) const;
    GLuint createShader(const std::string& shaderCode, GLenum shaderType, bool& success) const;
    static std::string ReadFile(const std::string& path);
};
