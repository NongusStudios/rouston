#pragma once

#include "../ext/glad/include/glad/glad.h"
#include "../ext/glfw/include/GLFW/glfw3.h"
#include "../ext/cglm/include/cglm/cglm.h"
#include "../ext/hashmap/hashmap.h"
#include "../ext/miniaudio.h"
#include "../ext/stb_image.h"

#include "utils.h"
#include "jvcc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <stdbool.h>
#include <assert.h>

typedef struct hashmap HashMap;

#define SUCCESS (Error){.type = ErrorType_SUCCESS}
#define ERROR(msg) (Error){.type = ErrorType_ERROR, .what = msg}

#define TODO printf("%s not implemented\n", __func__);

/*#define DYNARR(t) struct { t* ptr; size_t count; size_t cap; }
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
#define DYNARR_ADDONE(t, arr)  do {                      \
    if((arr).count == (arr).cap) {                       \
        DYNARR_RESIZE(t, ((arr).cap+1)*2, arr);          \
    }                                                    \
    (arr).count++;                                       \
} while(false)
#define DYNARR_CLEAR(arr) (arr).count = 0
#define DYNARR_FOR(t, name, arr) for(t* name = (arr).ptr; name != (arr).ptr + (arr).count; name++)*/

#define HANDLE_ERROR(err) if(!handleError(err)) abort()

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

    VEC(Event) events;
} Window;

typedef struct Gfx {
    uint32_t rect_vertex_array;

    uint32_t rect_vertex_buffer;
    uint32_t rectbatch_transform_storage_buffer;
    uint32_t rectbatch_colour_storage_buffer;
    size_t   rectbatch_storage_buffer_size;
    uint32_t rect_index_buffer;

    uint32_t rect_program;
    uint32_t rectbatch_program;
    uint32_t sprite_program;

    VEC(mat4) rectbatch_models;
    VEC(vec4) rectbatch_colours;
    mat4 projection;
    vec4 clear_colour;
} Gfx;

typedef struct Time {
    double elapsed;
    double delta;
    float  elapsedf;
    float  deltaf;
} Time;

typedef enum AssetType {
    AssetType_TEXTURE,
} AssetType;

typedef struct AssetTexture {
    uint32_t width, height;
    GLenum format;
    uint32_t texture;
} AssetTexture;

typedef struct Asset {
    char name[JVCC_MAX_STRING_LENGTH];
    AssetType   type;
    union {
        AssetTexture texture;
    };
} Asset;

typedef struct AssetManger {
    HashMap* assets;
} AssetManger;

typedef struct AppState {
    Window window;
    Gfx    gfx;
    Time   time;
    AssetManger asset_manager;
} AppState;

void initAppState();

AppState* getAppState();

bool handleError(Error err);
