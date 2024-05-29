/**
 * @path include/graphics/shader.hpp
 * @file shader.hpp
*/

#ifndef SHADER_HPP
#define SHADER_HPP

#include <vector>
#include <string>
#include <glm/glm.hpp>

class Shader {
    public:
        Shader(const char* vertex_path, const char* fragment_path, bool from_path = true);
        Shader(const char* vertex_source, const char* fragment_source, const std::vector<std::string>& varyings, bool from_path = true);
        Shader(const Shader&) = delete;
        Shader(Shader&&) = delete;
        Shader& operator=(const Shader&) = delete;
        Shader& operator=(Shader&&) = delete;

        virtual ~Shader();

        void compile_from_path(const char* vertex_path, const char* fragment_path);
        void compile_from_path(const char* vertex_path, const char* fragment_path, const std::vector<std::string>& varyings);
        void compile(const char* vertex_source, const char* fragment_source, bool from_path = true);
        void compile(const char* vertex_source, const char* fragment_source, const std::vector<std::string>& varyings, bool from_path = true);

        static void check_compile_errors(unsigned int shader, const std::string& type);

        void use() const;
        [[maybe_unused]] [[nodiscard]] unsigned int get_id() const;

        [[maybe_unused]] void set_bool(const std::string& name, bool value) const;
        [[maybe_unused]] void set_int(const std::string& name, int value) const;
        [[maybe_unused]] void set_mat4(const std::string& name, const glm::mat4& mat) const;

    private:
        u_int id;
};

#endif // SHADER_HPP