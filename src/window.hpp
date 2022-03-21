#pragma once

#include "opengl.hpp"

class Window {
public:
    Window(std::string title, const glm::ivec2& size, const glm::ivec2& position = {});
    Window(std::string title);
    ~Window();

    static Window& getInstance() { return *instance; }

    operator GLFWwindow*() const { return window; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    float getAspect() const { return static_cast<float>(width) / static_cast<float>(height); }
    glm::ivec2 getSize() const { return {width, height}; }
    glm::vec4 getViewport() const { return {0, height, width, -height}; } // vertical flip is required

    bool shouldClose() const { return glfwWindowShouldClose(window); }
    void shouldClose(bool flag) const { glfwSetWindowShouldClose(window, flag); }

    void makeCurrent() const { glfwMakeContextCurrent(window); }
    void swapBuffers() const { glfwSwapBuffers(window); }

    void showWindow(bool show = true) {
        if (show) {
            glfwShowWindow(window);
        } else {
            glfwHideWindow(window);
        }
    }

    void setCursorPos(const glm::dvec2& pos) {
        glfwSetCursorPos(window, pos.x, pos.y);
    }

    const std::string& getTitle() const { return title; }
    void setTitle(const std::string& str) {
        title = str;
        glfwSetWindowTitle(window, title.c_str());
    }

    const glm::ivec2& getPosition() const { return position; }
    void setSizeLimits(const glm::uvec2& minSize, const glm::uvec2& maxSize = {}) {
        glfwSetWindowSizeLimits(window, minSize.x, minSize.y, (maxSize.x != 0) ? maxSize.x : minSize.x, (maxSize.y != 0) ? maxSize.y : minSize.y);
    }

    void pollEvents() const {
        glfwPollEvents();
    }

    void waitEvents() const {
        glfwWaitEvents();
    }

    void toggleCursor() {
        locked = !locked;
        glfwSetInputMode(window, GLFW_CURSOR, locked ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
    }

    void toggleWireframe() {
        wireframe = !wireframe;
        glCall(glPolygonMode, GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
    }

    bool Locked() const { return locked; }
    bool Wireframe() const { return wireframe; }

private:
    GLFWwindow* window;
    int width;
    int height;
    std::string title;
    glm::ivec2 position;

    bool locked{ false };
    bool wireframe{ false };

    void initGLFW();
    void initWindow(bool fullscreen);

    static Window* instance;
    static std::vector<GLFWwindow*> instances;

    static void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void PosCallback(GLFWwindow* window, int x, int y);
    static void ErrorCallback(int error, const char* description);
};
