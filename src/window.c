#include "inc/window.h"

#define EVENTARR_INITCAP 1

void g_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    Window* win = &getAppState()->window;
    Event event = {
            .type = EventType_KEY,
            .key = {
                    .key = key,
                    .action = action,
                    .mods = mods,
            },
    };
    vecPush(VEC_REF(win->events), VecPos_BACK, &event);
}
void g_char_callback(GLFWwindow* window, uint32_t codepoint){
    Window* win = &getAppState()->window;
    Event event = {
            .type = EventType_CHARACTER,
            .character = codepoint,
    };
    vecPush(VEC_REF(win->events), VecPos_BACK, &event);
}
void g_cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    Window* win = &getAppState()->window;
    Event event = {
            .type = EventType_CURSORPOS,
            .cursorpos = {
                    .x = xpos,
                    .y = ypos,
            },
    };
    vecPush(VEC_REF(win->events), VecPos_BACK, &event);
}
void g_cursor_enter_callback(GLFWwindow* window, int entered){
    Window* win = &getAppState()->window;
    Event event = {
            .type = (entered) ? EventType_CURSOR_ENTERED : EventType_CURSOR_LEFT,
    };
    vecPush(VEC_REF(win->events), VecPos_BACK, &event);
}
void g_mbutton_callback(GLFWwindow* window, int button, int action, int mods){
    Window* win = &getAppState()->window;
    Event event = {
            .type = EventType_MOUSEBUTTON,
            .mousebutton = {
                    .button = button,
                    .action = action,
                    .mods = mods,
            },
    };
    vecPush(VEC_REF(win->events), VecPos_BACK, &event);
}
void g_scroll_callback(GLFWwindow* window, double xoff, double yoff){
    Window* win = &getAppState()->window;
    Event event = {
            .type = EventType_MOUSEBUTTON,
            .scroll = {
                    .xoff = xoff,
                    .yoff = yoff,
            },
    };
    vecPush(VEC_REF(win->events), VecPos_BACK, &event);
}

void g_resize_callback(GLFWwindow* window, int w, int h){
    Window* win = &getAppState()->window;
    win->width = w;
    win->height = h;
}

void updateWindowSize(Window* window){
    int w,h;
    glfwGetFramebufferSize(window->window, &w, &h);
    window->width  = w;
    window->height = h;
}

Error initWindow(const WindowDesc* desc){
    Window* window = &getAppState()->window;

    if(!glfwInit()) return ERROR("Failed to initialise GLFW!");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, desc->resizable);

    GLFWmonitor* mon = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(mon);
    switch(desc->type){
        case WindowType_WINDOWED:
            window->window = glfwCreateWindow(desc->width, desc->height, desc->title, NULL, NULL);
            break;
        case WindowType_BORDERLESS:
            glfwWindowHint(GLFW_RED_BITS, mode->redBits);
            glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
            glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
            glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
            window->window = glfwCreateWindow(mode->width, mode->height, desc->title, mon, NULL);
            break;
        case WindowType_EXCLUSIVE:
            window->window = glfwCreateWindow(desc->width, desc->height, desc->title, mon, NULL);
            break;
    }

    if(!window->window) return ERROR("Failed to create a window!");

    glfwSetWindowUserPointer(window->window, getAppState());
    glfwMakeContextCurrent(window->window);
    glfwSwapInterval(desc->swap_interval);

    glfwSetKeyCallback(window->window, g_key_callback);
    glfwSetCharCallback(window->window, g_char_callback);
    glfwSetCursorPosCallback(window->window, g_cursor_position_callback);
    glfwSetCursorEnterCallback(window->window, g_cursor_enter_callback);
    glfwSetMouseButtonCallback(window->window, g_mbutton_callback);
    glfwSetScrollCallback(window->window, g_scroll_callback);
    glfwSetWindowSizeCallback(window->window, g_resize_callback);

    updateWindowSize(window);
    window->events = VEC_INIT_WITH_CAPACITY(Event, 8);

    if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) return ERROR("Failed to load OpenGL!");

    return SUCCESS;
}
void  freeWindow(){
    AppState* state = getAppState();

    vecFree(getAppState()->window.events);

    glfwDestroyWindow(getAppState()->window.window);
    glfwTerminate();
}

bool isWindowRunning(){
    return !glfwWindowShouldClose(getAppState()->window.window);
}
void setWindowRunning(bool running){
    glfwSetWindowShouldClose(getAppState()->window.window, !running);
}
void setWindowTitle(const char* title){
    glfwSetWindowTitle(getAppState()->window.window, title);
}

void updateTime(double elapsed){
    Time* time = &getAppState()->time;
    time->delta = elapsed - time->elapsed;
    time->elapsed = elapsed;
    time->deltaf = (float)time->delta;
    time->elapsedf = (float)elapsed;
}

void windowPollEvents(){
    Window* window = &getAppState()->window;

    vecClear(VEC_REF(window->events));
    glfwPollEvents();
    updateTime(glfwGetTime());

    updateWindowSize(window);
}
void windowSwapBuffers(){
    glfwSwapBuffers(getAppState()->window.window);
}

Event* windowGetEvents(size_t* n){
    *n = vecLen(getAppState()->window.events);
    return getAppState()->window.events;
}
