#include "Shader.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const string &filepath) : m_RendererID(0), m_Filepath(filepath) {
    ShaderProgramSource source = getShaderContentSingleFile(filepath);
    m_RendererID = createShader(source.vertexSource, source.fragmentSource);
    if (m_repetitions.find(m_RendererID) == m_repetitions.end())
        m_repetitions[m_RendererID] = 1;
    else 
        m_repetitions[m_RendererID]++;
}

Shader::Shader(const Shader& shader){
    m_RendererID = shader.m_RendererID;
    m_Filepath = shader.m_Filepath;
    m_UniformLocationCache = shader.m_UniformLocationCache;
    m_repetitions[m_RendererID]++;
}

Shader::~Shader(){
    m_repetitions[m_RendererID]--;
    if (m_repetitions[m_RendererID] == 0) {
        GLDebug(glDeleteProgram(m_RendererID));
    }
}

void Shader::bind() const {
    if (m_boundShader != m_RendererID) {
        GLDebug(glUseProgram(m_RendererID));
        m_boundShader = m_RendererID;
    }
}
void Shader::unbind() const {
    if (m_boundShader != 0) {
        GLDebug(glUseProgram(0));
        m_boundShader = 0;    
    }
}

void Shader::setUniform1i(const string &name, int value) {
    GLDebug(glUniform1i(getUniformLocation(name), value));
}

void Shader::setUniform1f(const string &name, float value) {
    GLDebug(glUniform1f(getUniformLocation(name), value));
}

void Shader::setUniform4f(const string &name, float v1, float v2, float v3, float v4) {
    GLDebug(glUniform4f(getUniformLocation(name), v1, v2, v3, v4));
}

void Shader::setUniformMat4f(const string &name, glm::mat4 &projMatrix) {
    GLDebug(glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &projMatrix[0][0]));
    // CUIDAO: si la forma en la que nuestra libreria de matemáticas implementa las matrices es con vectores POR FILAS, tenemos que transponerla
    // En nuestro caso, glm crea las matrices por columnas, que es justo como las lee OpenGL, por lo que ponemos a FALSE el campo transpose
}

int Shader::getUniformLocation(const string name) {
    if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
        return m_UniformLocationCache[name];

    GLDebug(int location = glGetUniformLocation(m_RendererID, name.c_str()));
    if (location == -1)  // Puede que no exista el uniform en el programa especificado
        std::cerr << "Warning: uniform '" << name << "' does not exist." << std::endl;
    
    m_UniformLocationCache[name] = location;
    return location;
}


ShaderProgramSource Shader::getShaderContentSingleFile(const string &path) {
    std::ifstream stream(path);
    if (!stream.is_open())
        std::cerr << "Failed to open file in path " << path << std::endl;

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