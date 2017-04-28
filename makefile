all: programs

programs: messages bin
	gcc -g -Wall -ansi main2.c parseFile2.c converter.c -o converter
	./converter post.cc
	gcc -c stream.c -o stream.o
	ar cr libstream.a stream.o
	gcc -g -Wall -ansi post.c parseFile2.c converter.c -o post -lstream -L.
	gcc -g -Wall -ansi addauthor.c -o addauthor -lstream -L.
	gcc -g -Wall -ansi main.c parseFile.c -o a3
	chmod +x getPost.py
	chmod +x getReadPostNum.py
	chmod +x getStreams.py
	chmod +x markAllPosts.py

messages:
	mkdir messages

bin:
	mkdir bin