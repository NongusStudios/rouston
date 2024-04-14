#include "inc/gutils.h"

Error createShader(const char *src, GLenum type, uint32_t *dest) {
    uint32_t shd = glCreateShader(type);
    glShaderSource(shd, 1, &src, NULL);
    glCompileShader(shd);

    int result = 0;
    glGetShaderiv(shd, GL_COMPILE_STATUS, &result);
    if(!result){
        char log[1024];
        glGetShaderInfoLog(shd, 1024, NULL, log);
        fprintf(stderr, "%s\n", log);
        return ERROR("Failed to compile shader.");
    }

    *dest = shd;
    return SUCCESS;
}
Error createProgram(uint32_t *shaders, size_t count, uint32_t *dest) {
    uint32_t program = glCreateProgram();
    for(size_t i = 0; i < count; i++){
        glAttachShader(program, shaders[i]);
    }
    glLinkProgram(program);

    int result = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if(!result){
        char log[1024];
        glGetProgramInfoLog(program, 1024, NULL, log);
        fprintf(stderr, "%s\n", log);
        return ERROR("Failed to link program.");
    }

    *dest = program;
    return SUCCESS;
}

uint32_t createBufferStatic(const void* data, int64_t len){
    uint32_t buf;
    glCreateBuffers(1, &buf);
    glNamedBufferStorage(buf, len, data, GL_DYNAMIC_STORAGE_BIT);
    return buf;
}
uint32_t createBufferDynamic(const void* data, int64_t len, GLenum usage){
    uint32_t buf;
    glCreateBuffers(1, &buf);
    glNamedBufferData(buf, len, data, usage);
    return buf;
}

uint32_t createVertexArray(const VertexArrayDesc* desc){
    uint32_t va;
    glCreateVertexArrays(1, &va);

    glVertexArrayElementBuffer(va, desc->index_buffer);

    size_t j = 0;
    size_t lastj = 0;
    for(size_t i = 0; i < desc->count; i++){
        VertexBinding* binding = desc->bindings + i;
        assert(binding->attribute_count <= MAX_LOCATIONS);

        glVertexArrayVertexBuffer(va, i, desc->vertex_buffers[i], 0, desc->bindings[i].stride);

        for(; j < binding->attribute_count+lastj; j++){
            VertexAttribute* attrib = binding->attributes + j;

            glEnableVertexArrayAttrib(va, j);
            glVertexArrayAttribFormat(va, j, attrib->size, attrib->type, GL_FALSE, attrib->offset);
            glVertexArrayAttribBinding(va, j, i);
        }
        lastj = j;
        glVertexArrayBindingDivisor(va, i, (GLuint)binding->input_rate);
    }

    return va;
}
