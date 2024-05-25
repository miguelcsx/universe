/*
 * @path src/graphics/shader.cpp
 * @file shader.cpp
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <glad/glad.h>
#include "../include/graphics/shader.hpp"


Shader::Shader(const char* vertex_path, const char* fragment_path, bool from_path) {
    if (from_path) {
        compile_from_path(vertex_path, fragment_path);
    } else {
        compile(vertex_path, fragment_path, false);
    }
}

Shader::Shader(const char* vertex_source, const char* fragment_source, const std::vector<std::string>& varyings, bool from_path) {
    if (from_path) {
        compile_from_path(vertex_source, fragment_source, varyings);
    } else {
        compile(vertex_source, fragment_source, varyings, false);
    }
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::compile_from_path(const char* vertex_path, const char* fragment_path) {
    // Read the vertex and fragment shader files
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    // Ensure ifstream objects can throw exceptions
    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // Open files
        v_shader_file.open(vertex_path);
        f_shader_file.open(fragment_path);
        std::stringstream v_shader_stream, f_shader_stream;

        // Read file's buffer contents into streams
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();

        // Close file handlers
        v_shader_file.close();
        f_shader_file.close();

        // Convert stream into string and create shader
        std::string const v_shader_code = v_shader_stream.str();
        std::string const f_shader_code = f_shader_stream.str();
        const char* v_shader_code_c = v_shader_code.c_str();
        const char* f_shader_code_c = f_shader_code.c_str();
        compile(v_shader_code_c, f_shader_code_c);
    }
    catch (std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
}

void Shader::compile_from_path(const char* vertex_path, const char* fragment_path, const std::vector<std::string>& varyings) {
    // Read the vertex and fragment shader files
    std::ifstream v_shader_file;
    std::ifstream f_shader_file;

    // Ensure ifstream objects can throw exceptions
    v_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    f_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        // Open files
        v_shader_file.open(vertex_path);
        f_shader_file.open(fragment_path);
        std::stringstream v_shader_stream, f_shader_stream;

        // Read file's buffer contents into streams
        v_shader_stream << v_shader_file.rdbuf();
        f_shader_stream << f_shader_file.rdbuf();

        // Close file handlers
        v_shader_file.close();
        f_shader_file.close();

        // Convert stream into string and create shader
        std::string const v_shader_code = v_shader_stream.str();
        std::string const f_shader_code = f_shader_stream.str();
        const char* v_shader_code_c = v_shader_code.c_str();
        const char* f_shader_code_c = f_shader_code.c_str();
        compile(v_shader_code_c, f_shader_code_c, varyings);
    }
    catch (std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
}

void Shader::compile(const char* vertex_source, const char* fragment_source, bool from_path) {
    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_source, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_source, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");

    // Shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);
    glLinkProgram(id);
    check_compile_errors(id, "PROGRAM");

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::compile(const char* vertex_source, const char* fragment_source, const std::vector<std::string>& varyings, bool from_path) {
    unsigned int vertex, fragment;
    int success;
    char info_log[512];

    // Vertex Shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertex_source, NULL);
    glCompileShader(vertex);
    check_compile_errors(vertex, "VERTEX");

    // Fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragment_source, NULL);
    glCompileShader(fragment);
    check_compile_errors(fragment, "FRAGMENT");

    // Shader Program
    id = glCreateProgram();
    glAttachShader(id, vertex);
    glAttachShader(id, fragment);

    // Transform feedback varyings
    std::array<const char*, 2> varyings_c;
    for (int i = 0; i < varyings.size(); i++) {
        varyings_c[i] = varyings[i].c_str();
    }
    glTransformFeedbackVaryings(id, varyings.size(), varyings_c.data(), GL_INTERLEAVED_ATTRIBS);

    glLinkProgram(id);
    check_compile_errors(id, "PROGRAM");

    // Delete the shaders as they're linked into our program now and no longer necessary
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::check_compile_errors(unsigned int shader, const std::string& type) {
    int success;
    char info_log[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, info_log);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << info_log << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, info_log);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << info_log << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}

void Shader::use() const {
    glUseProgram(id);
}

unsigned int Shader::get_id() const {
    return id;
}

void Shader::set_bool(const std::string& name, bool value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), (int)value);
}

void Shader::set_int(const std::string& name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void Shader::set_mat4(const std::string& name, const glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
