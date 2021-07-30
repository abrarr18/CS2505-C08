#ifndef COMMAND_PROCESSOR_H
#define COMMAND_PROCESSOR_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include "nextField.h"
#include "StringHashTable.h"
#include "IndexWrapper.h"
#include <math.h>

#include <stdio.h>
#define PI 3.14159265358979323846 // best approximation as double
#define RADIUS 6371.0088

struct _Record;
typedef struct _Record Record;

int handle_Script(char* script, char* log);

char* db_file_handler(char* line);

int table_sz_handler(char* line);

void exists_handler(char* line, const StringHashTable* const pTable, FILE* log);

int details_of_handler(char* line, const StringHashTable* const pTable, FILE* log, char* fileName);

void distance_handler(arrayList* index, char* line, FILE* log, char* fileName);

#endif
