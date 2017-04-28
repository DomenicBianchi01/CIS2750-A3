/*Domenic Bianchi
CIS 2750 Assignment 2
February 19, 2017
This program adds or removes a user from a stream or multiple streams*/

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "addauthor.h"
#include "stream.h"

int main(int argc, char * argv[]) {

	char list[256];
	char name[256];
	char statusMessage[256];
	bool removeFlag = false;
	clearArray(list, 256);
	clearArray(name, 256);
	clearArray(statusMessage, 256);

	if (strcmp(argv[1], "-r") == 0) {

		removeFlag = true;
	}

	updateStreams(argv[3], argv[2], statusMessage, removeFlag);

	printf("%s", statusMessage);

	return 0;
}

void updateStreams(char username[], char streamList[], char statusMessage[], bool flag) {

	/*If falg is true, that means the -r flag was included in the user input and the user should be removed for the stream(s)*/
	if (flag == true) {

		removeUser(username, streamList);
	}
	else {

		addUser(username, streamList, statusMessage);
	}
}

void clearArray(char string[], int length) {

	int j = 0;

	/*Set entire array to null terminators*/
	for (j = 0; j < length; j++) {
		
		string[j] = '\0';
	}
}