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

GLFWwindow *create_window(int width, int height, const char *title) {
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
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
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

unsigned int init_shader(GLuint shader_type, std::string path) {
    auto s = read_shader(path);
    const char* shader_content = s.c_str();
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
        std::cerr << "SHADER::VERTERX::COMPILATION_FAILED\n"<< std::endl;
        return -1;
    }
    return shader;
}

int main() {
    glfw_init(GL_VERSION_MAJOR, GL_VERSION_MINOR);
    GLFWwindow *window = create_window(WIDTH, HEIGHT, (char *) WINDOW_TITLE);
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

    float vertices[]={
            -0.5,0.5,0.0,
            -0.5,-0.5,0.0,
            0.5,-0.5,0.0,
            -0.5,0.5,0.0,
            0.5,-0.5,0.0,
            0.5,0.5,0.0,
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int vertex_shader = init_shader(GL_VERTEX_SHADER,"../shaders/vs.glsl");
    unsigned int fragment_shader = init_shader(GL_FRAGMENT_SHADER,"../shaders/fs.glsl");

    const unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);
    glEnableVertexAttribArray(0);

    glUseProgram(shader_program);
    glBindVertexArray(VAO);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glfwTerminate();

    return 0;
}
