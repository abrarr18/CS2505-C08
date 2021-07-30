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

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>
#include "nextField.h"
#include "StringHashTable.h"
#include "IndexWrapper.h"
#include "Command_Processor.h"
#include <math.h>

#define PI 3.14159265358979323846 
#define RADIUS 6371.0088

struct _Record {
	uint32_t FID;
	char* name;
	char* type;
	char* state;
	char* county;
	char* long_DMS;
	char* lat_DMS;
	float long_DEC;
	float lat_DEC;
};

static int compare_Offsets(const void* left, const void* right);
static char* copyOf(const char* const str);

char* db_file_handler(char* line);

int table_sz_handler(char* line);

void exists_handler(char* line, const StringHashTable* const pTable, FILE* log);

int details_of_handler(char* line, const StringHashTable* const pTable, FILE* log, char* fileName);

void distance_handler(arrayList* index, char* line, FILE* log, char* fileName);



int handle_Script(char* script, char* log)
{
	
	FILE* scriptFile  = fopen(script, "r"); //open script file
	
	if ( scriptFile == NULL ) {
		printf("Script file %s was not found.\n", script);
		exit(2);
	}
	
	FILE* logFile = fopen(log, "w");
	
    
    if ( logFile == NULL ) {
		fprintf(logFile, "Output file %s could not be created.\n", log);
        fclose(scriptFile);
		exit(3);
    }
	
	char* db_file;
	int table_sz;
	
	char* line = calloc(sizeof(char), 501);

	int counter = 0;
	char* command;
	char* copy = calloc(sizeof(char), 501);
	arrayList* fidIndex;
	StringHashTable* fnameIndex = NULL;
	
	
	while(fgets(line, 500, scriptFile) != NULL){
		
		strcpy(copy, line);
		
		if(line[0] != ';'){ 
			
		
			command = strtok(copy, "\t");  
			
			if(strcmp(command, "db_file") == 0){
				counter = counter +1;
				db_file = db_file_handler(line);
				
				fidIndex = create_FID_Index(db_file);
			}
			else if(strcmp(command, "table_sz") == 0){
				counter = counter +1;
				
		        
				table_sz = table_sz_handler(line);
				 			
				
				fnameIndex = create_FName_Index(db_file, table_sz);
			}
			else if(strcmp(command, "exists") == 0){
				counter = counter +1;
				fprintf( logFile, "Cmd	%d:	%s\n", counter-2, line);
				
				exists_handler(line, fnameIndex, logFile);
			}
			else if (strcmp(command, "details_of") == 0){ 
				counter = counter +1;
				fprintf( logFile, "Cmd	%d:	%s\n", counter-2, line);
				details_of_handler(line, fnameIndex, logFile, db_file);
			}
			else if (strcmp(command, "distance_between") == 0){ 
				
				counter = counter +1;
				fprintf( logFile, "Cmd	%d:	%s\n", counter-2, line);
				
				distance_handler(fidIndex, line, logFile, db_file);			
			}
			else if (strcmp(command, "quit") == 0){ 
				counter = counter +1;
				fprintf( logFile, "Cmd	%d:	%s\n", counter-2, line);
				free(line);
				free(copy);
				clear_Table(fnameIndex);
				clear_List(fidIndex);
				
				fprintf( logFile, "Exiting...\n");
				fprintf( logFile, "------------------------------------------------------------\n");
			}
			else{
				printf("Command did not match\n");
			}
		}
		else { 
			if(counter >= 2){
				fprintf( logFile, "%s", line);
			}
		}
	}	
	
	return 0;
}

char* db_file_handler(char* line){
	
	char* fileName = calloc(201, sizeof(char));
	sscanf(line, "db_file\t%s\n", fileName);
	
	char* cp = copyOf(fileName);
	free(fileName);
	
	return cp;
}

int table_sz_handler(char* line){
	
	int number;
	sscanf(line, "table_sz\t%d\n", &number);
	
	return number;
}

void exists_handler(char* line, const StringHashTable* const pTable, FILE* log){
	
	
	char* name = calloc(501, sizeof(char));
	char* state = calloc(4, sizeof(char));
	
	sscanf(line, "exists\t%[^\t\n]\t%s\n", name, state);
	strcat(name, " ");
	strcat(name, state);
	
	char* nameCp = copyOf(name);
	free(name);
	
	uint32_t* offsets = StringHashTable_getLocationsOf(pTable, nameCp);
	
	if(offsets != NULL){
		uint32_t counter  = 0;
		
		int i = 0;
		while(offsets[i] != 0){
			counter++;
			i++;
		}
	
		fprintf( log, "%d occurrences: %s\n", counter, nameCp);
		fprintf( log, "------------------------------------------------------------\n");		
	}
	else{
		fprintf( log, "Not found in index: %s\n", nameCp);
		fprintf( log, "------------------------------------------------------------\n");				
	}
	free(state);
}

