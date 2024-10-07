#include "Shader.h"

std::string get_file_contents(const char* filename)
{
    std::ifstream in(filename, std::ios::binary);
    if (in)
    {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    throw(errno);
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode = get_file_contents(vertexPath);
    std::string fragmentCode = get_file_contents(fragmentPath);

    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
    compileErrors(vertexShader, "VERTEX");

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    compileErrors(vertexShader, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);

    glLinkProgram(ID);
    compileErrors(vertexShader, "PROGRAM");
}

void Shader::Use()
{
    glUseProgram(ID);
}

void Shader::Delete()
{
    glDeleteProgram(ID);
}

void Shader::compileErrors(unsigned int shader, const char* type)
{
    GLint status;
    char log[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE)
        {
            glGetShaderInfoLog(shader, 1024, NULL, log);
            std::cout << "SHADER_COMPILATION_ERROR for: " << type << "\n" << log;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            glGetProgramInfoLog(shader, 1024, NULL, log);
            std::cout << "SHADER_LINK_ERROR for: " << type << "\n" << log;
        }
    }

}
