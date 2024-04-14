#include "inc/inc.h"

#define WIDTH 800
#define HEIGHT 600

#define SPRITE_HANDLE_WIZARD "wizard.png"
#define SPRITE_HANDLE_SPRITE "sprite.jpg"

Scene scene;

void init(){
    Error err = {};

    initAppState();
    err = initWindow(&(WindowDesc){
            .width = WIDTH,
            .height = HEIGHT,
            .title = "Demo",
            .resizable = false,
            .type = WindowType_WINDOWED,
            .swap_interval = 0,
    });
    HANDLE_ERROR(err);

    err = initAssetManager();
    HANDLE_ERROR(err);

    initGfx(&(GfxDesc){
        .w = WIDTH,
        .h = HEIGHT,
    });
}

void cleanup(){
    freeGfx();
    freeAssetManager();
    freeWindow();
}

void input(Event* events, size_t count){
    for(Event* event = events; event != events + count; event++){
        switch(event->type){
            case EventType_KEY:
                if(event->key.key == GLFW_KEY_ESCAPE && event->key.action == GLFW_PRESS){
                    setWindowRunning(false);
                }
                break;
            default:
                break;
        }
    }
}

int main() {
    init();
    AppState* state = getAppState();

    initScene(&scene);

    float w, h;
    w = (float)getAsset(SPRITE_HANDLE_WIZARD)->texture.width;
    h = (float)getAsset(SPRITE_HANDLE_WIZARD)->texture.height;
    addSpriteToScene(&scene, (GfxSprite){
        .pos = {-250.f, 0.f},
        .layer = 0,
        .rotation = 0.f,
        .scale = {w,
                  h},
        .srcpos = {0.f, 0.f},
        .srcscale = {w, h},
        .texture_handle = SPRITE_HANDLE_WIZARD,
    });

    w = (float)getAsset(SPRITE_HANDLE_SPRITE)->texture.width;
    h = (float)getAsset(SPRITE_HANDLE_SPRITE)->texture.height;
    addSpriteToScene(&scene, (GfxSprite){
        .pos = {0.f, 0.f},
        .layer = 99,
        .rotation = 0.f,
        .scale = {w/4.f,
                  h/4.f},
        .srcpos = {w/3.f, 0.f},
        .srcscale = {w/3.f, h},
        .texture_handle = SPRITE_HANDLE_SPRITE,
    });

    double title_update = 0.0;
    while(isWindowRunning()){
        windowPollEvents();

        size_t event_count = 0;
        Event* events = windowGetEvents(&event_count);
        input(events, event_count);

        beginFrame();
        HANDLE_ERROR(drawScene(&scene));
        endFrame();

        title_update += state->time.delta;
        if(title_update >= 0.5){
            title_update = 0.0;
            char title[128];
            snprintf(title, 128, "Demo: FPS - %u | Delta - %f", (uint32_t)(1.0 / state->time.delta), state->time.delta);
            setWindowTitle(title);
        }
    }

    freeScene(&scene);
    cleanup();
    return 0;
}
