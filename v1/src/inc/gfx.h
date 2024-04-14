#pragma once

#include "state.h"

typedef struct GfxDesc {
    int w, h;
} GfxDesc;

typedef enum GfxObjType {
    GfxObjType_RECT,
    GfxObjType_SPRITE,
    GfxObjType_SPRITEBATCH,
    GfxObjType_TEXT,
} GfxObjType;

typedef struct GfxRect {
    vec2 pos;
    float rotation;
    vec2 scale;
    vec4 colour;
} GfxRect;

typedef struct GfxSprite {
    vec2 pos;
    float rotation;
    vec2 scale;

    vec2 srcpos;
    vec2 srcscale;

    uint32_t texture_handle;
} GfxSprite;

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
        GfxSprite sprite;
        DYNARR(GfxSprite) spritebatch;
        GfxText text;
    };
} GfxObj;

typedef struct Camera {
    vec2 pos;
} Camera;

typedef struct Scene {
    DYNARR(GfxObj) objects;
    vec4 clear_colour;
    Camera camera;
} Scene;

void initGfx(const GfxDesc* desc);
void freeGfx();

void initScene(Scene* scene);
void freeScene(Scene* scene);

void addRectToScene(Scene* scene, const GfxRect* rect);
void addSpriteToScene(Scene* scene, const GfxSprite* sprite);
// sprite.texture_handle becomes index into texture_handles
void addSpriteBatchToScene(Scene* scene, const GfxSprite* sprites, size_t count, uint32_t* texture_handles);
void addTextToScene(Scene* scene, const GfxText* text);

void setSceneCamera(Scene *scene, vec2 pos);

void drawScene(const Scene* scene);
