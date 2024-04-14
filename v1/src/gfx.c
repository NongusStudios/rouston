#include "inc/gfx.h"

void initGfx(const GfxDesc* desc){

}
void freeGfx(){

}

void initScene(Scene* scene){

}
void freeScene(Scene* scene){

}

void addRectToScene(Scene* scene, const GfxRect* rect){

}
void addSpriteToScene(Scene* scene, const GfxSprite* sprite){

}
// sprite.texture_handle becomes index into texture_handles
void addSpriteBatchToScene(Scene* scene, const GfxSprite* sprites, size_t count, uint32_t* texture_handles){

}
void addTextToScene(Scene* scene, const GfxText* text){

}
void setSceneCamera(Scene *scene, vec2 pos) {

}

void drawScene(const Scene* scene){

}