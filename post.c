/*Domenic Bianchi*/ 
/*CIS 2750 Assignment 2*/ 
/*February 19 , 2017*/ 
/*This program gets user input to create a new post*/ 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <stdbool.h> 
#include <time.h> 
#include "stream.h" 
#include "addauthor.h" 
struct postEntry {
    void(*postEntrysubmitPost)(); 
    void(*postEntrygetTimeAndDatec)(char*); 
    userPost*(*postEntryformatEntrycccc)(char*,char*,char*,char*); 
}; 
userPost * postEntryformatEntrycccc  ( char name[] , char stream[] , char text[] , char dateAndTime[] ) {
    userPost * current ; 
    /*Malloc memory for the Post node*/ 
    current = malloc ( sizeof ( userPost ) ) ; 
    /*If the malloc fails , do not create the node*/ 
    if ( current == NULL ) {
        free ( current ) ; 
        return NULL ; 
    }
    current->username = name ; 
    current->streamname = stream ; 
    current->date = dateAndTime ; 
    current->text = text ; 
    return current ; 
}
void postEntrygetTimeAndDatec ( char string[] ) {
    time_t currentTime ; 
    struct tm * timeStruct ; 
    /*Get the current data and time from the machine this program is being run on*/ 
    currentTime = time ( NULL ) ; 
    timeStruct = localtime ( &currentTime ) ; 
    /*Format the time string in order to be parsed correctly in the python script*/ 
    strftime ( string , 256 , "%b. %d, %Y %H:%M:%S\n" , timeStruct ) ; 
}
void postEntrysubmitPost ( userPost * post ) {
    /*updateStream is a library function*/ 
    updateStream ( post ) ; 
    free ( post ) ; 
}
void constructorpostEntry (struct postEntry * ptr) {
    ptr->postEntrysubmitPost = postEntrysubmitPost;
    ptr->postEntrygetTimeAndDatec = postEntrygetTimeAndDatec;
    ptr->postEntryformatEntrycccc = postEntryformatEntrycccc;
}
 int main ( int argc , char * argv[] ) {
    struct postEntry post ; 
    constructorpostEntry(&post); 
    userPost * fullPost ; 
    char dateAndTime[256] ; 
    clearArray ( dateAndTime , 256 ) ; 
    /*Get the time and date*/ 
    post.postEntrygetTimeAndDatec ( dateAndTime ) ; 
    /*Format all the inputed information into a struct that can be sent to the stream library*/ 
    fullPost = post.postEntryformatEntrycccc ( argv[1] , argv[2] , argv[3] , dateAndTime ) ; 
    /*Send struct to stream library*/ 
    post.postEntrysubmitPost ( fullPost ) ; 
    return 0 ; 
}
