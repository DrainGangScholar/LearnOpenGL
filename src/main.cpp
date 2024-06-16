#include "glfw.h"
#include "includes.h"
#include "shader.h"
#include <ostream>
#define GL_VERSION_MAJOR 4
#define GL_VERSION_MINOR 2
#define WIDTH 800
#define HEIGHT 800
#define WINDOW_TITLE "OPEN GL BOSS"

inline unsigned int texture(std::string name) {
  unsigned int texture;

  glGenTextures(1, &texture);

  std::cout << texture << std::endl;
  glBindTexture(GL_TEXTURE_2D, texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char *data =
      stbi_load(std::string("../assets/textures/").append(name).c_str(), &width,
                &height, &channels, 0);
  if (!data) {
    std::cerr << "Couldn't load texture" << std::endl;
    stbi_image_free(data);
    return -1;
  }

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
               GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);

  stbi_image_free(data);
  return texture;
}
int main() {
  glfw glfw;
  glfw.init(GL_VERSION_MAJOR, GL_VERSION_MINOR, WIDTH, HEIGHT, WINDOW_TITLE);

  float vertices[] = {-0.5, 0.5,  0.0, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
                      -0.5, -0.5, 0.0, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
                      0.5,  -0.5, 0.0, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
                      0.5,  0.5,  0.0, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};
  unsigned int indices[] = {0, 1, 2, 0, 2, 3};

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // vertex
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // color
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture
  //
  // unsigned int texture_id = texture("wall.jpg");
  unsigned int texture_id = texture("awesomeface.png");
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                        (void *)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  Shader shader = Shader();
  int vertex_color_location = glGetUniformLocation(shader.ID, "color");
  int nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes
            << std::endl;

  shader.use();
  glUniform1i(glGetUniformLocation(shader.ID, "texture"), 0);

  while (!glfw.should_close()) {
    glfw.process_input();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    shader.use();
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glfwPollEvents();
    glfwSwapBuffers(glfw.window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  shader.terminate();
  glfw.terminate();

  return 0;
}
