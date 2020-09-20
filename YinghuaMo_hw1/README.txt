Yinghua Mo, Assignment 1

Problem 1 Heap
We are required to write an algorithm to have the array sorted in the required way.
a[j/2]>=a[j] for 1<=j/2<j<=N
For the algorithm, the most important part is (heapify) function. Firstly, I will determine whether the heap has a node that has only one child. If it does, I will deal with the node and then start from the value that located in front of it. Say, I start with the node (i), and compare it with the node (i – 1), then compare the bigger one with the parent. Then I swap the biggest one with the parent.
In the main function, I will have the (heapify) function run for (logn) time to ensure the sequence. There is one thing that should be noticed is that the result looks different from the given example but is conform with the requirement.

Problem 2 2D Array
In the function of (dmatrix_non_contiguous), firstly, I would like to malloc the address for every start of the rows, then I malloc the space for each row. Maybe the program may crush the computer if it asks for a very large piece of space.

Problem 3 2D Contiguous Array
In the function of (dmatrix_contiguous), firstly, I would like to malloc the whole space for the matrix, then I malloc the space for the address of start of each row. Maybe the program may crush the computer if it asks for a very large piece of space.

Problem 4 3D Contiguous Array
In the function of (d3darr_contiguous), fistly, I would like to malloc the whole space, then I malloc the address for each start of the row, and then I malloc the space of each row. Maybe the program may crush the computer if it asks for a very large piece of space.

Problem 5 3D Array Performance
The overall idea is to initialize the matrixes and compare the working time. The result that I got for calculating the 1000 * 1000 matrix is
The operation on the static matrix has run for 	 0.0093 seconds.
The operation on the non-contiguous matrix has run for 	 0.0056 seconds.
The operation on the contiguous matrix has run for 	 0.0052 seconds.
After several run, the speed of static matrix seems to lower than the results of other methods. It may be because of the parameter of the static work kernel requires the static matrix to be copied into the function.

Problem 6 File Read In
First, to define the char text[MAX_LINES][MAX_CHAR_PER_LINE], and then open the file and read and copy the line to the text table. The potential error might be the error when open the file and the error when read lines.

Problem 7 Alphabetizer
Firstly, the program will get every line of the text then compare the string of every line to swap the line. The potential errors maybe caused by some unexpected input type, which may cause the comparison failed.

Problem 8 Repeat Counter
The potential errors include the fault of finding a word for unexpected input and failure when apply for the space.

Problem 9 Vector.
The potential errors include the crush for insufficien space after requesting for more space(resize), even though all the inputs are valid. A more general solution is to implement the vector using template. 




