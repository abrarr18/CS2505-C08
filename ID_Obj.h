#ifndef ID_OBJ_H
#define ID_OBJ_H
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <inttypes.h>

struct _ID_Obj {
	uint32_t feature_ID;
	uint32_t offset;
};
typedef struct _ID_Obj ID_Obj;

ID_Obj* create_ID_Obj(uint32_t off, uint32_t FID);

int32_t compare_ID_Obj(const void* const pLeft, const void* const pRight);

void clean_ID_Obj(void* const pID);

#endif
