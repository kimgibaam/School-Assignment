### There are beads with overlapping numbers from 1 to 10. When these n beads are listed in a row, implement the algorithm C to find the longest necklace that meets the following conditions
 
- Input : The order in which n beads with numbers are listed
- Condition 1 : The number of the first and last beads on the necklace must be the same
- Condition 2 : When the necklace is spread out in a circle, the numbers facing each other must be the same

![image](https://user-images.githubusercontent.com/53115254/93735065-98b99280-fc16-11ea-98da-49130a6deeb9.png)

- Result

![image](https://user-images.githubusercontent.com/53115254/93735102-c4d51380-fc16-11ea-99e0-54687cba6cd0.png)


Calculate the Big O for time complexity according to n

We only look at the most important parts because we need Big O.
The repeating statement within the Necklace function contains the repeating statement due to the function call.
It has the greatest weight.

In any case, the outer iteration statement is executed in number n.
In the worst case scenario, if all the numbers are the same, the repeat statement is 20 times when n = 8.
It has a similar number of operations to nlogn because it computes 64 times at 16.

So Bigo is O(n^2*logn)