#include "glfw.h"
#include "includes.h"
#include "shader.h"
#define GL_VERSION_MAJOR 4
#define GL_VERSION_MINOR 6
#define WIDTH 800
#define HEIGHT 800
#define WINDOW_TITLE "OPEN GL BOSS"

int main() {
  glfw glfw;
  glfw.init(GL_VERSION_MAJOR, GL_VERSION_MINOR, WIDTH, HEIGHT, WINDOW_TITLE);

  float vertices[] = {
      -0.5, 0.5, 0.0, -0.5, -0.5, 0.0, 0.5, -0.5, 0.0, 0.5, 0.5, 0.0,
  };
  unsigned int indices[] = {0, 1, 2, 0, 2, 3};

  unsigned int VAO;
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  unsigned int VBO;
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  unsigned int EBO;
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                        (void *)nullptr);
  glEnableVertexAttribArray(0);

  Shader shader = Shader();
  int vertex_color_location = glGetUniformLocation(shader.ID, "color");
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
            << std::endl;

  while (!glfw.should_close()) {
    glfw.process_input();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();

    float time = glfwGetTime();
    float green = sin(time) / 2.0f + 0.5f;
    float red = cos(time) / 2.0f + 0.5f;
    float blue = tan(time) / 2.0f + 0.5f;
    glUniform3f(vertex_color_location, red, green, blue);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwPollEvents();
    glfwSwapBuffers(glfw.window);
  }

  shader.terminate();
  glfw.terminate();

  return 0;
}
