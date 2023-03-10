A Unix shell program is implemented to support the following:
1. The internal shell command "exit" which terminates the shell
2. A command with no arguments
Example: ls ...etc
* Shell must block until the command completes and, if the return
code is abnormal, print out a message to that effect.
3. A command with arguments
Example: ls –l, cp source destination, rm filename.
4. A command, with or without arguments, executed in the background using &.
Example: firefox &
* In this case, your shell must execute the command and return
immediately, not blocking until the command finishes.

The shell shows that the opened process will be nested as a
child process to the shell program via opening the task manager found in the
operating system like in following figure. Additionally when a child process is terminated (main application will be interrupted by a
SIGCHLD signal)  write in a log
file . 
