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

#include <stdio.h>
#include "Command_Processor.h"

int main(int argc, char **argv)
{
	if(argc < 3){
		printf("Error: File not specified");
		exit(1);
	}	
	handle_Script(argv[1], argv[2]);
		
	return 0;
}

