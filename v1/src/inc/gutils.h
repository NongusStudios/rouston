#pragma once

#include "state.h"

typedef enum VertexInputRate {
    VertexInputRate_VERTEX,
    VertexInputRate_INSTANCE,
} VertexInputRate;

typedef struct VertexAttribute {
    uint32_t location;
    uint32_t size;
    GLenum   type;
    size_t   offset;
} VertexAttribute;

typedef struct VertexBinding {
    size_t stride;
    VertexInputRate input_rate;

    VertexAttribute attributes[16];
    size_t attribute_count;
} VertexBinding;

typedef struct VertexArrayDesc {
    uint32_t* vertex_buffers;
    size_t vertex_buffer_count;

    VertexBinding* bindings;
    size_t binding_count;
} VertexArrayDesc;

Error createShader(const char* src, GLenum type, uint32_t* dest);
Error createProgram(uint32_t *shaders, size_t count, uint32_t *dest);

uint32_t createBufferStatic(void* data, size_t len);
uint32_t createBufferDynamic(void* data, size_t len);

uint32_t createVertexArray(const VertexArrayDesc* desc);
