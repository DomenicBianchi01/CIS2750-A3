# CIS2750-A3

************
Compilation
************

To compile the programs required to run the web pages, execute the following command when inside the directory containing index.php:

make

This command will compile A1 (`converter.c` and `parseFile2.c`) and run the program to convert a file named `post.cc`. Please note that the file must be named post.cc, otherwise the makefile will not be able to the link the `correct .c` file to the other programs. Next, an object file for the stream library will be created. Then, the post and addauthor programs from A2 will be compiled where the library will then be linked. Finally, permissions for all python scripts included in the submission will be updated.

Also, a "messages" and a local "bin" directory will be created if they do not already exist in the current directory

The addauthor, post, converter, and a3 executables are all designed to be run by other programs automatically, however you can still run them in the command line by the following the instructions below, however, for all programs other than `./a3` and `./converter`, it is assumed that all command line arguments will be correct when running these programs:

`./converter filename`

where `filename` is the .cc file. Output is the converted .c file

`./addauthor flag username streamname`

where `username` is the user to be added to the stream, `streamname` is the name of the stream, and `flag` is -r to remove the user or empty to add the user (however, the "" must still be included if considered to be "empty")

`./post username streamname postcontent`

where `username` is the user to be added to the stream,`streamname` is the name of the stream, and `postcontent` is the message to be posted.

`./a3 filename`

where `filename` is the .wpml file (configuration file)

***********
Custom Tags
***********
a - adds a form linked to a text area (for making a post)
	
	.a(action="filename",name="...",value="...",size=<width>x<height>)
        
where `filename` is the file name of the action for the form, `name` is the ID to link the form and text are, `value` is the default text displaying in the text area, and "size" is the dimensions of the text area.

k - adds a break tag (`<br>`)
	
	.k()

