# multithreaded prime numbers discriminant program !!

- single thread program execution time
![image](https://user-images.githubusercontent.com/53115254/93728488-17083b80-fbfb-11ea-83ab-1b0bf4bf0fd4.png)

- multi thread program execution time
![image](https://user-images.githubusercontent.com/53115254/93728502-24252a80-fbfb-11ea-81e1-9baea04315b6.png)

The single thread program took approximately 4.6 seconds.
The multithreaded program took approximately 2.2 seconds to perform.

The performance speed has been improved by about 2.1 times.

Because the number of processors in the system is eight, if the parallelized portion is 100%,
The speed of performance should be eight times faster.

But given the 2.1-fold increase in speed, that's all.
It can be thought that the parallelized part is about 60%.

Amdahl's law: 1/ (0.4 + 0.6/8) = 2.1 times.