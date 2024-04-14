#include "inc/state.h"

#include <memory.h>

AppState g_app_state;

void initAppState(){
    memset(&g_app_state, 0, sizeof(AppState));
}

AppState* getAppState() { return &g_app_state; }

bool handleError(Error err){
    switch(err.type){
        case ErrorType_SUCCESS: break;
        case ErrorType_WARNING: printf("WARNING: %s\n", err.what); break;
        case ErrorType_ERROR:
            fprintf(stderr, "ERROR: %s\n", err.what);
            return false;
    }
    return true;
}
