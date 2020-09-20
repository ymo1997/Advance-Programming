# MPCS 51100 PSet 2
## Starting Framework

Include any notes on your implementation 
as well as your results for problem 6
in this file.

- Student Name: Yinghua Mo
- Student Email: ymo1997@uchicago.edu
- Notes:
p1:
We are asked to implement the validation of BST recursively with the range.
My idea is to validate from the top to the bottom, to see whether the val of the current node is within the range of min and max. Then I would like to check the left node and the right node.

p2:
BST has the characteristic that if you traverse the tree in orderly, the elements will be shown in order. So I would like to traverse the tree iteratively with in-order traversal, to see whether all the elements are ascending. If it is, then return true. If it is not, return false.

p3:
Travel two trees at the same time and to compare each node of them. If something wrong, return false at once. Else continue to traverse.

p4 & p5 & p6:
I implemented the naive BST base on the basic characteristic of BST;

I implemented the Balanced BST base on the RB tree with the slides.


p7:
The basic idea is to do the binary search on earh row of the matrix.

p8:
Because I am allowed to use more space, and I notice that every float is two decimal long. So for every row of the matrix, I would like to have a array of 101 elements. And every element's value is the postion of the nearest element in the original row. 
Within this kind of method, the time complexity of the initialization would be O(M * 101), notice that n would be smaller than 101. But within this way, the time complexity for getting the nearest position is O(1) for we have stored the answer into the array. But the additional space complexity will reach to O(M * 101);


### Problem 6 Results:

Naive BST Dictionary
- Time to import (sec):2.396404
- Avg. time per find (us):0.000030

Self Balancing BST Dictionary
- Time to import (sec):0.238421
- Avg. time per find (us):0.000010

### Discussion:

After running the result:
Naive time for import : 2.396404 seconds.
Naive time for finding : 0.000030 microseconds per word
bbalance time for import : 0.238421 seconds.
balance time for finding : 0.000010 microseconds per word

As we can see, the balanced BST has better result both in generating tree and finding nodes. But from the perspective of average finding, the difference is small for the scale is small. But the difference between two methods can be seen obviously.
