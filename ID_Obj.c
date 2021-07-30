//  On my honor:
// - I have not discussed the C language code in my program with
//   anyone other than my instructor or the teaching assistants
//   assigned to this course.
//
// - I have not used C language code obtained from another student,
//   the Internet, or any other unauthorized source, either modified
//   or unmodified.
//
// - If any C language code or documentation used in my program
//   was obtained from an authorized source, such as a text book or
//   course notes, that has been clearly noted with a proper citation
//   in the comments of my program.
//
// - I have not designed this program in such a way as to defeat or
//   interfere with the normal operation of the Curator System.
//
//   <Abrar Islam>
//   <abrarr18>
#include "ID_Obj.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>
#include <inttypes.h>

static char* copyOf(const char* const str);

ID_Obj* create_ID_Obj(uint32_t off, uint32_t FID){
	
	ID_Obj* object = malloc(sizeof(ID_Obj));
	
	object->feature_ID = FID;
	object->offset = off;
	return object;
}

int32_t compare_ID_Obj(const void* const pLeft, const void* const pRight){
	
	const ID_Obj* left  = (ID_Obj*) pLeft;
	const ID_Obj* right = (ID_Obj*) pRight;
	
	if(left->feature_ID < right->feature_ID){
		return -1;
	}
	else if(left->feature_ID > right->feature_ID){
		return 1;
	}
	else {
		return 0;
	}
}

void clean_ID_Obj(void* const pID){
}

static char* copyOf(const char* const str) {
	
	if ( str == NULL ) {
		return calloc(1, sizeof(char));
	}
	
	char* copy = calloc(strlen(str) + 1, sizeof(char));
    strcpy(copy, str);	
	
	return copy;
}
