#pragma once

#include "state.h"

#define GFX_LAYER_COUNT 100

typedef struct GfxDesc {
    int w, h;
} GfxDesc;

typedef enum GfxObjType {
    GfxObjType_PASS,
    GfxObjType_RECT,
    GfxObjType_RECTBATCH,
    GfxObjType_SPRITE,
    GfxObjType_SPRITEBATCH,
    GfxObjType_TEXT,
} GfxObjType;

typedef struct GfxRect {
    vec2 pos;
    float rotation;
    vec2 scale;
    vec4 colour;
    uint32_t layer;
} GfxRect;

typedef struct GfxSprite {
    vec2 pos;
    float rotation;
    vec2 scale;

    vec2 srcpos;
    vec2 srcscale;

    uint32_t layer;
    const char* texture_handle;
} GfxSprite;

typedef struct GfxSpriteBatch {
    VEC(GfxSprite) sprites;
} GfxSpriteBatch;

typedef struct GfxRectBatch {
    VEC(GfxRect) rects;
} GfxRectBatch;

typedef struct GfxText {
    vec2 pos;
    float rotation;
    vec2 scale;
    const char* text;
    uint32_t font_handle;
} GfxText;

typedef struct GfxObj {
    GfxObjType type;
    union {
        GfxRect rect;
        GfxRectBatch rectbatch;
        GfxSprite sprite;
        GfxSpriteBatch spritebatch;
        GfxText text;
    };
} GfxObj;

typedef struct Camera {
    vec2 pos;
} Camera;

typedef struct Scene {
    VEC(GfxObj) objects;
    Camera camera;
} Scene;

typedef struct bVertex {
    vec2 a_pos;
    vec2 a_uv;
} bVertex;

typedef struct uTransform {
    mat4 model;
    mat4 view;
    mat4 projection;
} uTransform;

void initGfx(const GfxDesc* desc);
void freeGfx();

void initScene(Scene* scene);
void freeScene(Scene* scene);

void beginFrame();
void endFrame();

GfxRect* addRectToScene(Scene* scene, GfxRect rect);
GfxRectBatch* addRectBatchToScene(Scene* scene, const GfxRect* rects, size_t rect_count);
void updateRectBatch(GfxRectBatch* batch, const GfxRect* rects, size_t rect_count);
GfxSprite* addSpriteToScene(Scene* scene, GfxSprite sprite);
// sprite.texture_handle becomes index into texture_handles
GfxSpriteBatch * addSpriteBatchToScene(Scene *scene, const GfxSprite *sprites, size_t sprite_count,
                                       uint32_t* texture_handles, size_t texture_count);
GfxText* addTextToScene(Scene* scene, GfxText text);

void setSceneCamera(Scene *scene, vec2 pos);

Error drawScene(const Scene* scene);
