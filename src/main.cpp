#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#define GL_VERSION_MAJOR 4
#define GL_VERSION_MINOR 6
#define WIDTH 800
#define HEIGHT 800
#define WINDOW_TITLE "OPEN GL BOSS"
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
  std::cout<<"Successfully created a GLFW window"<<std::endl;
  return window;
}
int init_glad() {
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cerr << "Failed to initialize GLAD" << std::endl;
    return -1;
  }
  std::cout<<"Successfully initialized GLAD"<<std::endl;
  return 0;
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0,0,width,height);
}
int main() {
  std::cout << "hello, world" << std::endl;
  glfw_init(GL_VERSION_MAJOR, GL_VERSION_MINOR);
  GLFWwindow *window = create_window(WIDTH, HEIGHT, (char *)WINDOW_TITLE);
  if (window == nullptr) {
    return -1;
  }
  glfwMakeContextCurrent(window);
  if(init_glad()){
	  return -1;
  }
  glViewport(0, 0, 800, 600);
  glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
  return 0;
}
