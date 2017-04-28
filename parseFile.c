#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include "parseFile.h"

void parseDataFile(char file[], char * argv[]) {

	int tempArgsCounter = 0;
	int tempTextCounter = 0;
	int actionTextCounter = 0;
	int tempValueCounter = 0;
	int saveNameCounter = 0;
	int counter = 0;
	int inputCounter = 0;
	int radioNameCounter = 0;
	int radio1Counter = 0;
	int radio2Counter = 0;
	int extraTagsCounter = 0;
	int parameterType = 0;
	char input[500];
	char tempValue[500];
	char tempArgs[500];
	char tempText[500];
	char actionText[500];
	char saveName[500];
	char radioName[500];
	char radio1[500];
	char radio2[500];
	char extraTags[1000];
	char randomString[500];
	char tempSize = '3';
	char tag;
	bool inBrackets = false;
	bool inQuotations = false;
	bool textFound = false;
	bool sizeFound = false;
	bool linkFound = false;
	bool valueFound = false;
	bool firstTime = true;
	bool foundFflag = false;
	bool radioNameFound = false;
	bool radio1Found = false;
	bool radio2Found = false;
	bool foundExtraTags = false;
	bool randomText = true;
	bool printRandomText = false;
	FILE * fp = NULL;

	clearArray(input, 500);
	clearArray(tempArgs, 500);
	clearArray(tempText, 500);
	clearArray(actionText, 500);
	clearArray(tempValue, 500);
	clearArray(saveName, 500);
	clearArray(radioName, 500);
	clearArray(radio1, 500);
	clearArray(radio2, 500);
	clearArray(extraTags, 1000);
	clearArray(randomString, 500);

	fp = fopen(file, "r");

	printf("<html>\n<body>\n");

	while (fgets(input, 500, fp) != NULL) {

		int i = 0;

		for (i = 0; i < strlen(input); i++) {

			/*If whitespace is found outside of quotations, skip the character*/
			if (inQuotations == false && input[i] == ' ') {

				continue;
			}

			/*If a quotation mark is found then inverse the boolean value that determines if we are inside a quotation block or not*/
			if (input[i] == '"') {

				inQuotations = !inQuotations;

				if (tag == 'e') {

					i = i + 1;
				}
			}

			/*Start of tag*/
			if (input[i] == '.' && inBrackets == false && inQuotations == false) {

				tag = input[i+1];
				i = i + 3;
				inBrackets = true;
				
				if (tag == 't' || tag == 'h') {

					i = i - 1;
					continue;
				}
				else if (tag == 'd') {

					i = i - 1;
					continue;
				}
				else if (tag == 'k') {

					i = i - 1;
					printf("<br>\n");
					continue;
				}
				else if (tag == 'y' || tag == 'e') {

					i = i + 3;
					continue;
				}
			}
			/*Finish with the tag (end of the tag). Print out the HTML needed to display the tag*/
			else if (input[i] == ')' && inBrackets == true && inQuotations == false) {

				inBrackets = false;

				if (tag == 'h') {

					/*Default header*/
					if (strlen(tempText) == 0) {

						strcpy(tempText, "HEADING");
					}
					else {

						tempText[0] = ' ';
						tempText[strlen(tempText)-1] = '\0';
					}

					if (randomText == true || printRandomText == true) {

						char tempString[500];
						clearArray(tempString, 500);

						strcpy(tempString, extraTags);
						strcat(tempString, " ");
						strcat(tempString, randomString);

						if (foundExtraTags == false) {

							strcat(tempString, " ");
							strcat(tempString, tempArgs);
						}

						printf("<h%c %s>%s</h%c>\n", tempSize, tempString, tempText, tempSize);
					}
					else {

						printf("<h%c %s>%s</h%c>\n", tempSize, extraTags, tempText, tempSize);
					}
				}
				else if (tag == 'd') {

					if (strlen(tempArgs) == 0) {

						printf("<hr>\n");
					}
					else {

						printf("<hr %s>\n", tempArgs);
					}
				}
				else if (tag == 't') {

					char tempString[500];
					char input2[500];
					char finalInput[500];
					clearArray(tempString, 500);
					clearArray(input2, 500);
					clearArray(finalInput, 500);
					int z = 1;

					/*If the text needs to be retrieved from a file*/
					if (parameterType == 2) {

						FILE * fp2 = NULL;

						/*Get file name*/
						for (z = 1; z < strlen(tempText)-1; z++) {

							tempString[z-1] = tempText[z];
						}

						if (access(tempString, F_OK) != -1) {

							fp2 = fopen(tempString, "r");

							/*Print results of the text*/
							while (fgets(input2, 500, fp2) != NULL) {

								strcat(finalInput, input2);
							}

							fclose(fp2);
						}
					}
					else if (strlen(tempText) != 0) {

						for (z = 1; z < strlen(tempText)-1; z++) {

							tempString[z-1] = tempText[z];
						}

						strcpy(finalInput, tempString);
					}

					if (strlen(finalInput) == 0) {

						strcpy(finalInput, "Default text");
					}
					
					if (strlen(extraTags) == 0 && randomText == false && printRandomText == false) {

						printf("<p>%s</p>\n", finalInput);
					}
					else if (randomText == true || printRandomText == true) {

						char tempString3[500];
						clearArray(tempString3, 500);

						strcpy(tempString3, extraTags);
						strcat(tempString3, " ");
						strcat(tempString3, randomString);

						if (foundExtraTags == false) {

							strcat(tempString3, " ");
							strcat(tempString3, tempArgs);
						}

						printf("<p %s>%s</p>\n", tempString3, finalInput);
					}
					else {

						printf("<p %s>%s</p>\n", extraTags, finalInput);
					}
				}
				else if (tag == 'i') {

					int s = 0;
					int numOfFields = 1;
					int counter = 0;
					char temp[500];
					char temp2[500];
					char temp3[500];

					if (strlen(extraTags) == 0 && randomText == false && printRandomText == false) {

						printf("<form action=%s method=\"post\">\n", actionText);
					}
					else if (randomText == true || printRandomText == true) {

						char tempString3[500];
						clearArray(tempString3, 500);

						strcpy(tempString3, extraTags);
						strcat(tempString3, " ");
						strcat(tempString3, randomString);

						if (foundExtraTags == false) {

							strcat(tempString3, " ");
							strcat(tempString3, tempArgs);
						}

						printf("<form %s action=%s method=\"post\">\n", tempString3, actionText);
					}
					else {

						printf("<form %s action=%s method=\"post\">\n", extraTags, actionText);
					}

					/*Print out the HTML for each input field*/
					while (inputCounter != 0) {

						numOfFields = inputCounter;
						counter = 0;

						clearArray(temp, 500);
						clearArray(temp2, 500);
						clearArray(temp3, 500);

						/*Get label/text of the input field*/
						for (s = 0; s < strlen(tempText); s++) {

							if (numOfFields == 1) {

								temp[counter] = tempText[s];
								counter = counter + 1;
							}

							if (tempText[s] == '"' && tempText[s+1] == '"') {

								numOfFields = numOfFields - 1;
							}
						}

						counter = 0;
						numOfFields = inputCounter;

						/*Get name of the input field*/
						for (s = 0; s < strlen(saveName); s++) {

							if (numOfFields == 1) {

								temp2[counter] = saveName[s];
								counter = counter + 1;
							}

							if (saveName[s] == '"' && saveName[s+1] == '"') {

								numOfFields = numOfFields - 1;
							}
						}

						counter = 0;
						numOfFields = inputCounter;

						/*Get default value of the input field*/
						for (s = 0; s < strlen(tempValue); s++) {

							if (numOfFields == 1) {

								temp3[counter] = tempValue[s];
								counter = counter + 1;
							}

							if (tempValue[s] == '"' && tempValue[s+1] == '"') {

								numOfFields = numOfFields - 1;
							}
						}

						temp[0] = ' ';
						temp[strlen(temp)-1] = '\0';

						if (strlen(temp3) == 1) {

							strcat(temp3, "\"");
						}

						printf("%s <input type=\"text\" name=%s value=%s><br>\n", temp, temp2, temp3);

						inputCounter = inputCounter - 1;
					}
					
					printf("<input type=\"submit\">\n");
					printf("</form>\n");
				}
				else if (tag == 'e') {

					int z = 1;
					char tempString[500];
					char executableName[500];
					clearArray(tempString, 500);
					clearArray(executableName, 500);

					/*Get executable name*/
					for (z = 0; z < strlen(tempArgs); z++) {

						tempString[z] = tempArgs[z];
					}

					strcpy(executableName, "./");
					strcat(executableName, tempString);

					/*Check if the executable is in the current directory*/
					if (access(executableName, F_OK) != -1) {
   						 system(executableName);
					} 
					else {
   						
   						strcpy(executableName, "./bin/");
						strcat(executableName, tempString);

						/*Check if the executable is in the local bin directory*/
						if (access(executableName, F_OK) != -1) {
   							system(executableName);
						} 
						else {

							strcpy(executableName, "/bin/");
							strcat(executableName, tempString);

							/*printf("%s\n", executableName);*/

							/*Check if the executable is in the system bin directory*/
							if (access(executableName, F_OK) != -1) {
   								 system(executableName);
							}
						}
					}
				}
				else if (tag == 'y') {

					int z = 1;
					char tempString[500];
					char executableName[500];
					clearArray(tempString, 500);
					clearArray(executableName, 500);

					/*Get executable name*/
					for (z = 1; z < strlen(tempArgs)-1; z++) {

						tempString[z-1] = tempArgs[z];
					}

					strcpy(executableName, "./");
					strcat(executableName, tempString);

					printf("---EXECUTE---: %s", executableName);

					foundFflag = true;
				}
				else if (tag == 'b') {

					if (strlen(extraTags) == 0 && randomText == false && printRandomText == false) {

						printf("<form action=%s method=\"post\">\n", tempValue);
					}
					else if (randomText == true || printRandomText == true) {

						char tempString[500];
						clearArray(tempString, 500);

						strcpy(tempString, extraTags);
						strcat(tempString, " ");
						strcat(tempString, randomString);

						if (foundExtraTags == false) {

							strcat(tempString, " ");
							strcat(tempString, tempArgs);
						}

						printf("<form %s action=%s method=\"post\">\n", tempString, tempValue);
					}
					else {

						printf("<form %s action=%s method=\"post\">\n", extraTags, tempValue);
					}

					printf("<input type=\"submit\" name=%s value=%s>\n", actionText, actionText);
					printf("</form>\n");
				}
				else if (tag == 'l') {

					actionText[0] = ' ';
					actionText[strlen(actionText)-1] = '\0';

					if (strlen(actionText) == 0) {

						strcpy(actionText, "link");
					}

					if (strlen(extraTags) == 0 && randomText == false && printRandomText == false) {

						printf("<a href=%s>%s</a>\n",tempValue, actionText);
					}
					else if (randomText == true || printRandomText == true) {

						char tempString[500];
						clearArray(tempString, 500);

						strcpy(tempString, extraTags);
						strcat(tempString, " ");
						strcat(tempString, randomString);

						if (foundExtraTags == false) {

							strcat(tempString, " ");
							strcat(tempString, tempArgs);
						}

						printf("<a %s href=%s>%s</a>\n", tempString, tempValue, actionText);
					}
					else {

						printf("<a %s href=%s>%s</a>\n", extraTags, tempValue, actionText);
					}
				}
				else if (tag == 'p') {

					int z = 0;
					int counter = 0;
					char tempString[100];
					char tempString2[100];
					clearArray(tempString, 100);
					clearArray(tempString2, 100);

					/*Get the dimensions of the image*/
					for (z = 0; z < strlen(tempValue); z++) {

						if (tempValue[z] != 'x') {

							tempString[counter] = tempValue[z];
							counter = counter + 1;
						}
						else {

							strcpy(tempString2, tempString);
							clearArray(tempString, 100);
							counter = 0;
							tempString[counter] = '\"';
							counter = counter + 1;
						}
					}

					if (strlen(tempString2) == 0) {

						strcpy(tempString2, "100");
					}

					if (strlen(tempString) == 0) {

						strcpy(tempString, "100");
					}
					else {

						tempString[0] = ' ';
					}
					if (strlen(extraTags) == 0 && randomText == false && printRandomText == false) {

						printf("<img src=%s width=%s height=%s>\n", actionText, tempString2, tempString);
					}
					else if (randomText == true || printRandomText == true) {

						char tempString3[500];
						clearArray(tempString3, 500);

						strcpy(tempString3, extraTags);
						strcat(tempString3, " ");
						strcat(tempString3, randomString);

						if (foundExtraTags == false) {

							strcat(tempString3, " ");
							strcat(tempString3, tempArgs);
						}

						printf("<img %s src=%s width=%s height=%s>\n", tempString3, actionText, tempString2, tempString);
					}
					else {

						printf("<img %s src=%s width=%s height=%s>\n", extraTags, actionText, tempString2, tempString);
					}
				}
				else if (tag == 'r') {

					if (strlen(extraTags) == 0 && randomText == false && printRandomText == false) {

						printf("<form action=%s method=\"post\">\n", actionText);
					}

					else if (randomText == true || printRandomText == true) {

						char tempString3[500];
						clearArray(tempString3, 500);

						strcpy(tempString3, extraTags);
						strcat(tempString3, " ");
						strcat(tempString3, randomString);

						if (foundExtraTags == false) {

							strcat(tempString3, " ");
							strcat(tempString3, tempArgs);
						}

						printf("<form %s action=%s method=\"post\">\n", tempString3, actionText);
					}
					else {

						printf("<form %s action=%s method=\"post\">\n", extraTags, actionText);
					}

					int u = 0;

					for (u = 0; u < strlen(radioName); u++) {

						if (radioName[u] == '!' && radioName[u+1] == '#' && radioName[u+2] == '$' && radioName[u+3] == '@') {

							u = u + 3;
						}
						else {

							printf("%c", radioName[u]);
						}
					}

					char tempText2[500];
					clearArray(tempText2, 500);

					strcpy(tempText2, tempText);

					tempText2[0] = ' ';
					tempText2[strlen(tempText2)-1] = '\0';

					if (strlen(tempText2) != 0) {
						printf("<input type=\"radio\" name=%s value=%s> %s<br>\n", saveName, tempText, tempText2);
					}

					printf("</form>\n");
				}
				else if (tag == 'a') {

					int z = 0;
					char tempString[100];
					char tempString2[100];
					clearArray(tempString, 100);
					clearArray(tempString2, 100);

					/*Get dimensions of text area*/
					for (z = 0; z < strlen(tempText); z++) {

						if (tempText[z] != 'x') {

							tempString[counter] = tempText[z];
							counter = counter + 1;
						}
						else {

							strcpy(tempString2, tempString);
							clearArray(tempString, 100);
							counter = 0;
						}
					}

					tempValue[0] = ' ';
					tempValue[strlen(tempValue)-1] = '\0';

					printf("<form action=%s method=\"post\" id=%s>\n", actionText, saveName);
					printf("<input type=\"submit\">\n");
					printf("</form>\n");
					printf("<textarea rows=%s cols=%s name=%s form=%s>\n%s", tempString, tempString2, saveName, saveName, tempValue);
					printf("</textarea>\n");
				}
				else if (tag == 'u') {

					int s = 0;
					int numOfFields = 1;
					int counter = 0;
					char temp[500];
					char temp2[500];
					char temp3[500];

					radio1[0] = ' ';
					radio1[strlen(radio1)-1] = '\0';
					radio2[0] = ' ';
					radio2[strlen(radio2)-1] = '\0';

					printf("<form action=%s method=\"post\">\n", actionText);
					printf("<input type=\"radio\" name=%s value=\"%s\" checked>%s<br>\n", radioName, radio1, radio1);
					printf("<input type=\"radio\" name=%s value=\"%s\">%s<br>\n", radioName, radio2, radio2);

					/*Print out the HTML for each input field*/
					while (inputCounter != 0) {

						numOfFields = inputCounter;
						counter = 0;

						clearArray(temp, 500);
						clearArray(temp2, 500);
						clearArray(temp3, 500);

						/*Get label/text of the input field*/
						for (s = 0; s < strlen(tempText); s++) {

							if (numOfFields == 1) {

								temp[counter] = tempText[s];
								counter = counter + 1;
							}

							if (tempText[s] == '"' && tempText[s+1] == '"') {

								numOfFields = numOfFields - 1;
							}
						}

						counter = 0;
						numOfFields = inputCounter;

						/*Get name of the input field*/
						for (s = 0; s < strlen(saveName); s++) {

							if (numOfFields == 1) {

								temp2[counter] = saveName[s];
								counter = counter + 1;
							}

							if (saveName[s] == '"' && saveName[s+1] == '"') {

								numOfFields = numOfFields - 1;
							}
						}

						counter = 0;
						numOfFields = inputCounter;

						/*Get default value of the input field*/
						for (s = 0; s < strlen(tempValue); s++) {

							if (numOfFields == 1) {

								temp3[counter] = tempValue[s];
								counter = counter + 1;
							}

							if (tempValue[s] == '"' && tempValue[s+1] == '"') {

								numOfFields = numOfFields - 1;
							}
						}

						temp[0] = ' ';
						temp[strlen(temp)-1] = '\0';

						if (strlen(temp3) == 1) {

							strcat(temp3, "\"");
						}

						printf("%s <input type=\"text\" name=%s value=%s><br>\n", temp, temp2, temp3);

						inputCounter = inputCounter - 1;
					}
					
					printf("<input type=\"submit\">\n");
					printf("</form>\n");
				}

				/*Reset all variables and strings*/
				clearArray(tempText, 500);
				clearArray(tempArgs, 500);
				clearArray(actionText, 500);
				clearArray(tempValue, 500);
				clearArray(saveName, 500);
				clearArray(radioName, 500);
				clearArray(radio1, 500);
				clearArray(radio2, 500);
				clearArray(extraTags, 1000);
				clearArray(randomString, 500);
				tempSize = '3';
				tempTextCounter = 0;
				tempArgsCounter = 0;
				actionTextCounter = 0;
				tempValueCounter = 0;
				saveNameCounter = 0;
				counter = 0;
				inputCounter = 0;
				radioNameCounter = 0;
				radio1Counter = 0;
				radio2Counter = 0;
				extraTagsCounter = 0;
				parameterType = 0;
				textFound = false;
				sizeFound = false;
				linkFound = false;
				valueFound = false;
				radioNameFound = false;
				radio1Found = false;
				radio2Found = false;
				foundExtraTags = false;
				randomText = true;
				printRandomText = false;
				firstTime = true;
				tag = '-';
			}

			/*Header*/
			if (tag == 'h') {

				/*When a comma is reached, we know we have reached the end of the parameter value(s)*/
				if (input[i] == ',' && inQuotations == false) {

					textFound = false;
					sizeFound = false;
					foundExtraTags = false;

					if (randomText == true) {

                    	printRandomText = true;
                    	strcat(randomString, tempArgs);
                    	strcat(randomString, " ");
                    }

					randomText = true;

					clearArray(tempArgs, 500);
					tempArgsCounter = 0;

					continue;
				}

				if (textFound == true) {

					tempText[tempTextCounter] = input[i];
					tempTextCounter = tempTextCounter + 1;

					continue;
				}
				else if (sizeFound == true) {

					tempSize = input[i];
					continue;
				}
				else if (foundExtraTags == true) {

					extraTags[extraTagsCounter] = input[i];
					extraTagsCounter = extraTagsCounter + 1;

					continue;
				}

				if (input[i] == '=' && strcmp(tempArgs, "text") == 0) {

					textFound = true;
					sizeFound = false;
					foundExtraTags = false;
					randomText = false;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "size") == 0) {

					textFound = false;
					sizeFound = true;
					foundExtraTags = false;
					randomText = false;
				}
				else if (input[i] == '=') {
						
					strcat(extraTags, " ");
					strcat(extraTags, tempArgs);
					strcat(extraTags, "=");
					extraTagsCounter = extraTagsCounter + strlen(tempArgs) + 2;

					textFound = false;
					sizeFound = false;
					foundExtraTags = true;
					randomText = false;
				}
				else {

					tempArgs[tempArgsCounter] = input[i];
					tempArgsCounter = tempArgsCounter + 1;
				}
			}
			else if (tag == 'd') {

				tempArgs[tempArgsCounter] = input[i];
				tempArgsCounter = tempArgsCounter + 1;
			}
			/*Text*/
			else if (tag == 't') {

				/*When a comma is reached, we know we have reached the end of the parameter value(s)*/
				if (input[i] == ',' && inQuotations == false) {

					textFound = false;
					foundExtraTags = false;

					if (randomText == true) {

                    	printRandomText = true;
                    	strcat(randomString, tempArgs);
                    	strcat(randomString, " ");
                    }

                    randomText = true;
					clearArray(tempArgs, 500);
					tempArgsCounter = 0;

					continue;
				}

				if (textFound == true) {

					tempText[tempTextCounter] = input[i];
					tempTextCounter = tempTextCounter + 1;

					continue;
				}
				else if (foundExtraTags == true) {

					extraTags[extraTagsCounter] = input[i];
					extraTagsCounter = extraTagsCounter + 1;

					continue;
				}

				if (input[i] == '=' && (strcmp(tempArgs, "file") == 0 || strcmp(tempArgs, "text") == 0)) {

					if (strcmp(tempArgs, "text") == 0) {

						parameterType = 1;
					}
					else {

						parameterType = 2;
					}

					textFound = true;
					randomText = false;
					foundExtraTags = false;
				}
				else if (input[i] == '=') {

					strcat(extraTags, " ");
					strcat(extraTags, tempArgs);
					strcat(extraTags, "=");
					extraTagsCounter = extraTagsCounter + strlen(tempArgs) + 2;

					textFound = false;
					randomText = false;
					foundExtraTags = true;
				}
				else {

					tempArgs[tempArgsCounter] = input[i];
					tempArgsCounter = tempArgsCounter + 1;
				}
			}
			/*Eexecute program*/
			else if (tag == 'y' || tag == 'e') {

				tempArgs[tempArgsCounter] = input[i];
				tempArgsCounter = tempArgsCounter + 1;
			}
			/*Button*/
			else if (tag == 'b') {

				/*When a comma is reached, we know we have reached the end of the parameter value(s)*/
				if (input[i] == ',' && inQuotations == false) {

					textFound = false;
					linkFound = false;
					foundExtraTags = false;

                    if (randomText == true) {

                    	printRandomText = true;
                    	strcat(randomString, " ");
                    	strcat(randomString, tempArgs);
                    	strcat(randomString, " ");
                    }

					randomText = true;

					clearArray(tempArgs, 500);
					tempArgsCounter = 0;

					continue;
				}

				if (textFound == true) {

					actionText[actionTextCounter] = input[i];
					actionTextCounter = actionTextCounter + 1;

					continue;
				}
				else if (linkFound == true) {

					tempValue[tempValueCounter] = input[i];
					tempValueCounter = tempValueCounter + 1;

					continue;
				}
				else if (foundExtraTags == true) {

					extraTags[extraTagsCounter] = input[i];
					extraTagsCounter = extraTagsCounter + 1;

					continue;
				}

				if (input[i] == '=' && strcmp(tempArgs, "name") == 0) {

					textFound = true;
					linkFound = false;
					foundExtraTags = false;
					randomText = false;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "link") == 0) {

					textFound = false;
					linkFound = true;
					foundExtraTags = false;
					randomText = false;
				}
				else if (input[i] == '=') {

					strcat(extraTags, " ");
					strcat(extraTags, tempArgs);
					strcat(extraTags, "=");
					extraTagsCounter = extraTagsCounter + strlen(tempArgs) + 2;

					textFound = false;
					linkFound = false;
					foundExtraTags = true;
					randomText = false;
				}
				else {

					tempArgs[tempArgsCounter] = input[i];
					tempArgsCounter = tempArgsCounter + 1;
				}
			}
			/*Link*/
			else if (tag == 'l') {

				/*When a comma is reached, we know we have reached the end of the parameter value(s)*/
				if (input[i] == ',' && inQuotations == false) {

					textFound = false;
					linkFound = false;
					foundExtraTags = false;

					if (randomText == true) {

                    	printRandomText = true;
                    	strcat(randomString, " ");
                    	strcat(randomString, tempArgs);
                    	strcat(randomString, " ");
                    }

					randomText = true;

					clearArray(tempArgs, 500);
					tempArgsCounter = 0;

					continue;
				}

				if (textFound == true) {

					actionText[actionTextCounter] = input[i];
					actionTextCounter = actionTextCounter + 1;

					continue;
				}
				else if (linkFound == true) {

					tempValue[tempValueCounter] = input[i];
					tempValueCounter = tempValueCounter + 1;

					continue;
				}
				else if (foundExtraTags == true) {

					extraTags[extraTagsCounter] = input[i];
					extraTagsCounter = extraTagsCounter + 1;

					continue;
				}

				if (input[i] == '=' && strcmp(tempArgs, "text") == 0) {

					textFound = true;
					linkFound = false;
					foundExtraTags = false;
					randomText = false;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "link") == 0) {

					textFound = false;
					linkFound = true;
					foundExtraTags = false;
					randomText = false;
				}
				else if (input[i] == '=') {

					strcat(extraTags, " ");
					strcat(extraTags, tempArgs);
					strcat(extraTags, "=");
					extraTagsCounter = extraTagsCounter + strlen(tempArgs) + 2;

					textFound = false;
					linkFound = false;
					foundExtraTags = true;
					randomText = false;
				}
				else {

					tempArgs[tempArgsCounter] = input[i];
					tempArgsCounter = tempArgsCounter + 1;
				}
			}
			/*Picture*/
			else if (tag == 'p') {

				/*When a comma is reached, we know we have reached the end of the parameter value(s)*/
				if (input[i] == ',' && inQuotations == false) {

					textFound = false;
					linkFound = false;
					foundExtraTags = false;

					if (randomText == true) {

                    	printRandomText = true;
                    	strcat(randomString, tempArgs);
                    	strcat(randomString, " ");
                    }

					randomText = true;
					tempArgsCounter = 0;

					clearArray(tempArgs, 500);

					continue;
				}

				if (textFound == true) {

					actionText[actionTextCounter] = input[i];
					actionTextCounter = actionTextCounter + 1;

					continue;
				}
				else if (linkFound == true) {

					tempValue[tempValueCounter] = input[i];
					tempValueCounter = tempValueCounter + 1;

					continue;
				}
				else if (foundExtraTags == true) {

					extraTags[extraTagsCounter] = input[i];
					extraTagsCounter = extraTagsCounter + 1;

					continue;
				}

				if (input[i] == '=' && strcmp(tempArgs, "image") == 0) {

					textFound = true;
					linkFound = false;
					randomText = false;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "size") == 0) {

					textFound = false;
					linkFound = true;
					randomText = false;
				}
				else if (input[i] == '=') {

					strcat(extraTags, " ");
					strcat(extraTags, tempArgs);
					strcat(extraTags, "=");
					extraTagsCounter = extraTagsCounter + strlen(tempArgs) + 2;

					textFound = false;
					linkFound = false;
					randomText = false;
					foundExtraTags = true;
				}
				else {

					tempArgs[tempArgsCounter] = input[i];
					tempArgsCounter = tempArgsCounter + 1;
				}
			}
			/*Radio Button*/
			else if (tag == 'r') {

				/*When a comma is reached, we know we have reached the end of the parameter value(s)*/
				if (input[i] == ',' && inQuotations == false) {

					if (randomText == true) {

                    	printRandomText = true;
                    	strcat(randomString, tempArgs);
                    	strcat(randomString, " ");
                    }

					if (valueFound == true) {

						if (strcmp(tempValue, "") != 0) {

							strcpy(saveName, tempValue);
						}

						char tempText2[500];
						clearArray(tempText2, 500);

						strcpy(tempText2, tempText);

						tempText2[0] = ' ';
						tempText2[strlen(tempText2)-1] = '\0';

						if (firstTime == true) {

							firstTime = false;
							strcat(radioName, "<input type=\"radio\" name=");
							strcat(radioName, saveName);
							strcat(radioName, " value=");
							strcat(radioName, tempText);
							strcat(radioName, " checked>");
							strcat(radioName, tempText2);
							strcat(radioName, "<br>\n");
							strcat(radioName, "!#$@");
						}
						else {

							strcat(radioName, "<input type=\"radio\" name=");
							strcat(radioName, saveName);
							strcat(radioName, " value=");
							strcat(radioName, tempText);
							strcat(radioName, ">");
							strcat(radioName, tempText2);
							strcat(radioName, "<br>\n");
							strcat(radioName, "!#$@");
						}

						clearArray(tempText, 500);
						clearArray(tempText2, 500);
						tempTextCounter = 0;
					}

					textFound = false;
					linkFound = false;
					valueFound = false;
					foundExtraTags = false;
					randomText = true;

					clearArray(tempArgs, 500);
					tempArgsCounter = 0;

					continue;
				}

				if (textFound == true) {

					actionText[actionTextCounter] = input[i];
					actionTextCounter = actionTextCounter + 1;

					continue;
				}
				else if (linkFound == true) {

					tempValue[tempValueCounter] = input[i];
					tempValueCounter = tempValueCounter + 1;

					continue;
				}
				else if (valueFound == true) {

					tempText[tempTextCounter] = input[i];
					tempTextCounter = tempTextCounter + 1;

					continue;
				}
				else if (foundExtraTags == true) {

					extraTags[extraTagsCounter] = input[i];
					extraTagsCounter = extraTagsCounter + 1;

					continue;
				}

				if (input[i] == '=' && strcmp(tempArgs, "action") == 0) {

					textFound = true;
					linkFound = false;
					valueFound = false;
					randomText = false;
					foundExtraTags = false;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "name") == 0) {

					textFound = false;
					linkFound = true;
					valueFound = false;
					randomText = false;
					foundExtraTags = false;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "value") == 0) {

					textFound = false;
					linkFound = false;
					valueFound = true;
					randomText = false;
					foundExtraTags = false;
				}
				else if (input[i] == '=') {

					textFound = false;
					linkFound = false;
					valueFound = false;
					foundExtraTags = true;
					randomText = false;

					strcat(extraTags, " ");
					strcat(extraTags, tempArgs);
					strcat(extraTags, "=");
					extraTagsCounter = extraTagsCounter + strlen(tempArgs) + 2;
				}
				else {

					tempArgs[tempArgsCounter] = input[i];
					tempArgsCounter = tempArgsCounter + 1;
				}
			}
			/*Input fields*/
			else if (tag == 'i' || tag == 'a' || tag == 'u') {

				/*When a comma is reached, we know we have reached the end of the parameter value(s)*/
				if (input[i] == ',' && inQuotations == false) {

					if (randomText == true) {

                    	printRandomText = true;
                    	strcat(randomString, tempArgs);
                    	strcat(randomString, " ");
                    }

					clearArray(tempArgs, 500);
					tempArgsCounter = 0;

					linkFound = false;
					textFound = false;
					sizeFound = false;
					valueFound = false;
					radioNameFound = false;
					radio1Found = false;
					radio2Found = false;
					foundExtraTags = false;
					randomText = true;

					continue;
				}

				if (linkFound == true) {

					actionText[actionTextCounter] = input[i];
					actionTextCounter = actionTextCounter + 1;

					continue;
				}
				else if (valueFound == true) {

					tempValue[tempValueCounter] = input[i];
					tempValueCounter = tempValueCounter + 1;

					continue;
				}
				else if (sizeFound == true) {

					saveName[saveNameCounter] = input[i];
					saveNameCounter = saveNameCounter + 1;

					continue;
				}
				else if (textFound == true) {

					tempText[tempTextCounter] = input[i];
					tempTextCounter = tempTextCounter + 1;

					continue;
				}
				else if (radioNameFound == true) {

					radioName[radioNameCounter] = input[i];
					radioNameCounter = radioNameCounter + 1;

					continue;
				}
				else if (radio1Found == true) {

					radio1[radio1Counter] = input[i];
					radio1Counter = radio1Counter + 1;

					continue;
				}
				else if (radio2Found == true) {

					radio2[radio2Counter] = input[i];
					radio2Counter = radio2Counter + 1;

					continue;
				}
				else if (foundExtraTags == true) {

					extraTags[extraTagsCounter] = input[i];
					extraTagsCounter = extraTagsCounter + 1;

					continue;
				}

				if (input[i] == '=' && (strcmp(tempArgs, "text") == 0 || strcmp(tempArgs, "size") == 0)) {

					textFound = true;
					linkFound = false;
					valueFound = false;
					sizeFound = false;
					radioNameFound = false;
					radio1Found = false;
					radio2Found = false;
					foundExtraTags = false;
					randomText = false;
					inputCounter = inputCounter + 1;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "name") == 0) {

					textFound = false;
					linkFound = false;
					valueFound = false;
					sizeFound = true;
					radioNameFound = false;
					radio1Found = false;
					radio2Found = false;
					randomText = false;
					foundExtraTags = false;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "value") == 0) {

					textFound = false;
					linkFound = false;
					valueFound = true;
					sizeFound = false;
					radioNameFound = false;
					radio1Found = false;
					radio2Found = false;
					randomText = false;
					foundExtraTags = false;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "action") == 0) {

					textFound = false;
					linkFound = true;
					valueFound = false;
					sizeFound = false;
					radioNameFound = false;
					radio1Found = false;
					radio2Found = false;
					randomText = false;
					foundExtraTags = false;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "radioName") == 0) {

					textFound = false;
					linkFound = false;
					valueFound = false;
					sizeFound = false;
					radioNameFound = true;
					radio1Found = false;
					radio2Found = false;
					randomText = false;
					foundExtraTags = false;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "radio1") == 0) {

					textFound = false;
					linkFound = false;
					valueFound = false;
					sizeFound = false;
					radioNameFound = false;
					radio1Found = true;
					radio2Found = false;
					randomText = false;
					foundExtraTags = false;
				}
				else if (input[i] == '=' && strcmp(tempArgs, "radio2") == 0) {

					textFound = false;
					linkFound = false;
					valueFound = false;
					sizeFound = false;
					radioNameFound = false;
					radio1Found = false;
					radio2Found = true;
					randomText = false;
					foundExtraTags = false;
				}
				else if (input[i] == '=') {

					textFound = false;
					linkFound = false;
					valueFound = false;
					sizeFound = false;
					radioNameFound = false;
					radio1Found = false;
					radio2Found = false;
					randomText = false;
					foundExtraTags = true;

					strcat(extraTags, " ");
					strcat(extraTags, tempArgs);
					strcat(extraTags, "=");
					extraTagsCounter = extraTagsCounter + strlen(tempArgs) + 2;
				}
				else {

					tempArgs[tempArgsCounter] = input[i];
					tempArgsCounter = tempArgsCounter + 1;
				}
			}
		}

		clearArray(input, 500);
	}

	fclose(fp);

	if (foundFflag == false) {

		printf("</body>\n</html>\n");
	}
}

void clearArray(char string[], int length) {

	int j = 0;

	/*Set entire array to null terminators*/
	for (j = 0; j < length; j++) {
		
		string[j] = '\0';
	}
}
