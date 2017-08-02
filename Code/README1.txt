README FOR Q1 (PRODUCER - CONSUMER PROBLEM)

To run 
	: gcc new1.c -lpthread
	: ./a.out

Implementation:-
	- use of n threads for readers and 1 thread for writer where n is the number of consumers or readers.
	- use of flag 2-D array to determine whether a particular position of buffer string is read by a particular reader 
	- also another flag array which is used to determine that before overwriting/writing a particular position is read by all other readers
	- if a writer wants to overwrite a position it has to wait untill all other readers read it and till then it just wait i.e. doesn't write on other position, implemented using wait loop 
	- random selection of the position to be read or write and value to be filled 

