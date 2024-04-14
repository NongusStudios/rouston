#pragma once

#include <stdbool.h>
#include "utils.h"
#define JVCC_MAX_STRING_LENGTH 256

typedef struct JvccEntry {
    char name[JVCC_MAX_STRING_LENGTH];
    char str[JVCC_MAX_STRING_LENGTH];
} JvccEntry;

typedef struct JvccSection {
    char name[JVCC_MAX_STRING_LENGTH];
    VEC(JvccEntry) entries;
} JvccSection;

typedef struct JvccTable {
    VEC(JvccSection) sections;
} JvccTable;

void jvccTableInit(JvccTable* table);
bool jvccTableInitFromFile(JvccTable* table, const char* path);
void jvccTableFree(JvccTable* table);

//size_t jvccTableSectionInit(JvccTable* table, const char* name);
//size_t jvccTableSectionAddEntry(JvccTable* table, size_t section, const char* name, const char* str);
