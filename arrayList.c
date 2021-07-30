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

#include "arrayList.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

// Write declarations of static helper functions below:


/**  Creates a new, empty arrayList object such that:
 * 
 *      - capacity equals dimension
 *      - elemSz equals the size (in bytes) of the data objects the user
 *        will store in the arrayList
 *      - data points to a block of memory large enough to hold capacity
 *        user data objects
 *      - usage is zero
 *      - the user's comparison function is correctly installed
 *      - the user's clean function is correctly installed, if provided

 *   Returns:  new arrayList object
 */
arrayList* AL_create(uint32_t dimension, uint32_t elemSz, 
                     int32_t (*compareElems)(const void* const pLeft, const void* const pRight),
                     void (*cleanElem)(void* const pElem)) {
	
	arrayList* aList = malloc(1 * sizeof(arrayList));
	
	aList->data = calloc(dimension, elemSz);
	
	aList->elemSz = elemSz;
	
	aList->capacity = dimension;
	
	aList->usage = 0;
	
	aList->compareElems = compareElems;
	
	aList->cleanElem = cleanElem;
	
	return aList;
}

/**  Inserts the user's data object into the arrayList.
 * 
 *   Pre:     *pAL is a proper arrayList object
 *            *pElem is a valid user data object
 *   Post:    a copy of the user's data object has been inserted, at the
 *            position defined by the user's compare function
 *   Returns: true unless the insertion fails (unlikely unless it's not
 *            possible to increase the size of the arrayList
 */
bool AL_insert(arrayList* const pAL, const void* const pElem) {
	
	//if arrayList is null return false
	if(pAL == NULL) return false;
	
	//If we are adding to empty array
	if(pAL->usage == 0){
		memcpy(pAL->data, pElem, pAL->elemSz);
		pAL->usage++;
		return true;
	}
	
	//if array is full expand capacity
	if(pAL->capacity == pAL->usage){
		
		pAL->data = realloc(pAL->data, 2 * pAL->capacity * pAL->elemSz);
		pAL->capacity = (2 * pAL->capacity);	
	}
	
	int32_t comp;
	int index = 0;
	//find index where the insertion needs to happen
	for(int i = 0; i < pAL->usage; i++){
		
		comp = pAL->compareElems(AL_elemAt(pAL, i), pElem);
		
		if(comp < 0){
			index++;
		}
	}
	
	//move all the elements over
	for(int i = pAL->usage; i > index; i--){
		
		memcpy(pAL->data + (i * pAL->elemSz), pAL->data + ((i-1) * pAL->elemSz), pAL->elemSz);
		
	}
	//insert the new elment
	memcpy(pAL->data + (index * pAL->elemSz), pElem, pAL->elemSz);
	pAL->usage = pAL->usage +1;
	return true;
}

/**  Returns pointer to the data object at the given index.
 * 
 *   Pre:      *pAL is a proper arrayList object
 *             indexis a valid index for *pAL
 *   Returns:  pointer to the data object at index in *pAL; NULL if no
 *             such data object exists
 */
void* AL_elemAt(const arrayList* const pAL, uint32_t index) {
	
	if(pAL == NULL || index > (pAL->usage - 1)) return NULL;
	
	void* element = pAL->data + (index * pAL->elemSz);
	
	return element;
}

/**  Searches for a matching object in the arrayList.
 * 
 *   Pre:      *pAL is a proper arrayList object
 *             *pElem is a valid user data object
 *   Returns:  pointer to a matching data object in *pAL; NULL if no
 *             match can be found
 */
void* AL_find(const arrayList* const pAL, const void* const pElem) {
	
	for(int i = 0; i < pAL->usage; i++){
		if(pAL->compareElems(pAL->data + (i*pAL->elemSz), pElem) == 0){
			return pAL->data + (i * pAL->elemSz);
		}
	}	
	
	return NULL;
}

/**  Deallocates all dynamic content in the arrayList, including any
 *   dynamic content in the user data objects in the arrayList.
 * 
 *   Pre:   *pAL is a proper arrayList object
 *   Post:  the data array in *pAL has been freed, as has any dynamic
 *          memory associated with the user data objects that were in
 *          that array (via the user-supplied clean function); all the
 *          fields in *pAL are set to 0 or NULL, as appropriate.
 */
void AL_clean(arrayList* const pAL) {
	
	for(int i=0; i < pAL->usage; i++){
		pAL->cleanElem(pAL->data + (i * pAL->elemSz));
	}
	
	free(pAL->data);
	pAL->elemSz = 0;
	
	pAL->capacity = 0;
	
	pAL->usage = 0;
	
}

// Write definitions of static helper functions below:

