#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const string &filepath) : m_RendererID(0), m_Filepath(filepath) {
    ShaderProgramSource source = getShaderContentSingleFile(filepath);
    m_RendererID = createShader(source.vertexSource, source.fragmentSource);
}

Shader::~Shader(){
    GLDebug(glDeleteProgram(m_RendererID));
}

void Shader::bind() const {
    GLDebug(glUseProgram(m_RendererID));
}
void Shader::unbind() const {
    GLDebug(glUseProgram(0));
}

void Shader::setUniform4f(const string name, float v1, float v2, float v3, float v4) {
    GLDebug(glUniform4f(getUniformLocation(name), v1, v2, v3, v4));
}

int Shader::getUniformLocation(const string name) {
    GLDebug(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)  // Puede que no exista el uniform en el programa especificado
        std::cerr << "Warning: uniform '" << name << "' does not exist." << std::endl;
    return location;
}


ShaderProgramSource Shader::getShaderContentSingleFile(const string &path) {
    std::ifstream stream(path);

    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line)) {
        if (line.find("#shader") != string::npos) {
            if (line.find("vertex") != string::npos) {
                type = ShaderType::VERTEX;
            } else if (line.find("fragment") != string::npos) {
                type = ShaderType::FRAGMENT;
            }
        } else {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

string Shader::getShaderContent(string &path) {
    std::ifstream file(path);
    string str;
    string content;
    while (getline(file, str)) {
        content.append(str + "\n");
    }
    return content;
}

unsigned int Shader::compileShader(unsigned int type, const string &source) {
    GLDebug(unsigned int id = glCreateShader(type));
    const char* src = source.c_str();
    GLDebug(glShaderSource(id, 1, &src, nullptr));
    GLDebug(glCompileShader(id));

    // Manejo de errores
    int result;
    GLDebug(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE) {
        int length;
        GLDebug(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*) alloca(length * sizeof(char)); // En vez de usar new y que no esté en la pila, de esta forma el mensaje se mantiene en la pila
        GLDebug(glGetShaderInfoLog(id, length, &length, message));
        std::cerr << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")  << " shader.\nError: " << message << std::endl;
        GLDebug(glDeleteShader(id));
        return 0;
    }

    return id;
}

unsigned int Shader::createShader(const string &vertexShader, const string &fragmentShader) {
    GLDebug(unsigned int program = glCreateProgram());
    unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);
    
    GLDebug(glAttachShader(program, vs));
    GLDebug(glAttachShader(program, fs));
    GLDebug(glLinkProgram(program));
    GLDebug(glValidateProgram(program));

    GLDebug(glDeleteShader(vs));
    GLDebug(glDeleteShader(fs));

    return program;
}