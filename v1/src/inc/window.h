#pragma once

#include "state.h"

typedef enum WindowType {
    WindowType_WINDOWED,
    WindowType_BORDERLESS,
    WindowType_EXCLUSIVE,
} WindowType;

typedef struct WindowDesc {
    int width, height;
    bool resizable;
    const char* title;
    WindowType type;
} WindowDesc;

Error initWindow(const WindowDesc* desc);
void  freeWindow();

bool isWindowRunning();
void setWindowRunning(bool running);

void windowPollEvents();
void windowSwapBuffers();

Event* windowGetEvents(size_t* n);