int details_of_handler(char* line, const StringHashTable* const pTable, FILE* log, char* fileName){
	char* name = calloc(501, sizeof(char));
	char* state = calloc(3, sizeof(char));
	
	sscanf(line, "details_of\t%[^\t\n]\t%s\n", name, state);
	strcat(name, " ");
	strcat(name, state);
	
	char* nameCp = copyOf(name);
	free(name);
	
	uint32_t counter  = 0;
	uint32_t* offsets = StringHashTable_getLocationsOf(pTable, nameCp);
	if(offsets == NULL) printf("offs are null");
	
	if(offsets != NULL){
		
		int i = 0;
		while(offsets[i] != 0){
			counter++;
			i++;
		}
	}
	
	if(offsets == NULL){
		
		fprintf( log, "Not found in index: %s\n", nameCp);
		fprintf( log, "------------------------------------------------------------\n");	
		free(state);
		return 0;
	}
	
	
	char* record = calloc(501, sizeof(char));
	Record* r = malloc(sizeof(Record));
	char* recordCp;
	FILE* dbFile  = fopen(fileName, "r");
	
	if ( dbFile == NULL ) {
		printf("Data base file %s was not found.\n", fileName);
		exit(6);
	}
	
	
	for(int i= 0; i < counter; i++ ){

		fseek(dbFile, offsets[i], SEEK_SET);
		fgets(record, 500, dbFile);
		recordCp = copyOf(record);
		
		sscanf(nextField(recordCp), "%u", &(r->FID));
		r->name = nextField(NULL);
		r->type = nextField(NULL);
		r->state = nextField(NULL);
		nextField(NULL);
		r->county = nextField(NULL);
		nextField(NULL);
		r->lat_DMS = nextField(NULL);
		r->long_DMS = nextField(NULL);
		sscanf(nextField(NULL), "%f", &(r->lat_DEC ));
		sscanf(nextField(NULL), "%f", &(r->long_DEC));
		
		char lat;
		char lon;
		int s1, m1, d1;
		int s2, m2, d2;
		int cord1, cord2;
		
		sscanf(r->lat_DMS, "%d%c", &cord1, &lat);
		s1 = cord1 % 100;
		m1 = (cord1 % 10000) - s1;
		d1 = (cord1 - (m1 + s1))/10000;
		m1 = m1/100;
		
		sscanf(r->long_DMS, "%d%c", &cord2, &lon);
		s2 = cord2 % 100;
		m2 = (cord2 % 10000) - s2;
		d2 = (cord2 - (m2 + s2))/10000;
		m2 = m2/100;
		
		fprintf( log, "FID:\t%d\n", r->FID);
		fprintf( log, "Name:\t%s\n", r->name);
		fprintf( log, "Type:\t%s\n", r->type);
		fprintf( log, "State:\t%s\n", r->state);
		fprintf( log, "County:\t%s\n", r->county);
		
		if(lon == 'W'){
			fprintf( log, "Longitude:\t%dd %dm %ds West\t(%f)\n", d2, m2, s2, r->long_DEC);
		}else{
			fprintf( log, "Longitude:\t%dd %dm %ds East\t(%f)\n", d2, m2, s2, r->long_DEC);
		}
		
		if(lat == 'N'){
			fprintf( log, "Latitude:\t%dd %dm %ds North\t(%f)\n", d1, m1, s1, r->lat_DEC);
		}else{
			fprintf( log, "Latitude:\t%dd %dm %ds South\t(%f)\n", d1, m1, s1, r->lat_DEC);
		}
		fprintf( log, "\n");	
	}	
	
	fprintf( log, "------------------------------------------------------------\n");
	free(state);
	free(record);
	free(r);
	return 1;
}

