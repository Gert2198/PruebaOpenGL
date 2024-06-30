#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

using std::string;

struct ShaderProgramSource {
    string vertexSource;
    string fragmentSource;
};

class Shader {
private:
    inline static unsigned int m_boundShader = 0;
    inline static std::unordered_map<unsigned int, int> m_repetitions = {};
    unsigned int m_RendererID;
    string m_Filepath;
    std::unordered_map<string, int> m_UniformLocationCache;
public:
    Shader(const string &filepath);
    Shader(const Shader& shader);
    ~Shader();

    void bind() const;
    void unbind() const;

    // Set uniforms
    void setUniform1i(const string &name, int value);
    void setUniform1f(const string &name, float value);
    
    void setUniform2f(const string &name, float v1, float v2);
    void setUniform2f(const string &name, const glm::vec2& value);

    void setUniform3f(const string &name, float v1, float v2, float v3);
    void setUniform3f(const string &name, const glm::vec3& value);
    
    void setUniform4f(const string &name, float v1, float v2, float v3, float v4);
    void setUniform4f(const string &name, const glm::vec4& value);

    void setUniformMat3f(const string &name, const glm::mat3& matrix);
    void setUniformMat4f(const string &name, const glm::mat4& matrix);
private:
    int getUniformLocation(const string name);
    unsigned int compileShader(unsigned int type, const string &source);
    unsigned int createShader(const string &vertexShader, const string &fragmentShader);
    ShaderProgramSource getShaderContentSingleFile(const string &path);
    string getShaderContent(string &path);
};