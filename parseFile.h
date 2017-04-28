#ifndef __DBIANCHI_PARSEFILE__
#define __DBIANCHI_PARSEFILE__

/**
 * parseDataFile
 * Gets data from a config file and creates valid HTML which is outputed back to PHP.
 * IN: char file[], char * argv[]
 * OUT: None
 * POST: HTML is created and sent back to PHP through the use of print statements (output is redirected)
 * ERROR: If the config file is not formatted correctly, errors may occur
 **/
void parseDataFile(char file[], char * argv[]);

/**
 * clearArray
 * Clears all the elements of a char array so the array can be re-used for other purposes elsewhere in the program
 * IN: char string[], int length
 * OUT: None
 * POST: All elements of the char array are null terminated
 * ERROR: If the length variable (int length) is shorter than the actually length of the char array, not all elements of the array will be cleared which will result in errors when the array is re-used else where in the program
 **/
void clearArray(char string[], int length);

#endif