void distance_handler(arrayList* index, char* line, FILE* log, char* fileName){
	
	uint32_t fID1;
	uint32_t fID2;
	sscanf(line, "distance_between\t%u\t%u\n", &fID1, &fID2);
	
	fID1 = fID1 -1;
	fID2 = fID2- 1;
	
	ID_Obj* o1 = create_ID_Obj(0, fID1);
	ID_Obj* o2 = create_ID_Obj(0, fID2);
	
	if(AL_find(index, o1) == NULL){
		
		fprintf( log, "Invalid feature ID: %d\n", fID1);
		fprintf( log, "------------------------------------------------------------\n");	
	}
	else if(AL_find(index, o2) == NULL){
		fprintf( log, "Invalid feature ID: %d\n", fID2);
		fprintf( log, "------------------------------------------------------------\n");
	}
	else {
		
		uint32_t off1;
		uint32_t off2;
		char* rec1 = calloc(501, sizeof(char));
		char* rec2 = calloc(501, sizeof(char));
		char* rec1_cp;
		char* rec2_cp;
		
		ID_Obj* x1 = malloc(sizeof(ID_Obj));
		x1 = AL_find(index, o1);
		off1 = x1->offset;
		
		ID_Obj* x2 = malloc(sizeof(ID_Obj));
		x2 = AL_find(index, o2);
		off2 = x2->offset;
	
		FILE* dbFile  = fopen(fileName, "r");
	
		if ( dbFile == NULL ) {
			printf("Data base file %s was not found.\n", fileName);
			exit(6);
		}
		
		fseek(dbFile, off1, SEEK_SET);
		fgets(rec1, 500, dbFile);
		rec1_cp = copyOf(rec1);
		free(rec1);
		
		fseek(dbFile, off2, SEEK_SET);
		fgets(rec2, 500, dbFile);
		rec2_cp = copyOf(rec2);
		free(rec2);
		
		float lat1, lon1, lat2, lon2;
		char* dms_la1 = calloc(201, sizeof(char));
		char* dms_la2 = calloc(201, sizeof(char));
		char* dms_lo1 = calloc(201, sizeof(char));
		char* dms_lo2 = calloc(201, sizeof(char));
		char* name1 = calloc(201, sizeof(char));
		char* state1 = calloc(201, sizeof(char));
		char* name2 = calloc(201, sizeof(char));
		char* state2 = calloc(201, sizeof(char));
		
		nextField(rec1_cp);
		name1 = nextField(NULL);
		nextField(NULL);
		state1 = nextField(NULL);
		nextField(NULL);
		nextField(NULL);
		nextField(NULL);
		dms_la1 = nextField(NULL);
		dms_lo1 = nextField(NULL);
		sscanf(nextField(NULL), "%f", &lat1);
		sscanf(nextField(NULL), "%f", &lon1);
		
		nextField(rec2_cp);
		name2 = nextField(NULL);
		nextField(NULL);
		state2 = nextField(NULL);
		nextField(NULL);
		nextField(NULL);
		nextField(NULL);
		dms_la2 = nextField(NULL);
		dms_lo2 = nextField(NULL);
		sscanf(nextField(NULL), "%f", &lat2);
		sscanf(nextField(NULL), "%f", &lon2);
		
		lat1 = lat1 * PI / 180;
		lon1 = lon1 * PI / 180;
		lon2 = lon2 * PI / 180;
		lat2 = lat2 * PI / 180;
		
		float angle = acos((sin(lat1)*sin(lat2)) + (cos(lat1)*cos(lat2)*cos(fabs(lon1 - lon2))));
		
		float distance = RADIUS * angle;
		
		char la1, la2;
		char lo1, lo2;
		int s1, m1, d1, s3, m3, d3;
		int s2, m2, d2, s4, m4, d4;
		int cord1, cord2, cord3, cord4;
		
		
		sscanf(dms_la1, "%d%c", &cord1, &la1);
		s1 = cord1 % 100;
		m1 = (cord1 % 10000) - s1;
		d1 = (cord1 - (m1 + s1))/10000;
		m1 = m1/100;
		
		sscanf(dms_lo1, "%d%c", &cord2, &lo1);
		s2 = cord2 % 100;
		m2 = (cord2 % 10000) - s2;
		d2 = (cord2 - (m2 + s2))/10000;
		m2 = m2/100;
		
		sscanf(dms_la2, "%d%c", &cord3, &la2);
		s3 = cord3 % 100;
		m3 = (cord3 % 10000) - s3;
		d3 = (cord3 - (m3 + s3))/10000;
		m3 = m3/100;
		
		sscanf(dms_lo2, "%d%c", &cord4, &lo2);
		s4 = cord4 % 100;
		m4 = (cord4 % 10000) - s4;
		d4 = (cord4 - (m4 + s4))/10000;
		m4 = m4/100;
		
		fprintf( log, "First:\t( %3dd %2dm %2ds ", d2, m2, s2);
		if(lo1 == 'W'){
			fprintf(log, "West, ");
		}
		else{
			fprintf(log, "East, ");
		}
		fprintf( log, "%3dd %2dm %2ds ", d1, m1, s1);
		if(la1 == 'S'){
			fprintf(log, "South ) ");
		}
		else{
			fprintf(log, "North ) ");
		}
		fprintf( log, "%s, %s\n", name1, state1);
		
	
	
		fprintf( log, "Second:\t( %3dd %2dm %2ds ", d4, m4, s4);
		if(lo2 == 'W'){
			fprintf(log, "West, ");
		}
		else{
			fprintf(log, "East, ");
		}
		fprintf( log, "%3dd %2dm %2ds ", d3, m3, s3);
		if(la2 == 'S'){
			fprintf(log, "South ) ");
		}
		else{
			fprintf(log, "North ) ");
		}
		fprintf( log, "%s, %s\n", name2, state2);
		
		
		fprintf( log, "Distance: %0.1fkm\n", distance);
		fprintf( log, "------------------------------------------------------------\n");	
		free(dms_la1);
		free(dms_la2);
		free(dms_lo1);
		free(dms_lo2);
		free(name1);
		free(state1);
		free(name2);
		free(state2);
	}	
}

static int compare_Offsets(const void* left, const void* right){

	if(*(uint32_t*)left < *(uint32_t*)right){
		return -1;
	}
	else if(*(uint32_t*)left > *(uint32_t*)right){
		return 1;
	}
	else {
		return 0;
	}
}


static char* copyOf(const char* const str) {
	
	if ( str == NULL ) {
		return calloc(1, sizeof(char));
	}
	
	char* copy = calloc(strlen(str) + 1, sizeof(char));
    strcpy(copy, str);	
	
	return copy;
}
