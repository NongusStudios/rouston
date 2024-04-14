#include "inc/gfx.h"

#include "inc/gutils.h"
#include "inc/window.h"
#include "inc/asset_manager.h"
#include "inc/shaders.h"

const bVertex  g_rect_vertices[] = {
        { .a_pos = {-1.f, 1.f, }, .a_uv = { 0.f, 0.f } }, // [0] Top left
        { .a_pos = { 1.f, 1.f, }, .a_uv = { 1.f, 0.f } }, // [1] Top right
        { .a_pos = { 1.f,-1.f, }, .a_uv = { 1.f, 1.f } }, // [2] Bottom right
        { .a_pos = {-1.f,-1.f, }, .a_uv = { 0.f, 1.f } }, // [3] Bottom left
};
const uint32_t g_rect_indices [] = {
        0, 1, 2,
        2, 3, 0,
};

void initGfxObjects(Gfx* gfx){
    gfx->rect_vertex_buffer = createBufferStatic((void*)g_rect_vertices, sizeof(g_rect_vertices));
    gfx->rect_index_buffer = createBufferStatic((void*)g_rect_indices, sizeof(g_rect_indices));
    gfx->rectbatch_transform_storage_buffer = createBufferDynamic(NULL, 0, GL_DYNAMIC_DRAW);
    gfx->rectbatch_colour_storage_buffer = createBufferDynamic(NULL, 0, GL_DYNAMIC_DRAW);

    gfx->rect_vertex_array = createVertexArray(&(VertexArrayDesc){
            .count = 1,
            .vertex_buffers = &gfx->rect_vertex_buffer,
            .index_buffer   = gfx->rect_index_buffer,
            .bindings = (VertexBinding[]){
                    {
                            .input_rate = VertexInputRate_VERTEX,
                            .stride = sizeof(bVertex),
                            .attribute_count = 2,
                            .attributes = {
                                    {
                                            .type = GL_FLOAT,
                                            .size = 2,
                                            .offset = 0,
                                    },
                                    {
                                            .type = GL_FLOAT,
                                            .size = 2,
                                            .offset = sizeof(vec2),
                                    },
                            },
                    },
            },
    });

    /*gfx->rectbatch_vertex_array = createVertexArray(&(VertexArrayDesc){
        .count = 2,
        .vertex_buffers = (uint32_t[]){gfx->rect_vertex_buffer, gfx->rectbatch_instance_buffer, },
        .index_buffer   = gfx->rect_index_buffer,
        .bindings = (VertexBinding[]){
            {
                .input_rate = VertexInputRate_VERTEX,
                .stride = sizeof(bVertex),
                .attribute_count = 2,
                .attributes = {
                    {
                        .type = GL_FLOAT,
                        .size = 2,
                        .offset = 0,
                    },
                    {
                        .type = GL_FLOAT,
                        .size = 2,
                        .offset = sizeof(vec2),
                    },
                },
            },
            {
                .input_rate = VertexInputRate_INSTANCE,
                .stride = sizeof(RectBatchInstanceData),
                .attribute_count = 5,
                .attributes = {
                    {
                        .type = GL_FLOAT,
                        .size = 4,
                        .offset = 0,
                    },
                    {
                        .type = GL_FLOAT,
                        .size = 4,
                        .offset = sizeof(vec4),
                    },
                    {
                        .type = GL_FLOAT,
                        .size = 4,
                        .offset = sizeof(vec4)*2,
                    },
                    {
                        .type = GL_FLOAT,
                        .size = 4,
                        .offset = sizeof(vec4)*3,
                    },
                    {
                        .type = GL_FLOAT,
                        .size = 4,
                        .offset = sizeof(vec4)*4,
                    },
                },
            },
        },
    });*/

    uint32_t rect_vshd, rect_fshd, rectbatch_vshd, rectbatch_fshd, sprite_vshd, sprite_fshd;
    HANDLE_ERROR(createShader(g_rect_vshd, GL_VERTEX_SHADER, &rect_vshd));
    HANDLE_ERROR(createShader(g_rectbatch_vshd, GL_VERTEX_SHADER, &rectbatch_vshd));
    HANDLE_ERROR(createShader(g_sprite_vshd, GL_VERTEX_SHADER, &sprite_vshd));
    HANDLE_ERROR(createShader(g_rect_fshd, GL_FRAGMENT_SHADER, &rect_fshd));
    HANDLE_ERROR(createShader(g_rectbatch_fshd, GL_FRAGMENT_SHADER, &rectbatch_fshd));
    HANDLE_ERROR(createShader(g_sprite_fshd, GL_FRAGMENT_SHADER, &sprite_fshd));

    HANDLE_ERROR(createProgram((uint32_t[]){rect_vshd, rect_fshd, }, 2, &gfx->rect_program));
    HANDLE_ERROR(createProgram((uint32_t[]){rectbatch_vshd, rectbatch_fshd, }, 2, &gfx->rectbatch_program));
    HANDLE_ERROR(createProgram((uint32_t[]){sprite_vshd, sprite_fshd, }, 2, &gfx->sprite_program));

    glDeleteShader(rect_vshd);
    glDeleteShader(rectbatch_vshd);
    glDeleteShader(rect_fshd);
    glDeleteShader(rectbatch_fshd);
    glDeleteShader(sprite_vshd);
    glDeleteShader(sprite_fshd);
}

