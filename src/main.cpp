#include "glfw.h"
#include "includes.h"
#include "shader.h"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/trigonometric.hpp>
#include <ostream>
#define GL_VERSION_MAJOR 4
#define GL_VERSION_MINOR 2
#define WIDTH 800
#define HEIGHT 800
#define WINDOW_TITLE "OPEN GL BOSS"

glm::vec3 position = glm::vec3(0.0, 0.0, 3.0);
glm::vec3 front = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
float speed = 0.05f;

float delta_time = 0.0f;
float last_frame = 0.0f;

float pitch = 0.0f;
float yaw = -90.0f;

bool first_mouse = true;

float last_x = WIDTH / 2.0, last_y = HEIGHT / 2.0;

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
    return;
  }
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
    position += speed * front;
    return;
  }
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
    position -= speed * front;
    return;
  }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
    position -= glm::normalize(glm::cross(front, up)) * speed;
    return;
  }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
    position += glm::normalize(glm::cross(front, up)) * speed;
    return;
  }
}

void mouse_callback(GLFWwindow *window) {
  double x_pos, y_pos;
  glfwGetCursorPos(window, &x_pos, &y_pos);
  if (first_mouse) {
    last_x = x_pos;
    last_y = y_pos;
    first_mouse = false;
  }

  float offset_x = x_pos - last_x;
  float offset_y = y_pos - last_y;
  last_x = x_pos;
  last_y = y_pos;

  const float sensitivity = 0.01f;
  offset_x *= sensitivity;
  offset_y *= sensitivity;
  yaw += offset_x;
  pitch += offset_y;
  if (pitch > 89.0f) {
    pitch = 89.0f;
  }
  if (pitch < -89.0f) {
    pitch = -89.0f;
  }

  glm::vec3 direction;
  direction.x = cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
  direction.y = sin(glm::radians(pitch));
  direction.z = sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch));
  front = glm::normalize(direction);
}

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

  float vertices[] = {
      -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 0.0f,
      0.5f,  0.5f,  -0.5f, 1.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 1.0f, -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,

      -0.5f, 0.5f,  0.5f,  1.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  0.5f,  1.0f, 0.0f,

      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 0.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  0.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.5f,  -0.5f, -0.5f, 1.0f, 1.0f,
      0.5f,  -0.5f, 0.5f,  1.0f, 0.0f, 0.5f,  -0.5f, 0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f, 0.5f,  0.0f, 0.0f, -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

      -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f, 0.5f,  0.5f,  -0.5f, 1.0f, 1.0f,
      0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, 0.5f,  0.5f,  0.0f, 0.0f, -0.5f, 0.5f,  -0.5f, 0.0f, 1.0f};

  glm::vec3 cube_positions[] = {
      glm::vec3(0.0f, 0.0f, 0.0f),    glm::vec3(2.0f, 5.0f, -15.0f),
      glm::vec3(-1.5f, -2.2f, -2.5f), glm::vec3(-3.8f, -2.0f, -12.3f),
      glm::vec3(2.4f, -0.4f, -3.5f),  glm::vec3(-1.7f, 3.0f, -7.5f),
      glm::vec3(1.3f, -2.0f, -2.5f),  glm::vec3(1.5f, 2.0f, -2.5f),
      glm::vec3(1.5f, 0.2f, -1.5f),   glm::vec3(-1.3f, 1.0f, -1.5f)};

  unsigned int VBO, VAO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  // vertex
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);
  // texture
  unsigned int texture_id = texture("awesomeface.png");
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void *)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

  Shader shader = Shader();
  shader.use();
  glUniform1i(glGetUniformLocation(shader.ID, "texture"), 0);

  glEnable(GL_DEPTH_TEST);

  GLFWwindow *window = glfw.window;

  glfwSetCursorPosCallback(glfw.window, (GLFWcursorposfun)mouse_callback);

  while (!glfw.should_close()) {
    //    glfw.process_input();
    process_input(window);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id);

    shader.use();

    glm::mat4 proj = glm::perspective(
        glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
    unsigned int _proj = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(_proj, 1, GL_FALSE, glm::value_ptr(proj));

    unsigned int _model = glGetUniformLocation(shader.ID, "model");

    glm::mat4 view = glm::lookAt(position, position + front, up);
    unsigned int _view = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(_view, 1, GL_FALSE, glm::value_ptr(view));

    glBindVertexArray(VAO);
    for (unsigned int i = 0; i < 10; i++) {
      float angle = (float)glfwGetTime() * glm::radians(50.0f);
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cube_positions[i]);
      model = glm::rotate(model, angle, glm::vec3(0.5f, 1.0f, 0.0f));
      glUniformMatrix4fv(_model, 1, GL_FALSE, glm::value_ptr(model));
      glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glfwPollEvents();
    glfwSwapBuffers(glfw.window);
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);

  shader.terminate();
  glfw.terminate();

  return 0;
}
