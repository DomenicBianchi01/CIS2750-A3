#!/usr/bin/python

import os
import sys

def getStreamNames(argUsername):

	userStreams = []

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
					fileName = fileName[11:-15]
					userStreams.append(fileName)

			file.close()

	return userStreams

def markPosts(username, streamName, numOfPosts):

	#Mark all posts read
	if (streamName == "all"):

		streamList = getStreamNames(username)

		for fileName in streamList:
			userFileName = "./messages/" + fileName + "StreamUsers.txt"
			dataFileName = "./messages/" + fileName + "StreamData.txt"
			oldUserFile = open(userFileName, 'r')
			dataFile = open(dataFileName, 'r')
			newUserFile = open("./messages/temp.txt", 'w')

			lines = oldUserFile.readlines()
			dataLineCount = dataFile.readlines()

			for user in lines:
				user = user.strip('\n')
				count = len(user)

				for character in reversed(user):
					count = count - 1
					if character == " ":
						usernameShort = user[:count]
						#If the usernames match, upate the read count to the highest possible value (total number of posts in the stream)
						if (username == usernameShort):
							newUserFile.write(usernameShort + " " + str(len(dataLineCount)) + "\n")
						else:
							newUserFile.write(user + "\n")
						break

			oldUserFile.close()
			newUserFile.close()
			dataFile.close()

			os.rename("./messages/temp.txt", userFileName)

	#Mark all posts in the specified stream read
	else:

		userFileName = "./messages/" + streamName + "StreamUsers.txt"
		dataFileName = "./messages/" + streamName + "StreamData.txt"

		userFile = open(userFileName, 'r')
		dataFile = open(dataFileName, 'r')
		newFile = open("./messages/temp.txt", 'w')

		lines = userFile.readlines()
		dataLineCount = dataFile.readlines()

		#Search for a line that matches the username of the active user and save the index that tells the program how many posts in that strea have already been read
		for user in lines:
			user = user.strip('\n')
			count = len(user)

			#Looping through each line backwards, when the first space is found, everything to the right is the post read count; everything to the left is the username
			for character in reversed(user):
				count = count - 1
				if character == " ":
					usernameShort = user[:count]
					if (username == usernameShort):
						newFile.write(user[:count] + " " + str(len(dataLineCount)) + "\n")
					else:
						newFile.write(user + "\n")

		userFile.close()
		newFile.close()

		os.rename("./messages/temp.txt", userFileName);

def main():

	markPosts(sys.argv[2], sys.argv[1], sys.argv[3]);

if __name__ == "__main__":

	main()