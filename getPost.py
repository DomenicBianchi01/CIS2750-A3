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

def loadAllStreams(allFileNames, username, timeStamp):

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

			if (int(intDate) < timeStamp):
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

				if (int(intDate) < timeStamp):
					post = Post(streamNameToAdd, senderToAdd[8:], startByte, endByte, intDate, "", dateToAdd)
					readArray.append(post)

				textToAdd = ""
				lineCount = 0

		file.close()

		i = i + 1

	unreadArray = sorted(unreadArray, key=operator.attrgetter('intDate'))
	readArray = sorted(readArray, key=operator.attrgetter('intDate'))

	return {"unreadArray": unreadArray, "readArray": readArray}

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

def getPost(username, streamName, lastPostRead, toggleMode, maxCount, timeStamp):

	dataFileName = ""
	postFileName = ""
	userFileName = ""
	textToAdd = ""
	startIndex = 0
	endIndex = 0
	streamName2 = ""
	lines = []
	streamNames = []
	unreadPostArray = []
	readPostArray = []
	postArray = []

	if (streamName == "all"):

		mergedArray = []

		streamNames = getStreamNames(username)

		returnArray = loadAllStreams(streamNames, username, timeStamp)
		unreadPostArray = returnArray["unreadArray"]
		readPostArray = returnArray["readArray"]

		mergedArray = readPostArray + unreadPostArray

		if (toggleMode == 2):
			mergedArray = sorted(mergedArray, key=operator.attrgetter('sender'))

		try:
			startIndex = int(mergedArray[lastPostRead].startByte)
			endIndex = int(mergedArray[lastPostRead].endByte)
			streamName2 = mergedArray[lastPostRead].stream
		except IndexError:
			print("No unread messages")
			return

		postFile = open("./messages/" + streamName2[8:] + ".txt", 'r')

		postFile.seek(startIndex)
		post = postFile.read(endIndex - startIndex)

		textToAdd = "Stream: " + streamName2[8:-6] + "<br>"
		for textLine in post.splitlines():
			textToAdd = textToAdd + textLine + "<br>"

		print(textToAdd)

		postFile.close()

		if (toggleMode == 1 and len(unreadPostArray) != 0):

			readCount = 0

			for singlePost in readPostArray:
				if (singlePost.stream[8:-6] == streamName2[8:-6]):
					readCount = readCount + 1

			userFile = open("./messages/" + streamName2[8:] + "Users.txt", 'r')
			dataFile = open("./messages/" + streamName2[8:] + "Data.txt", 'r')
			newFile = open("./messages/temp.txt", 'w')

			lines = userFile.readlines()

			dataLineCount = len(dataFile.readlines())
			dataFile.close()

			#Loop through all line of the data file looking for a matching username
			for user in lines:
				user = user.strip('\n')
				count = len(user)

				#Looping through each line backwards, when the first space is found, everything to the right is the post read count; everything to the left is the username
				for character in reversed(user):
					count = count - 1
					if character == " ":
						usernameShort = user[:count]
						if (username == usernameShort and int(user[count:]) < lastPostRead+1 and int(user[count:])+1 <= dataLineCount):
							newFile.write(user[:count] + " " + str(int(user[count:])+1) + "\n")
						else:
							newFile.write(user + "\n")

			userFile.close()
			newFile.close()
			os.rename("./messages/temp.txt", "./messages/" + streamName2[8:] + "Users.txt");

	else:

		tempUserArray = []

		dataFileName = "./messages/" + streamName + "StreamData.txt"
		postFileName = "./messages/" + streamName + "Stream.txt"
		userFileName = "./messages/" + streamName + "StreamUsers.txt"

		userFile = open(userFileName, 'r')

		userLines = userFile.readlines()

		for user in userLines:
			user = user.strip('\n')
			count = len(user)

			for character in reversed(user):
				count = count - 1
				if character == " ":
					user = user[:count]
					break

			if username == user:
				tempUserArray.append(0)

		if (len(tempUserArray) == 0):
			print("User no longer has permission to view this stream. Select the \"Change Author\" or \"Change Stream\" buttons to continue")
			return

		userFile.close()

		dataFile = open(dataFileName, 'r')
		postFile = open(postFileName, 'r')

		lines = dataFile.readlines()

		if (lastPostRead >= maxCount):
			print("No unread messages")
			return

		lines.insert(0,0)

		if (toggleMode == 1):

			try:
				startIndex = int(lines[lastPostRead])
				endIndex = int(lines[lastPostRead + 1])
			except IndexError:
				print("No unread messages")
				return
		else:

			for i in range(0, int(maxCount)):
				try:
					startIndex = int(lines[i])
					endIndex = int(lines[i+1])

					postFile.seek(startIndex)
					post = postFile.read(endIndex - startIndex)

					postUsername = post.splitlines()[0][8:]
					post = Post("", postUsername, startIndex, endIndex, 0, "", "")
					postArray.append(post)
				except IndexError:
					pass

			postArray = sorted(postArray, key=operator.attrgetter('sender'))

			try:
				startIndex = int(postArray[lastPostRead].startByte)
				endIndex = int(postArray[lastPostRead].endByte)
			except IndexError:
				print("No unread messages")
				return

		postFile.seek(startIndex)
		post = postFile.read(endIndex - startIndex)

		textToAdd = "Stream: " + streamName + "<br>"

		for textLine in post.splitlines():
			textToAdd = textToAdd + textLine + "<br>"

		textToAdd = "<p>" + textToAdd + "</p>"

		print(textToAdd)

		#Only modify how many posts have been read by the user if post are being order by dates
		if (toggleMode == 1):

			userFile = open(userFileName, 'r')
			newFile = open("./messages/temp.txt", 'w')

			lines = userFile.readlines()

			#Loop through all line of the data file looking for a matching username
			for user in lines:
				user = user.strip('\n')
				count = len(user)

				#Looping through each line backwards, when the first space is found, everything to the right is the post read count; everything to the left is the username
				for character in reversed(user):
					count = count - 1
					if character == " ":
						usernameShort = user[:count]
						if (username == usernameShort and int(user[count:]) < lastPostRead+1):
							newFile.write(user[:count] + " " + str(int(user[count:])+1) + "\n")
						else:
							newFile.write(user + "\n")

			userFile.close()
			newFile.close()
			os.rename("./messages/temp.txt", userFileName);

		dataFile.close()
		postFile.close()

def programLoop():

	username = ""
	streamName = ""
	lastPostRead = 0

	username = sys.argv[1]
	streamName = sys.argv[2]
	postNum = int(sys.argv[3])
	toggleMode = int(sys.argv[4])
	maxCount = int(sys.argv[5])
	timeStamp = int(sys.argv[6])
	
	getPost(username, streamName, postNum, toggleMode, maxCount, timeStamp)

def main():

	programLoop()

if __name__ == "__main__":

	main()