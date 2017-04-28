#!/usr/bin/python

import os
import sys
import operator

#An object of this class represents the byte locations and user of a single post
class Post:

	stream = ""
	sender = ""
	text = ""
	date = ""
	intDate = 0
	startByte = 0
	endByte = 0

	def __init__(self, stream, sender, startByte, endByte, intDate, text, date):
		
		self.stream = stream
		self.sender = sender
		self.text = text
		self.date = date
		self.startByte = startByte
		self.endByte = endByte
		self.intDate = intDate

	def __str__(self):

		if (self.stream[len(self.stream)-6:] == "Stream"):
			tempStreamName = self.stream[:-6]
		else:
			tempStreamName = self.stream

		return tempStreamName + "<br>Sender: " + self.sender + "<br>" + self.date + "<br>" + str(self.text)

#This function takes in a date in the format of a string and converts that string into a number that represents the date.
#For example, Feb. 14, 2017 10:23 will be converted to 021420171023
def parseDate(date):
	
	newDate = ""

	date = date.strip("\n")
	date = date.split(" ")

	#Conver the month name into its equivalent integer value
	if (date[1] == "Jan."):
		newDate = "01"
	elif (date[1] == "Feb."):
		newDate = "02"
	elif (date[1] == "Mar."):
		newDate = "03"
	elif (date[1] == "Apr."):
		newDate = "04"
	elif (date[1] == "May."):
		newDate = "05"
	elif (date[1] == "Jun."):
		newDate = "06"
	elif (date[1] == "Jul."):
		newDate = "07"
	elif (date[1] == "Aug."):
		newDate = "08"
	elif (date[1] == "Sep."):
		newDate = "09"
	elif (date[1] == "Oct."):
		newDate = "10"
	elif (date[1] == "Nov."):
		newDate = "11"
	elif (date[1] == "Dec."):
		newDate = "12"

	#date[2] contains the day of the month. Remove the comma.
	#date[3] contains the year
	#date[4] contains the time (24-hour clock). Remove the : from the time
	newDate = date[3] + newDate + date[2].strip(",") + date[4].replace(":", "")
	#newDate = newDate + date[2].strip(",") + date[3] + date[4].replace(":", "")

	return newDate

def loadAllStreams(allFileNames, username):

	unreadArray = []
	readArray = []
	dataArray = []
	userArray = []
	streamArray = []
	newByteIndexs = []
	dataInfo = []
	unreadArray = []
	postOffset = []
	parsedDate = ""
	textToAdd = ""
	lineCount = 0
	counter = -1
	startByte = 0
	endByte = 0
	i = 0

	for name in allFileNames:
		dataArray.append("./messages/" + name + "StreamData.txt")
		userArray.append("./messages/" + name + "StreamUsers.txt")
		streamArray.append("./messages/" + name + "Stream.txt")

	#Get byte data for each stream
	for dataFile in dataArray:
		file = open(dataFile, 'r')

		lines = file.readlines()

		newByteIndexs.append(0)
		
		for byteNum in lines:
			byteNum = byteNum.strip('\n')
			newByteIndexs.append(byteNum)

		dataInfo.append(newByteIndexs)
		newByteIndexs = []
		file.close()

	#Get data that tells the program what the last post read was in each stream
	for userFile in userArray:
		#Open the user file
		file = open(userFile, 'r')

		lines = file.readlines()

		for user in lines:
			user = user.strip('\n')
			#Get the username
			name = user.rsplit(' ', 1)[0]
			#The number that "count" holds represents the last post read. For example, "7" would mean the user has read the first 7 posts
			count = user.rsplit(' ', 1)[1] 

			#If the name from the text file matches the active user in the program, then save the data to an array
			if (name == username):

				postOffset.append(count)

		file.close()

	#Get posts and order them by date
	for streamFile in streamArray:
		file = open(streamFile, 'r')
		counter = counter + 1

		#Get all unread posts
		for index in range(int(postOffset[i]), len(dataInfo[counter])-1):

			startByte = int(dataInfo[counter][index])
			endByte = int(dataInfo[counter][index+1])

			#Get post text from stream file
			file.seek(int(dataInfo[counter][index]))
			lines = file.read((int(dataInfo[counter][index+1])) - int(dataInfo[counter][index]))

			#Format the strings needed to create a post object
			streamNameToAdd = "Stream: " + streamFile[11:-4]
			senderToAdd = lines.splitlines()[0]
			dateToAdd = lines.splitlines()[1]

			for textLine in lines.splitlines()[2:]:
				textToAdd = textToAdd + textLine + "\n"

			intDate = parseDate(dateToAdd)

			post = Post(streamNameToAdd, senderToAdd[8:], startByte, endByte, intDate, textToAdd, dateToAdd);
			unreadArray.append(post)

			textToAdd = ""
			lineCount = 0

		#Get all read posts
		if (int(postOffset[i]) != 0):

			for index in range(0, int(postOffset[i])):

				startByte = int(dataInfo[counter][index])
				endByte = int(dataInfo[counter][index+1])

				#Get post text from stream file
				file.seek(int(dataInfo[counter][index]))
				lines = file.read((int(dataInfo[counter][index+1])) - int(dataInfo[counter][index]))

				#Format the strings needed to create a post object
				streamNameToAdd = "Stream: " + streamFile[11:-4]
				senderToAdd = lines.splitlines()[0]
				dateToAdd = lines.splitlines()[1]

				for textLine in lines.splitlines()[2:]:
					textToAdd = textToAdd + textLine + "\n"
					lineCount = lineCount + 1

				intDate = parseDate(dateToAdd)

				#Create post object
				post = Post(streamNameToAdd, senderToAdd[8:], startByte, endByte, intDate, "", dateToAdd)
				readArray.append(post)

				textToAdd = ""
				lineCount = 0

		file.close()

		i = i + 1

	unreadArray = sorted(unreadArray, key=operator.attrgetter('intDate'))
	readArray = sorted(readArray, key=operator.attrgetter('intDate'))

	return {"postCount": len(unreadArray) + len(readArray), "startingIndex": len(readArray)}

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

def getNum(username, streamName):

	lastPostRead = 0
	count = 0
	offsetCounter = 0
	fileCount = 0
	postCount = 0
	streamNames = []

	if (streamName == "all"):

		streamNames = getStreamNames(username)
		returnArray = loadAllStreams(streamNames, username)

		print returnArray["startingIndex"]
		print returnArray["postCount"]

	else:

		for fileName in os.listdir("./messages"):
			#Search for all files containing user data
			if "StreamUsers" in fileName: 
				fileName = "./messages/" + fileName
				file = open(fileName, 'r')
				lines = file.readlines()

				#Search for a line that matches the username of the active user and save the index that tells the program how many posts in that strea have already been read
				for user in lines:
					user = user.strip('\n')
					fullName = user
					count = len(user)

					for character in reversed(user):
						count = count - 1
						if character == " ":
							user = user[:count]
							break

					if username == user and fileName[11:-15] == streamName:
						lastPostRead = int(fullName[count+1:])

				file.close()

		for fileName in os.listdir("./messages"):
			#Search for all files containing user data
			if "StreamData" in fileName: 
				fileName = "./messages/" + fileName
				if (fileName[11:-14] == streamName):
					file = open(fileName, 'r')
					postCount = len(file.readlines())
					file.close()
					break

		print lastPostRead
		print postCount

def main():

	getNum(sys.argv[2], sys.argv[1]);

if __name__ == "__main__":

	main()