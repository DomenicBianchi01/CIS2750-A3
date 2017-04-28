/*Domenic Bianchi*/
/*CIS 2750 Assignment 2*/
/*February 19, 2017*/
/*This program gets user input to create a new post*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "stream.h"
#include "addauthor.h"

class postEntry {

	userPost * formatEntry(char name[], char stream[], char text[], char dateAndTime[]) {

		userPost * current;

		/*Malloc memory for the Post node*/
		current = malloc(sizeof(userPost));

		/*If the malloc fails, do not create the node*/
		if (current == NULL)  {

			free(current);
			return NULL;
		}

		current->username = name;
		current->streamname = stream;
		current->date = dateAndTime;
		current->text = text;

		return current;
	}

	void getTimeAndDate(char string[]) {

		time_t currentTime;
		struct tm * timeStruct;

		/*Get the current data and time from the machine this program is being run on*/
		currentTime = time(NULL);
		timeStruct = localtime(&currentTime);

		/*Format the time string in order to be parsed correctly in the python script*/
		strftime(string, 256, "%b. %d, %Y %H:%M:%S\n", timeStruct);		
	}

	void submitPost(userPost * post) {

		/*updateStream is a library function*/
		updateStream(post);
		free(post);
	}
};

int main(int argc, char * argv[]) {

	class postEntry post;

	userPost * fullPost;

	char dateAndTime[256];
	clearArray(dateAndTime, 256);

	/*Get the time and date*/
	post.getTimeAndDate(dateAndTime);

	/*Format all the inputed information into a struct that can be sent to the stream library*/
	fullPost = post.formatEntry(argv[1], argv[2], argv[3], dateAndTime);

	/*Send struct to stream library*/
	post.submitPost(fullPost);

	return 0;
}
