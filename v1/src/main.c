#include "inc/inc.h"



void init(){
    Error err = {};

    initAppState();
    err = initWindow(&(WindowDesc){
            .width = 800,
            .height = 600,
            .title = "Demo",
            .resizable = true,
            .type = WindowType_WINDOWED,
    });
    assert(handleError(&err));


}

void cleanup(){
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
    while(isWindowRunning()){
        windowPollEvents();

        size_t event_count = 0;
        Event* events = windowGetEvents(&event_count);
        input(events, event_count);


        glViewport(0, 0, state->window.width, state->window.height);
        glClearColor(.4, .4, .4, 1.);
        glClear(GL_COLOR_BUFFER_BIT);
        windowSwapBuffers();
    }

    cleanup();
    return 0;
}