void freeRectObjects(Gfx* gfx){
    glDeleteBuffers(2, (uint32_t[]){gfx->rect_vertex_buffer, gfx->rect_index_buffer, });
    glDeleteProgram(gfx->rect_program);
    glDeleteProgram(gfx->rectbatch_program);
}

void initGfx(const GfxDesc* desc){
    Gfx* gfx = &getAppState()->gfx;
    gfx->clear_colour[0] = .4f;
    gfx->clear_colour[1] = .4f;
    gfx->clear_colour[2] = .4f;
    gfx->clear_colour[3] = 1.f;

    initGfxObjects(gfx);

    glViewport(0, 0, desc->w, desc->h);
    float w = (float)desc->w;
    float h = (float)desc->h;
    glm_ortho(-w/2.f, w/2.f, -h/2.f, h/2.f, -(float)GFX_LAYER_COUNT, (float)GFX_LAYER_COUNT, gfx->projection);
}
void freeGfx(){
    Gfx* gfx = &getAppState()->gfx;
    freeRectObjects(gfx);
    vecFree(gfx->rectbatch_models);
    vecFree(gfx->rectbatch_colours);
}

void initScene(Scene* scene){
    glm_vec2_zero(scene->camera.pos);
    scene->objects = VEC_INIT_EMPTY(GfxObj);
}

