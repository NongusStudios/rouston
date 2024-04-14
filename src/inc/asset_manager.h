#pragma once

#include "state.h"

#define ASSET_PATH "../assets"
#define ASSET_HASH_INIT_CAP 128
#define ASSET_IMAGE_TYPE "IMAGE"

const char* assetPath(const char* path);

Error initAssetManager();
void freeAssetManager();

const Asset* getAsset(const char* handle);
