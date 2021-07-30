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

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  
#include <inttypes.h>
#include <assert.h>
#include "ID_Obj.h"
#include "nextField.h"
#include "StringHashTable.h"
#include "IndexWrapper.h"
#include "arrayList.h"

static char* copyOf(const char* const str);


arrayList* create_FID_Index(char* fileName){
	arrayList* FID_Index = AL_create(256, sizeof(ID_Obj), compare_ID_Obj, clean_ID_Obj);
	FILE* gisFile  = fopen(fileName, "r");
	if ( gisFile == NULL ) {
		printf("Data base file %s was not found.\n", fileName);
		exit(4);
	}
	char* line = calloc(sizeof(char), 501);
	fgets(line, 500, gisFile);
	ID_Obj* x;
	uint32_t a;
	uint32_t b;
		
	while(fgets(line, 500, gisFile)){
		a = ftell(gisFile);
		sscanf(nextField(line), "%u", &b);
		x = create_ID_Obj(a, b);
		AL_insert(FID_Index, x);
	}
	
	free(line);
	return FID_Index;
}


StringHashTable* create_FName_Index(char* fileName, uint32_t size){
	StringHashTable* FName_Index = StringHashTable_create(size, *elfhash);
	FILE* gisFile  = fopen(fileName, "r");
	if ( gisFile == NULL ) {
		printf("Data base file %s was not found.\n", fileName);
		exit(5);
	}
	
	char* line = calloc(501, sizeof(char));
	char* fname = calloc(501, sizeof(char));
	char* state = calloc(4, sizeof(char));
	char* nameCp;
	uint32_t offset;
	line = fgets(line, 500, gisFile);
	
	while(line != NULL){
		
		offset = ftell(gisFile);
		line = fgets(line, 500, gisFile);
		nextField(line);
		strcpy(fname, nextField(NULL));
		nextField(NULL);
		state = nextField(NULL);
		strcat(fname, " ");
		strcat(fname, state);
		nameCp = copyOf(fname);
		
		StringHashTable_addEntry(FName_Index, nameCp, offset);
	}
	
	free(line);
	free(fname);
	free(state);
	
	return FName_Index;
}

void clear_Table(StringHashTable* const pTable){
	StringHashTable_clear(pTable);
}

void clear_List(arrayList* const pAL){
	AL_clean(pAL);
}

uint32_t elfhash(const char* str) {
	
	assert(str != NULL ); 		
	uint32_t hashvalue = 0,
	high; 						
			
	
	while ( *str ) { 			
		
		hashvalue = (hashvalue << 4) + *str++; 	
												
		if ( high = (hashvalue & 0xF0000000) ) {
			hashvalue = hashvalue ^ (high >> 24);
		} 
		hashvalue = hashvalue & 0x7FFFFFFF; 	
	}
return hashvalue;
}

static char* copyOf(const char* const str) {
	
	if ( str == NULL ) {
		return calloc(1, sizeof(char));
	}
	
	char* copy = calloc(strlen(str) + 1, sizeof(char));
    strcpy(copy, str);	
	
	return copy;
}


