Name:	Nathan Kong
UID:	204 401 093
TA:	Aishwarya Kamal
File:	readme.txt

1. Unpack the srt files
   > tar xf srt.tgz

2. Change the Makefile to include the thread library. On the line with LDLIBS,
   add -lpthread to allow the library in main.c.

3. Include the library of pthread.h in main.c. Look for any redundant code
   in the body of the program to implement threading.

4. I noticed that there is a comment made about how a portion of the code
   operates on every pixel of the image.  Therefore we can thread these
   processes by having n threads operate on each row.

   Some problems that I ran into involved segmentation faults within my program
   but were resolved by resorting to using global variables. I would be able to
   access these variables in my threadedFunction and main. The thread number is
   passed to the function therefore each thread would be responsible for a set
   of numbers, for example:
   nthreads = 2
   thread1 = {1, 3, 5, 7, 9 ...}
   thread2 = {2, 4, 6, 8, 10 ...}

5. Execute the program using:
   >  make clean check
   
   1-test.ppm:
   real    0m39.684s
   user    0m39.677s
   sys     0m0.001s

   2-test.ppm:
   real    0m19.927s
   user    0m39.582s
   sys     0m0.002s

   4-test.ppm:
   real    0m10.404s
   user    0m41.367s
   sys     0m0.003s

   8-test.ppm:
   real    0m6.363s
   user    0m45.807s
   sys     0m0.004s

6. We notice that the real time decreases as the number of threads increase.
   However there is a limit as to how efficient we can make srt by adding
   threads, there is less of a significance in time performance going from
   4 threads to 8 threads.

7. The images can be checked using MobaXterm
   > gimp 1-test.ppm

8. I had ran into some problems concerning an odd message which would appear
   at the end of running 'make clean check':
   make: *** [check] Error 1
   I was not able to resolve this difference by changing Makefile.

9. Compiling my program triggered warnings for the delcaration of memset inside
   my program. I got around this by using a loop instead to initialize my
   array to zero.
