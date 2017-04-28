/**********************
 Assignment #3
 Domenic Bianchi
 0921557
 March 17, 2017
 This program reads in a web page configuration file and outputs the equivelent HTML code
 ************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "parseFile.h"

int main(int argc, char * argv[]) {

	if (argc < 2) {

		printf("Invalid number of parameters\n");
		return 1;
	}

	FILE * dataFile = fopen(argv[1], "r");

	if (dataFile == NULL) {

		printf("File could not be opened\n");
		return 1;
	}

	fclose(dataFile);

	/*Parse config file*/
	parseDataFile(argv[1], argv);
	
	return 0;
}
