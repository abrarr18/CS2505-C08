#ifndef INDEXWRAPPER_H
#define INDEXWRAPPER_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <inttypes.h>
#include <assert.h>

#include "ID_Obj.h"
#include "nextField.h"
#include "StringHashTable.h"
#include "arrayList.h"

arrayList* create_FID_Index(char* fileName);

StringHashTable* create_FName_Index(char* fileName, uint32_t size);

void clear_Table(StringHashTable* const pTable);

void clear_List(arrayList* const pAL);

uint32_t elfhash(const char* str);

#endif
