clang_defer
===========

Mimics the Go Lang defer in C 
Very basic implementation of the Go Lang defer in C using the gcc extensions. The defer function diverts the return of the called function to run the deferred functions and, then, returns to the original caller. 


Usage
-----
The idea is to schedule the resource releasing just after acquire it. So we can open a file and defer the closing. Alloc memory and defer the deallocation. Acquire a mutex and defer the releasing, etc. So, this way, we can avoid resource leaking.

There is an usage example in the code.


Todo
----
[ ] Split out the code and example 
[ ] Write more examples
[ ] Write a makefile
[ ] Write tests



