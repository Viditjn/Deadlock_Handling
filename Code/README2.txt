README FOR MERGESORT USING SHARED MEMORY 	   

To run :-
	- make
	- ./2
Implementation
	- takes in 'n'(the number of elements) and the 'n' elements as input.

		- if number of elements less than 5 sorted using selection sort

		- then forking the left and right child and if it is not forked properly then an error message is printed out.

		c) Similarly it forks the right child also.

		d) Then the function waits for the child process to finish.

		e) At last it calls the merge function.

3)	a) The merge function takes in the array , lower index , middle inddex , higher index as input.

	b) it merges the elements as in the normal merge sort.

	c) returns the sorted array.


Report for comparison between normal merge sort and concurrent merge sort


The parallel merge sort has a runtime of O(n) for large test cases and thus is better than normal Merge Sort by O(logn) times. The gain in performance is due to the forking of process which runs the processes in parallel fashion.