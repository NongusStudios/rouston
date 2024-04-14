#pragma once

#include "state.h"

#define MAX_LOCATIONS 16

typedef enum VertexInputRate {
    VertexInputRate_VERTEX,
    VertexInputRate_INSTANCE,
} VertexInputRate;

typedef struct VertexAttribute {
    int      size;
    GLenum   type;
    size_t   offset;
} VertexAttribute;

typedef struct VertexBinding {
    GLsizei stride;
    VertexInputRate input_rate;

    VertexAttribute attributes[MAX_LOCATIONS];
    size_t attribute_count;
} VertexBinding;

typedef struct VertexArrayDesc {
    uint32_t* vertex_buffers;
    uint32_t index_buffer;
    VertexBinding* bindings;
    size_t count;
} VertexArrayDesc;

Error createShader(const char* src, GLenum type, uint32_t* dest);
Error createProgram(uint32_t *shaders, size_t count, uint32_t *dest);

uint32_t createBufferStatic(const void* data, int64_t len);
uint32_t createBufferDynamic(const void* data, int64_t len, GLenum usage);

uint32_t createVertexArray(const VertexArrayDesc* desc);