y - delay running the executable until all other elements of tags in the config file have been sent to the browser. The executable in the .y tag is run as the very last action when generating HTML. Should NOT be used in replacement of the `.e` tag and should only be used to run programs written by me. It is assumed that `filename` includes the relative path of the file to be run.
	
	.y(exe="filename"
	
where `filename` is the file to be executed.

u - adds two radio buttons and two input fields within one form (for addauthor)
	
	.u(action="filename", radioName="...", radio1="...", radio2="...", text="...", name="...", value="...", text="...", name="...", value="...")
       
where "filename" is the file name of the action for the form, `radioName` is the name of the two radio buttons, `radio1/radio2` are the values of each radio button. The next six parameters follow the same format of the `.i` tag. Text, name, and value must be grouped together and listed in order.

*****************
Known Limitations
*****************
-Because the 4 programs mentioned above (and all python programs included in this submission) are not designed to be manually called by a user (rather the webpage and PHP creates a command line that will call the program) there is no error checking for proper number of command line arguments and format (with the exception of ./a3 and ./converter). It is assumed the correct number, format, and values of all command line arguments will always be provided. For example, if you attempt to run ./converter without a file name, the program will segmentation fault.

-This assignment is not compatible with Internet Explorer or Microsoft Edge. I have only tested the assignment successfully on Google Chrome and Safari. Other browsers may NOT be compatible.

-In order for the elements on the webpage to display correctly, make sure the web browser is wide enough where all the top row buttons can fit on one "line" (when on the viewing stream web page)

-A stream cannot be named "all"

-A stream cannot be named "-r"

-A stream name cannot be longer than 256 characters

-The post text cannot be longer than 5000 characters

-Given a configuration file, no single line can be longer than 500 characters.

-Given a configuration file, no tag can be spread over multiple lines. Each tag must be present on a single line. Multiple tags can be on the same line as long as they don't exceed the 500 character limit.

-Give a tag, no string can include quotation marks. For example this tag is valid:
	.t(text="Enter your username:")
However, the following is not allowed:
	.t(text="Enter "your" username:")
Even if you add escape characters, quotations inside quotations are still not allowed. Single quotes are OK.

-When running addauthor and inputing a list of stream names for "List of streams:", the number of characters you input cannot exceed 256 characters.

-Stream names cannot have a comma in them. Doing so will cause the add and remove user functions in addauthor to create undesired streams.

-If a file in the "messages" directory is deleted while the view program is being run and requires to use the file, the program will crash

-The "messages" directory/folder must exist for the web page programs to run correctly. The directory can be completely empty, it just has to exist. (the Makefile will create this directory if it does not already exist)

-When adding a post (pressed the "Post" button"), the post will be added to the current stream being viewed as the current user. If the viewing all streams, you cannot add a post (an error message will be displayed).

-When adding a post, the post cannot contain any special characters that would cause the message to be broken up into multiple command line arguments. For example, quotation marks (") cannot be used because that would cause the post to be broken into multiple arguments. The reason for this is because the web page programs pass the entire message as single command line argument to the programs that are used to add the post. If the special characters are escaped (\") manually by the user, this will work.

-It is assumed that any programs that are to be run using the .e tag in the config files will NOT have any output. (Executing something like "ls" is not allowed because it produces output)

-The .y tag, if used, must always be the last tag in a configuration file and only use files created by me.

-When initially viewing messages, the oldest unread message will be displayed first. If the 'Toggle Order' button is pressed, the messages will be sorted by names. If pressed again, the messages will be sorted by date again, but the display will begin at the very beginning/first page; it will not go to the oldest unread post. Only pressing the "Check For New" button will display the oldest unread post first.

-Do not use the refresh button on any web browser. Do not manually type in URL's for various webpages. Doing so will cause the program to possibly crash. Only use the buttons presented on the webpage to navigate.

-When viewing a stream, once you reach the end of the list of posts (pressing the Next Page button), an error message saying that there are no more unread posts will be displayed. When pressing the Previous Page, once you reach the "first" message in the list, you will not be able to navigate any further back. In other words, no error message is displayed for reaching the front of the list of posts; error messages are only displayed when you reach the end of the list of posts.

-Regarding the .h tag, sizes can be less than 1 and greater than 6. In the event the number is negative, the absolute value is taken.

-Regarding the .e tag, only one "exe" parameter can be included. Multiple "exe" parameters will result in no executables being run. Also, the tag cannot include any "garbage" tags and/or text. In other words, the .e flag must follow the specification exactly with no extra text/characters.

-For text tag (.t), if the file name provided in the "file" parameter does not exist, the default text will be displayed

-For the radio button tag (.r). When "garbage" tags are included, an extra garbage tag/text of "value" will be created. This only occurs when there is invalid text (excluding actual parameter garbage values). For example:
	.r(action="t1.php",name="m1",align="left",value="b1",value="b2") <-- this is OK
	.r(dd,action="t1.php",name="m1",align="left",value="b1",value="b2") <-- will produce an extra tag/text containing the word "value"
	.r(action="t1.php",name="m1",align="left",value="b1",dd,value="b2") <-- will produce an extra tag/text containing the word "value"

-When viewing a single stream, if the user is removed from the stream, an error message saying the user no longer has permission to view the current stream. When viewing all streams, when a user is removed from a stream, no error messages are given. Instead, the web page will just say there are no unread messages (and there will be no read posts either to display)

-When the "Mark All" button is pressed, it will mark ALL posts in the stream even if they are not visible to the current user. For example, if there are two posts in the stream and the user adds another post, the third post will not be visible until the user click "Check For New"; however if they click "Mark All", all 3 posts will be marked as read.

-When viewing all streams, if you are viewing the streams as user A, then remove user A, and then add user A back, as you navigate through the posts you will notice that it seems like the posts are out of order. I use two array, one for unread posts and one for reads posts. Every time "Next Page" or "Previous Page" is pressed these two arrays are updated to reflect appropriate changes. Therefore, as you press "Next" everything will seem ok, but as you press "Previous", the posts will have changed order. Say there were two streams each with two posts. After you removed a user and added a user back to one of the streams you would have two unread and two read posts. When I press "Next", the unread array is actually updated to now contain only one 1 unread. Because the two arrays are sorted by date (or name) independently of each other when you press "Previous" it will seem as if the posts have change order, but really its just because the unread post have moved to the read posts array causing the sorting to change.

*************************
Extra Program Information
*************************
***The purpose of this section is to address miscellaneous topics***

-For easy identification and to help with marking, when viewing messages I have added a line of text that tells you how messages are currently sorted (by name or date).

-Whenever the "Check For New" button is pressed, the messages will be sorted by date. Even if the messages were toggled by name, the toggle mode will change when the Check For New button is pressed. Also, after making a new post, the Check For New button will need to be pressed to view it.

-Adding an author when viewing posts will not cause the active user to change. If you are the "Domenic" user, then add the user "Bianchi", after adding "Bianchi" you will still view posts as the "Domenic" user.

-Error messages are displayed on the screen when:
	
  -The user no longer has permission to view the stream being viewed (only applies when viewing a single stream)
  
  -An attempt is made to add a user to a stream that they already have permission to view

-When creating a new post, I made the design choice to only allow posts to be created when NOT viewing all streams. I decided to do this because logically, the user is essentially logging in to view messages in a specific stream. Therefore, they should not be allowed to create messages as another user or post to another stream without actually viewing the stream. This follows the typical format of an actual online forum.


