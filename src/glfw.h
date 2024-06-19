#include "includes.h"
#include <GLFW/glfw3.h>
class glfw {
public:
  GLFWwindow *window;
  int width, height;

public:
  glfw() {}
  void init(int gl_major, int gl_minor, int width, int height,
            const char *title) {
    glfw_init(gl_major, gl_minor);
    window = create_window(width, height, (char *)title);
    if (window == nullptr) {
      std::cerr << "Couldn't create window";
      return;
    }
    glfwMakeContextCurrent(window);
    if (init_glad()) {
      return;
    }
    glViewport(0, 0, 600, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  }

  inline void terminate() { glfwTerminate(); }
  inline int should_close() { return glfwWindowShouldClose(window); }
  inline void process_input() { process_input(window); }

private:
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

  GLFWwindow *create_window(int width, int height, const char *title) {
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
      std::cerr << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
      return nullptr;
    }
    std::cout << "Successfully created a GLFW window" << std::endl;
    return window;
  }

  int init_glad() {
    if (!gladLoadGLLoader((GLADloadproc)(glfwGetProcAddress))) {
      std::cerr << "Failed to initialize GLAD" << std::endl;
      return -1;
    }
    std::cout << "Successfully initialized GLAD" << std::endl;
    return 0;
  }

  static void framebuffer_size_callback(GLFWwindow *window, int width,
                                        int height) {
    glViewport(0, 0, width, height);
  }

  void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
      glfwSetWindowShouldClose(window, true);
    }
  }
};
