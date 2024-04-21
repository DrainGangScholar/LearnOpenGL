#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#define GL_VERSION_MAJOR 4
#define GL_VERSION_MINOR 6
#define WIDTH 800
#define HEIGHT 800
#define WINDOW_TITLE "OPEN GL BOSS"
std::string read_shader(const std::string &path) {
  std::ifstream file(path);
  if (!file.is_open()) {
    std::cerr << "Failed to open file: " << path << std::endl;
    return "";
  }
  std::stringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

int glfw_init(int gl_major, int gl_minor) {
  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, gl_major);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, gl_minor);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  return 0;
}
GLFWwindow *create_window(int width, int height, char *title) {
  GLFWwindow *window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (window == nullptr) {
    std::cerr << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return nullptr;
  }
  std::cout << "Successfully created a GLFW window" << std::endl;
  return window;
}
int init_glad() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  std::cout << "Successfully initialized GLAD" << std::endl;
  return 0;
}
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}
void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }
}
int main() {
  glfw_init(GL_VERSION_MAJOR, GL_VERSION_MINOR);
  GLFWwindow *window = create_window(WIDTH, HEIGHT, (char *)WINDOW_TITLE);
  if (window == nullptr) {
    return -1;
  }
  glfwMakeContextCurrent(window);
  if (init_glad()) {
    return -1;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);

  glBindVertexArray(VAO);

  float vertices[] = {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.0f};
  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  auto vs = read_shader("./shaders/vs.glsl/");
  const char *vertex_shader_source = vs.c_str();
  if (vs.empty()) {
    std::cerr << "Couldn't load vertex shader" << std::endl;
    return -1;
  }
  unsigned int vertex_shader;
  vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_shader_source, nullptr);

  int success;
  char info_log[512];
  glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
  glCompileShader(vertex_shader);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
    std::cerr << "SHADER::VERTERX::COMPILATION_FAILED\n"
              << info_log << std::endl;
    return -1;
  }

  std::fill(info_log, info_log + 512, 0);

  unsigned int fragment_shader;
  auto fs = read_shader("./shaders/fs.glsl/");
  const char *fragment_shader_source = fs.c_str();
  if (fs.empty()) {
    std::cerr << "Couldn't load fragment shader" << std::endl;
    return -1;
  }
  fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_shader_source, nullptr);

  glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
  glCompileShader(fragment_shader);
  if (!success) {
    glGetShaderInfoLog(vertex_shader, 512, nullptr, info_log);
    std::cerr << "SHADER::FRAGMENT::COMPILATION_FAILED\n"
              << info_log << std::endl;
    return -1;
  }

//  glBindBuffer(GL_ARRAY_BUFFER, VBO);
//  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int shader_program;
  shader_program = glCreateProgram();
  glAttachShader(shader_program, vertex_shader);
  glAttachShader(shader_program, fragment_shader);
  glLinkProgram(shader_program);

  std::fill(info_log, info_log + 512, 0);

  glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragment_shader, 512, nullptr, info_log);
    std::cerr << "SHADER::PROGRAM::COMPILATION_FAILED\n"
              << info_log << std::endl;
    return -1;
  }
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glUseProgram(shader_program);
  glBindVertexArray(VAO);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }

  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  glfwTerminate();

  return 0;
}
