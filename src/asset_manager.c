#include "inc/asset_manager.h"

int assetCmp(const void* a, const void* b, void* udata){
    return strcmp(((Asset*)a)->name, ((Asset*)b)->name);
}

uint64_t assetHash(const void* item, uint64_t seed0, uint64_t seed1){
    Asset* asset = (Asset*)item;
    return hashmap_sip(asset->name, strlen(asset->name), seed0, seed1);
}

const char* assetPath(const char* path){
    static char str[256] = {};
    memset(str, 0, 256);
    snprintf(str, 256, "%s/%s", ASSET_PATH, path);
    return str;
}

Error setImageProperty(const char* name, const char* str, GLint* image_filtering){
    if (strncmp(name, "filtering", JVCC_MAX_STRING_LENGTH) == 0) {
        if(strncmp(str, "NEAREST", JVCC_MAX_STRING_LENGTH) == 0){
            *image_filtering = GL_NEAREST;
        } else if(strncmp(str, "LINEAR", JVCC_MAX_STRING_LENGTH) == 0){
            *image_filtering = GL_LINEAR;
        } else {
            return ERROR("Invalid option for image property 'filtering' in load.jvcc");
        }
    } else {
        return ERROR("Unexpected asset property in load.jvcc");
    }

    return SUCCESS;
}

Error initAssetManager(){
    AssetManger* asset_manager = &getAppState()->asset_manager;

    asset_manager->assets = hashmap_new(sizeof(Asset), ASSET_HASH_INIT_CAP, 0, 0,
                                        assetHash, assetCmp, NULL, NULL);

    JvccTable load_jvcc;
    if(!jvccTableInitFromFile(&load_jvcc, assetPath("load.jvcc"))){
        return ERROR("Failed to parse load.jvcc!");
    }

    VEC_FOREACH(JvccSection, section, load_jvcc.sections){
        Asset asset = {};
        strncpy_s(asset.name, JVCC_MAX_STRING_LENGTH,
                  section->name, strnlen(section->name, JVCC_MAX_STRING_LENGTH));

        assert(vecLen(section->entries) >= 1);
        if(strncmp(section->entries[0].name, "type", JVCC_MAX_STRING_LENGTH) != 0){
            return ERROR("Expected first property of all sections in load.jvcc to be 'type'");
        }

        const char* type = section->entries[0].str;
        AssetType asset_type;
        if(strncmp(type, ASSET_IMAGE_TYPE, JVCC_MAX_STRING_LENGTH) == 0){
            asset_type = AssetType_TEXTURE;
        } else {
            return ERROR("Unexpected asset type in load.jvcc");
        }

        Error err;
        GLint image_filtering = 0;
        for(size_t i = 1; i < vecLen(section->entries); i++){
            const char* name = section->entries[i].name;
            const char* str = section->entries[i].str;
            switch(asset_type) {
                case AssetType_TEXTURE:
                    err = setImageProperty(name, str, &image_filtering);
                    if(err.type != ErrorType_SUCCESS) return err;
                    break;
            }
        }

        switch (asset_type) {
            case AssetType_TEXTURE: {
                int w, h, channels;
                stbi_uc* pixels = stbi_load(assetPath(section->name), &w, &h,
                                            &channels, STBI_rgb_alpha);

                AssetTexture t = {
                    .width = w,
                    .height = h,
                    .format = GL_RGBA,
                };

                glCreateTextures(GL_TEXTURE_2D, 1, &t.texture);
                glTextureParameteri(t.texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTextureParameteri(t.texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTextureParameteri(t.texture, GL_TEXTURE_MIN_FILTER, image_filtering);
                glTextureParameteri(t.texture, GL_TEXTURE_MAG_FILTER, image_filtering);

                glTextureStorage2D(t.texture, 1, GL_RGBA8, w, h);
                glTextureSubImage2D(t.texture, 0, 0, 0, w, h,
                           GL_RGBA, GL_UNSIGNED_BYTE, pixels);

                asset = (Asset){
                    .type = asset_type,
                    .texture = t,
                };
                stbi_image_free(pixels);
                break;
            }
        }

        strncpy(asset.name, section->name, JVCC_MAX_STRING_LENGTH);
        hashmap_set(asset_manager->assets, &asset);
    }

    jvccTableFree(&load_jvcc);

    return SUCCESS;
}
void freeAssetManager(){
    AssetManger* asset_manager = &getAppState()->asset_manager;

    hashmap_free(asset_manager->assets);
}

const Asset* getAsset(const char* handle){
    AssetManger* asset_manager = &getAppState()->asset_manager;
    Asset asset = {};
    strncpy_s(asset.name, JVCC_MAX_STRING_LENGTH, handle, strlen(handle)+1);
    return (Asset*)hashmap_get(asset_manager->assets, &asset);
}
