#pragma once

#include <string>

#include "Renderer.h"

using std::string;

struct ShaderProgramSource {
    string vertexSource;
    string fragmentSource;
};

class Shader {
private:
    unsigned int m_RendererID;
    string m_Filepath;
    // caching for uniforms
public:
    Shader(const string &filepath);
    ~Shader();

    void bind() const;
    void unbind() const;

    // Set uniforms
    void setUniform4f(const string name, float v1, float v2, float v3, float v4);
private:
    int getUniformLocation(const string name);
    unsigned int compileShader(unsigned int type, const string &source);
    unsigned int createShader(const string &vertexShader, const string &fragmentShader);
    ShaderProgramSource getShaderContentSingleFile(const string &path);
    string getShaderContent(string &path);
};