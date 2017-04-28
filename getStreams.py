#!/usr/bin/python

#Domenic Bianchi
#CIS 2750 Assignment 2
#February 19, 2017
#This program displays an interface to display posts from a signle or mutliple streams

import os
import sys

def getFileData(argUsername):

	fullStreamNames = []
	userReadPostIndex = []
	userStreams = []
	printErrorMessage = False				

	#Get all file names in the message directory
	for fileName in os.listdir("./messages"):
		#Search for all files containing user data
		if "StreamUsers" in fileName: 
			fileName = "./messages/" + fileName
			file = open(fileName, 'r')
			lines = file.readlines()

			#Search for a line that matches the username of the active user and save the index that tells the program how many posts in that strea have already been read
			for username in lines:
				username = username.strip('\n')
				fullName = username
				count = len(username)

				for character in reversed(username):
					count = count - 1
					if character == " ":
						username = username[:count]
						break

				if argUsername == username:
					fullStreamNames.append(fileName)
					fileName = fileName[11:-15]
					userStreams.append(fileName)
					userReadPostIndex.append(int(fullName[count+1:]))

			file.close()

	return {"fullStreamNames": fullStreamNames, "userReadPostIndex": userReadPostIndex, "userStreams": userStreams, "printErrorMessage": printErrorMessage}

def programLoop():

	userStreams = []
	userReadPostIndex = []
	fullStreamNames = []
	returnArray = []
	argUsername = ""
	counter = 0;

	argUsername = sys.argv[1]

	#Find all files containing username
	returnArray = getFileData(argUsername)
	fullStreamNames = returnArray["fullStreamNames"]
	userReadPostIndex = returnArray["userReadPostIndex"]
	userStreams = returnArray["userStreams"]

	if (len(userStreams) != 0):
		print("<form action=\"viewStream.php\" method=\"post\">")

	for fileName in userStreams:
		print("<input type=\"radio\" name=\"stream\" value=\"" + fileName + "\">" + fileName + "<br>")

	if len(userStreams) == 0:
		print("<p>No Streams</p>")
	else:
		print("<input type=\"radio\" name=\"stream\" value=\"all\" checked>all<br>")

	if (len(userStreams) != 0):
		print("<input type=\"submit\">")
		print("</form>")

	print("</body>\n</html>")

def main():

	programLoop()

if __name__ == "__main__":

	main()
