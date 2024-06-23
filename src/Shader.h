#pragma once

#include <string>
#include <unordered_map>

using std::string;

struct ShaderProgramSource {
    string vertexSource;
    string fragmentSource;
};

class Shader {
private:
    unsigned int m_RendererID;
    string m_Filepath;
    std::unordered_map<string, int> m_UniformLocationCache;
public:
    Shader(const string &filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    // Set uniforms
    void setUniform1i(const string name, int value);
    void setUniform1f(const string name, float value);
    void setUniform4f(const string name, float v1, float v2, float v3, float v4);
private:
    int getUniformLocation(const string name);
    unsigned int compileShader(unsigned int type, const string &source);
    unsigned int createShader(const string &vertexShader, const string &fragmentShader);
    ShaderProgramSource getShaderContentSingleFile(const string &path);
    string getShaderContent(string &path);
};