// nextField.h
#ifndef NEXTFIELD_H
#define NEXTFIELD_H
#include <stdbool.h>

#define MAX_FIELDLENGTH 100

/**  Returns string containing copy of next field of a GIS record.
 *   
 *   Pre:
 *      When beginning to parse a GIS record string, pChar points to the 
 *      first character in a GIS record string.
 *      When parsing a GIS record string for subsequent fields, pChar is
 *      set to NULL.
 * 
 *   Returns:
 *      Pointer to a dynamically-allocated C-string holding the contents
 *        of the next field; this may be an empty string, but will never
 *        be NULL.
 * 
 *   How it works:
 * 
 *   If pChar is NULL, the function begins parsing at the character after
 *   the end of the previous field (which was presumably read during an
 *   earlier call to nextField() on the same GIS record string).
 * 
 *   If pChar is not NULL, the function begins parsing at the first 
 *   character in the GIS record string.
 * 
 *   If pChar points to a delimiter ('|'), an empty string is returned.
 * 
 *   Otherwise, a dynamically-allocated char array is created to hold the
 *   next field, using the assumption that no GIS record field will 
 *   contain more than 200 characters.
 * 
 *   Next, the function uses sscanf() to read characters until a delimiter
 *   is found, or the end of the GIS record string is reached.
 * 
 *   Finally, realloc() is used to shrink the array holding the field to
 *   the minimum suitable size.
 * 
 *   If pChar is NULL, the function begins parsing at the poi
 * 
 *   Hints:
 *      Use sscanf() with a suitable scanset to read the next field.  See
 *      the notes on String I/O for examples.
 *      The function keeps track of the position in the GIS record string
 *      by making use of a static local variable.
 */
char* nextField(const char* const pChar);

#endif
