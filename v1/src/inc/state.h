#pragma once

#include "../ext/glad/include/glad/glad.h"
#include "../ext/glfw/include/GLFW/glfw3.h"
#include "../ext/cglm/include/cglm/cglm.h"

#include "../ext/miniaudio.h"
#include "../ext/stb_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdbool.h>
#include <assert.h>

#define SUCCESS (Error){.type = ErrorType_SUCCESS}
#define ERROR(msg) (Error){.type = ErrorType_ERROR, .what = msg}

#define DYNARR(t) struct { t* ptr; size_t count; size_t cap; }
#define DYNARR_INIT(t, n, arr) (arr).ptr = memAlloc(n*sizeof(t)); (arr).count = 0; (arr).cap = n
#define DYNARR_FREE(arr) memFree((arr).ptr)
#define DYNARR_RESIZE(t, n, arr) (arr).ptr = ((arr).ptr) ? memRealloc((arr).ptr, n*sizeof(t)) : memAlloc(n*sizeof(t)); \
    (arr).cap = n; (arr).count = ((arr).cap < (arr).count) ? (arr).cap : (arr).count
#define DYNARR_APPEND(t, i, arr) do {                    \
    if((arr).count == (arr).cap) {                       \
        DYNARR_RESIZE(t, ((arr).cap+1)*2, arr);          \
    }                                                    \
    (arr).ptr[(arr).count++] = (i);                      \
} while(false)
#define DYNARR_CLEAR(t, arr) (arr).count = 0

#define HANDLE_ERROR(err) if(!handleErr(err)) abort();

typedef enum ErrorType {
    ErrorType_SUCCESS,
    ErrorType_WARNING,
    ErrorType_ERROR,
} ErrorType;

typedef struct Error {
    ErrorType type;
    const char* what;
} Error;


typedef enum EventType {
    EventType_KEY,
    EventType_CHARACTER,
    EventType_CURSORPOS,
    EventType_CURSOR_ENTERED,
    EventType_CURSOR_LEFT,
    EventType_MOUSEBUTTON,
    EventType_SCROLL,
} EventType;
typedef struct Event {
    EventType type;
    union {
        struct {
            int key, action, mods;
        } key;
        uint32_t character;
        struct {
            double x, y;
        } cursorpos;
        struct {
            int button, action, mods;
        } mousebutton;
        struct {
            double xoff, yoff;
        } scroll;
    };
} Event;
typedef struct Window {
    GLFWwindow* window;
    int width, height;

    DYNARR(Event) events;
} Window;

typedef struct Gfx {
    uint32_t tmp;
} Gfx;

typedef struct AppState {
    Window window;
    Gfx    gfx;
} AppState;

void initAppState();

AppState* getAppState();

void* memAlloc(size_t n);
void* memRealloc(void* ptr, size_t n);
void  memFree(void* ptr);

bool handleError(const Error* err);
