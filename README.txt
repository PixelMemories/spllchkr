Spelling checker - CS214 P2

[ MEMBERS ]
Colin Cunningham - cdc166
Richard Li - rl902

[ MAJOR DESIGN NOTES ]
Our Program incorporates 3 main functions to complete the task of spell checking txt files using a given dictionary. 
First we have DYprepare(), which using the first argument given to the program will loop through the entirety of the given dictionary file and create a very large 2d array full of words.
Then we have prepareCompare() which will loop through each of the the given txt files after the dictionary argument and create a linked list of all the words in the txt files. It ignores all surrounding punctuation. 
prepareCompare will then call on our 3rd important function checkList() which, utilizing some custom strcmp and binary/seq searchs, scans the dictionary for each word in each linked list for each txt file, and prints out (in the given format) any spelling errors it detects. 

There are also some helper functions that are sprinkled in along the way to make the process smoother. 

[ TEST PLAN ]
Out test plan consists of using some general txt files any proud american would have handy (ie the constitution.txt and a road.txt) and an edgcase txt file (Edges.txt). 
The general txt files test how our program does handling short, and long text files, as well as some odd english shenanigans including formatting and misspelled words. 
Our edge cases txt file lists some really weird edgecases that we might run into for some god awful reason just to make sure we follow the write ups logic to a T.
Finally our program has a runtime reader built into its main function so we can see how long the program takes to run each time we feed it inputs. 
