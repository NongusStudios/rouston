#include "inc/jvcc.h"

#include <string.h>
#include <stdbool.h>

void jvccTableInit(JvccTable* table){
    table->sections = VEC_INIT_EMPTY(JvccSection);
}

bool parseSourceToTable(JvccTable* table, const char* src, size_t len){
    char current_string[256] = {};
    size_t current_string_len = 0;

    JvccSection* current_section = NULL;
    JvccEntry* current_entry = NULL;

    bool in_comment = false;

    for(size_t i = 0; i < len; i++){
        char ch = src[i];

        if(in_comment && ch != '\n') continue;

        switch(ch){
            case '#':
                in_comment = true;
                break;
            case '\n':
            case ' ':
            case '\t':
                break;
            case '[':
                vecPush(VEC_REF(table->sections), VecPos_BACK, NULL);
                current_section = vecLast(table->sections);

                current_section->entries = VEC_INIT_EMPTY(JvccEntry);
                strncpy_s(current_section->name, JVCC_MAX_STRING_LENGTH,
                          current_string, current_string_len);
                current_string_len = 0;
                break;
            case ']':
                current_section = NULL;
                break;
            case ':':
                if(!current_section){
                    fprintf(stderr, ": used outside of a section\n");
                    return false;
                }

                vecPush(VEC_REF(current_section->entries), VecPos_BACK, NULL);
                current_entry = vecLast(current_section->entries);
                strncpy_s(current_entry->name, JVCC_MAX_STRING_LENGTH, current_string, current_string_len);
                current_string_len = 0;
                break;
            case ',':
                if(!current_entry && current_section) {
                    fprintf(stderr, "Expected value after :\n");
                    return false;
                } else if(!current_entry) break;

                strncpy_s(current_entry->str, JVCC_MAX_STRING_LENGTH, current_string, current_string_len);
                current_string_len = 0;
                current_entry = NULL;
                break;
            default:
                current_string[current_string_len++] = ch;
        }
    }

    return true;
}

bool jvccTableInitFromFile(JvccTable* table, const char* path){
    jvccTableInit(table);

    size_t len = 0;
    FILE* fp = fileOpen(path, "r", &len);

    char* src = memAlloc(len + 1);
    fread(src, 1, len, fp);
    fclose(fp);

    if(!parseSourceToTable(table, src, len)) return false;
    memFree(src);

    return true;
}

void jvccTableFree(JvccTable* table){
    VEC_FOREACH(JvccSection, sec, table->sections){
        vecFree(sec->entries);
    }
    vecFree(table->sections);
}
