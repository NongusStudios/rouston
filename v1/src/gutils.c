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

}

uint32_t createBufferStatic(void* data, size_t len){

}
uint32_t createBufferDynamic(void* data, size_t len){

}

uint32_t createVertexArray(const VertexArrayDesc* desc){

}