void beginFrame(){
    Gfx* gfx = &getAppState()->gfx;
    glEnable(GL_DEPTH_TEST);
    glClearColor(
        gfx->clear_colour[0],
        gfx->clear_colour[1],
        gfx->clear_colour[2],
        gfx->clear_colour[3]
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void endFrame(){
    windowSwapBuffers();
}

void freeScene(Scene* scene){
    VEC_FOREACH(GfxObj, obj, scene->objects){
        switch(obj->type){
            case  GfxObjType_SPRITEBATCH:
                vecFree(obj->spritebatch.sprites);
                break;
            case GfxObjType_RECTBATCH:
                vecFree(obj->rectbatch.rects);
                break;
            default: break;
        }
    }
    vecFree(scene->objects);
}

GfxRect* addRectToScene(Scene* scene, GfxRect rect){
    GfxObj obj = {
        .type = GfxObjType_RECT,
        .rect = rect,
    };
    vecPush(VEC_REF(scene->objects), VecPos_BACK, &obj);
    return &scene->objects[vecLen(scene->objects) - 1].rect;
}
GfxRectBatch* addRectBatchToScene(Scene* scene, const GfxRect* rects, size_t rect_count){
    GfxRectBatch batch;
    batch.rects = VEC_INIT_WITH_CAPACITY(GfxRect, rect_count);

    for(size_t i = 0; i < rect_count; i++){
        vecPush(VEC_REF(batch.rects), VecPos_BACK, rects + i);
    }

    GfxObj obj = {
        .type = GfxObjType_RECTBATCH,
        .rectbatch = batch,
    };

    vecPush(VEC_REF(scene->objects), VecPos_BACK, &obj);
    return &scene->objects[vecLen(scene->objects) - 1].rectbatch;
}
void updateRectBatch(GfxRectBatch* batch, const GfxRect* rects, size_t rect_count){
    vecClear(VEC_REF(batch->rects));
    for(size_t i = 0; i < rect_count; i++){
        vecPush(VEC_REF(batch->rects), VecPos_BACK, rects + i);
    }
}
GfxSprite* addSpriteToScene(Scene* scene, GfxSprite sprite){
    GfxObj obj = {
        .type = GfxObjType_SPRITE,
        .sprite = sprite,
    };
    vecPush(VEC_REF(scene->objects), VecPos_BACK, &obj);
    return &scene->objects[vecLen(scene->objects) - 1].sprite;
}
// sprite.texture_handle becomes index into texture_handles
GfxSpriteBatch* addSpriteBatchToScene(Scene *scene, const GfxSprite* sprites, size_t sprite_count, uint32_t* texture_handles,
                      size_t texture_count) {
    TODO
}
GfxText* addTextToScene(Scene* scene, GfxText text){
    TODO
}
void setSceneCamera(Scene *scene, vec2 pos) {
    glm_vec2_copy(pos, scene->camera.pos);
}

void rectModelMat(const GfxRect* rect, mat4 dest){
    glm_mat4_identity(dest);

    vec3 pos = {rect->pos[0], rect->pos[1], -(float)rect->layer};
    glm_translate(dest, pos);
    glm_rotate_z(dest, rect->rotation, dest);

    vec3 scale = { rect->scale[0], rect->scale[1], 0.f };
    glm_scale(dest, scale);
}

void spriteModelMat(const GfxSprite* sprite, mat4 dest){
    glm_mat4_identity(dest);

    vec3 pos = {sprite->pos[0], sprite->pos[1], -(float)sprite->layer};
    glm_translate(dest, pos);
    glm_rotate_z(dest, sprite->rotation, dest);

    vec3 scale = { sprite->scale[0], sprite->scale[1], 0.f };
    glm_scale(dest, scale);
}

void drawRect(Gfx* gfx, const GfxRect* rect, mat4 view){

    mat4 model;
    rectModelMat(rect, model);

    glUseProgram(gfx->rect_program);
    glUniformMatrix4fv(0, 1, GL_FALSE, (float*)model);
    glUniformMatrix4fv(1, 1, GL_FALSE, (float*)view);
    glUniformMatrix4fv(2, 1, GL_FALSE, (float*)gfx->projection);
    glUniform4fv(3, 1, rect->colour);

    glBindVertexArray(gfx->rect_vertex_array);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Error drawSprite(Gfx* gfx, const GfxSprite* sprite, mat4 view){
    mat4 model;
    spriteModelMat(sprite, model);

    glUseProgram(gfx->sprite_program);
    glUniformMatrix4fv(0, 1, GL_FALSE, (float*)model);
    glUniformMatrix4fv(1, 1, GL_FALSE, (float*)view);
    glUniformMatrix4fv(2, 1, GL_FALSE, (float*)gfx->projection);

    const Asset* asset = getAsset(sprite->texture_handle);
    if(!asset || asset->type != AssetType_TEXTURE){
        return ERROR("Invalid asset handle, either doesn't exist or is not a texture");
    }

    vec2 pos = {sprite->srcpos[0]/(float)asset->texture.width, sprite->srcpos[1]/(float)asset->texture.width};
    vec2 size  = {sprite->srcscale[0]/(float)asset->texture.width, sprite->srcscale[1]/(float)asset->texture.width};

    glUniform2fv(3, 1, pos);
    glUniform2fv(4, 1, size);

    glBindTextureUnit(0, asset->texture.texture);

    glBindVertexArray(gfx->rect_vertex_array);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void drawRectBatch(Gfx* gfx, const GfxRectBatch* batch, mat4 view){
    if(!gfx->rectbatch_models){
        gfx->rectbatch_models = VEC_INIT_WITH_CAPACITY(mat4, vecLen(batch->rects) + 1);
        gfx->rectbatch_colours = VEC_INIT_WITH_CAPACITY(vec4, vecLen(batch->rects) + 1);
    }

    VEC_FOREACH(GfxRect, rect, batch->rects){
        mat4 model;
        rectModelMat(rect, model);
        glm_mat4_copy(model, model);
        vecPush(VEC_REF(gfx->rectbatch_models), VecPos_BACK, model);
        vecPush(VEC_REF(gfx->rectbatch_colours), VecPos_BACK, rect->colour);
    }

    if(vecLen(gfx->rectbatch_models) > gfx->rectbatch_storage_buffer_size) {
        glNamedBufferData(gfx->rectbatch_transform_storage_buffer,
                          (GLsizeiptr) (vecLen(gfx->rectbatch_models) * sizeof(mat4)),
                          gfx->rectbatch_models, GL_DYNAMIC_DRAW);

        glNamedBufferData(gfx->rectbatch_colour_storage_buffer,
                          (GLsizeiptr) (vecLen(gfx->rectbatch_colours) * sizeof(vec4)),
                          gfx->rectbatch_colours, GL_DYNAMIC_DRAW);

        gfx->rectbatch_storage_buffer_size = vecLen(gfx->rectbatch_models);
    } else {
        glNamedBufferSubData(gfx->rectbatch_transform_storage_buffer, 0,
                             (GLsizeiptr)(vecLen(gfx->rectbatch_models) * sizeof(mat4)),
                             gfx->rectbatch_models);
        glNamedBufferSubData(gfx->rectbatch_colour_storage_buffer, 0,
                             (GLsizeiptr)(vecLen(gfx->rectbatch_colours) * sizeof(vec4)),
                             gfx->rectbatch_colours);
    }

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, gfx->rectbatch_transform_storage_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, gfx->rectbatch_transform_storage_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, gfx->rectbatch_colour_storage_buffer);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, gfx->rectbatch_colour_storage_buffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    glUseProgram(gfx->rectbatch_program);
    glUniformMatrix4fv(0, 1, GL_FALSE, (float*)view);
    glUniformMatrix4fv(1, 1, GL_FALSE, (float*)gfx->projection);

    glBindVertexArray(gfx->rect_vertex_array);
    glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0,
                            (GLsizei) vecLen(batch->rects));

    vecClear(VEC_REF(gfx->rectbatch_models));
}

Error drawScene(const Scene* scene){
    Gfx* gfx = &getAppState()->gfx;

    mat4 view;
    glm_mat4_identity(view);

    VEC_FOREACH(GfxObj, obj, scene->objects){
        switch (obj->type) {
            case GfxObjType_RECT:
                drawRect(gfx, &obj->rect, view);
                break;
            case GfxObjType_RECTBATCH:
                drawRectBatch(gfx, &obj->rectbatch, view);
                break;
            case GfxObjType_SPRITE: {
                Error err = drawSprite(gfx, &obj->sprite, view);
                if(err.type != ErrorType_SUCCESS) return err;
                break;
            }
            default: break;
        }
    }

    return SUCCESS;
}