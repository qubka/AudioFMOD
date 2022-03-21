#include "shader.hpp"
#include "opengl.hpp"

Shader::Shader() : programId{glCall_(glCreateProgram)} {
}

Shader::~Shader() {
    glCall(glDeleteProgram, programId);
}

void Shader::use() const {
    glCall(glUseProgram, programId);
}

void Shader::unuse() const {
    glCall(glUseProgram, 0);
}

bool Shader::link(const std::string& vertexPath, const std::string& fragmentPath, const std::string& tessControlPath, const std::string& tessEvalPath) const {
    std::vector<GLuint> shaderIds; // for cleanup

    bool success;

    shaderIds.push_back(createShader(ReadFile(vertexPath), GL_VERTEX_SHADER, success));
    if (!success) return false;

    shaderIds.push_back(createShader(ReadFile(fragmentPath), GL_FRAGMENT_SHADER, success));
    if (!success) return false;

    if (!tessControlPath.empty()) {
        shaderIds.push_back(createShader(ReadFile(tessControlPath), GL_TESS_CONTROL_SHADER, success));
        if (!success) return false;
    }

    if (!tessEvalPath.empty()) {
        shaderIds.push_back(createShader(ReadFile(tessEvalPath), GL_TESS_EVALUATION_SHADER, success));
        if (!success) return false;
    }

    glCall(glLinkProgram, programId);

#ifndef NDEBUG
    GLint status;

    glCall(glGetProgramiv, programId, GL_LINK_STATUS, &status);
    if (!status) {
        GLint length;
        glCall(glGetProgramiv, programId, GL_INFO_LOG_LENGTH, &length);
        std::string info(length, ' ');
        glCall(glGetProgramInfoLog, programId, info.length(), &length, info.data());
        std::cerr << "ERROR: Linking Program: " << std::endl;
        std::cerr << info << std::endl;
        return false;
    }
#endif

    for (auto shaderId : shaderIds) {
        glCall(glDetachShader, programId, shaderId);
    }

#ifndef NDEBUG
    glCall(glValidateProgram, programId);

    glCall(glGetProgramiv, programId, GL_VALIDATE_STATUS, &status);
    if (!status) {
        GLint length;
        glCall(glGetProgramiv, programId, GL_INFO_LOG_LENGTH, &length);
        std::string info(length, ' ');
        glCall(glGetProgramInfoLog, programId, info.length(), &length, info.data());
        std::cerr << "ERROR: Validating Program: " << std::endl;
        std::cerr << info << std::endl;
        return false;
    }
#endif

    return true;
}

GLuint Shader::createShader(const std::string& shaderCode, GLenum shaderType, bool& success) const {
    GLuint shaderId = glCall(glCreateShader, shaderType);
    if (!shaderId) {
        std::cerr << "ERROR: creating shader. Type: ";
        switch (shaderType) {
            case GL_FRAGMENT_SHADER:
                std::cerr << "GL_FRAGMENT_SHADER";
                break;
            case GL_VERTEX_SHADER:
                std::cerr << "GL_VERTEX_SHADER";
                break;
            case GL_TESS_CONTROL_SHADER:
                std::cerr << "GL_TESS_CONTROL_SHADER";
                break;
            case GL_TESS_EVALUATION_SHADER:
                std::cerr << "GL_TESS_EVALUATION_SHADER";
                break;
            default:
                std::cerr << "UNKNOWN GL SHADER";
                break;
        }
        std::cerr << std::endl;
        success = false;
        return shaderId;
    }

    const GLchar* code = shaderCode.c_str();
    glCall(glShaderSource, shaderId, 1, &code, nullptr);
    glCall(glCompileShader, shaderId);

#ifndef NDEBUG
    GLint status;
    glCall(glGetShaderiv, shaderId, GL_COMPILE_STATUS, &status);
    if (!status) {
        GLint length;
        glCall(glGetShaderiv, shaderId, GL_INFO_LOG_LENGTH, &length);
        std::string info(length, ' ');
        glCall(glGetShaderInfoLog, shaderId, info.length(), &length, info.data());
        std::cerr << "ERROR: Compiling Shader: " << std::endl;
        std::cerr << info << std::endl;
        success = false;
        return shaderId;
    }
#endif

    glCall(glAttachShader, programId, shaderId);
    success = true;
    return shaderId;
}

void Shader::setUniform(const std::string& name, int value) const {
    glCall(glUniform1i, findUniform(name), value);
}

void Shader::setUniform(const std::string& name, float value) const {
    glCall(glUniform1f, findUniform(name), value);
}

void Shader::setUniform(const std::string& name, const glm::vec2& value) const {
    glCall(glUniform2f, findUniform(name), value.x, value.y);
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) const {
    glCall(glUniform3f, findUniform(name), value.x, value.y, value.z);
}

void Shader::setUniform(const std::string& name, const glm::vec4& value) const {
    glCall(glUniform4f, findUniform(name), value.x, value.y, value.z, value.w);
}

void Shader::setUniform(const std::string& name, const glm::mat2& value) const {
    glCall(glUniformMatrix2fv, findUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat2& value, int count) const {
    glCall(glUniformMatrix2fv, findUniform(name), count, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat3& value) const {
    glCall(glUniformMatrix3fv, findUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat3& value, int count) const {
    glCall(glUniformMatrix3fv, findUniform(name), count, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat4& value) const {
    glCall(glUniformMatrix4fv, findUniform(name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat4& value, int count) const {
    glCall(glUniformMatrix4fv, findUniform(name), count, GL_FALSE, glm::value_ptr(value));
}

GLint Shader::findUniform(const std::string& name) const {
    GLint uniformLocation = glCall(glGetUniformLocation, programId, name.c_str());
    if (uniformLocation < 0) {
        std::cerr << "ERROR: Could not find uniform: " << name << std::endl;
    }
    return uniformLocation;
}

std::string Shader::ReadFile(const std::string& path) {
    assert(std::filesystem::exists(path) && "Could not load file");

    std::ifstream in{path};
    if (!in.is_open()) {
        std::cerr << "ERROR: Cannot opened file: " << path << std::endl;
        return "";
    }

    std::stringstream stream;
    stream << in.rdbuf();
    in.close();
    return stream.str();
}
