#include "includes.h"
class Shader {
private:
  unsigned int vertex_shader, fragment_shader;

public:
  unsigned int ID;

  Shader() {
    vertex_shader = init_shader(GL_VERTEX_SHADER, "../shaders/vs.glsl");
    fragment_shader = init_shader(GL_FRAGMENT_SHADER, "../shaders/fs.glsl");

    ID = glCreateProgram();
    glAttachShader(ID, vertex_shader);
    glAttachShader(ID, fragment_shader);
    glLinkProgram(ID);

    glUseProgram(ID);
  }
  std::string read_shader(const std::string &path) {
    std::ifstream file(path);
    if (!file.is_open()) {
      std::cerr << "Failed to open file: " << path << std::endl;
      return "";
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
  }
  unsigned int init_shader(GLuint shader_type, std::string path) {
    auto s = read_shader(path);
    const char *shader_content = s.c_str();
    if (s.empty()) {
      std::cerr << "Couldn't load shader with path" << path << std::endl;
      return 0;
    }
    unsigned int shader = glCreateShader(shader_type);
    GLint failure;
    glShaderSource(shader, 1, &shader_content, nullptr);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &failure);
    glCompileShader(shader);
    if (failure) {
      glGetShaderInfoLog(shader, 0, nullptr, nullptr);
      std::cerr << "SHADER::VERTERX::COMPILATION_FAILED\n" << std::endl;
      return -1;
    }
    return shader;
  }
  inline void use() { glUseProgram(ID); }
  inline void terminate() {
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);
  }
};
