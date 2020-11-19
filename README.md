University of Delaware Fall 2019
CISC 361: Unix Shell 

The Assignment
You will write a simple shell in C with some "built-in" commands.

Overview
What you need to do is to have a loop that will print a prompt (consisting of a "prefix," which is initially a blank (space), followed by ' [cwd]> ', where cwd is the "current working directory," and will execute commands until the exit command is entered. In the loop you should check if the command is one of your built-in commands (given later) or a program in the search paths. You also need to support passing arguments to the commands (i.e., you will need to build an argv array). If the command entered is neither one of your built-in commands nor in the search paths, you should print "command: Command not found." where command is the command name that was entered.

Built-in Commands to support
    exit - obvious!
    which - same as the tcsh one (hint: you can write a function to do this and use it for finding the command to execute) [Review get_path_main.c in shell skeleton code for details]
    where - same as the tcsh one (reports all known instances of command in path) [Review get_path_main.c in shell skeleton code]
    cd - chdir(2) to directory given; with no arguments, chdir to the home directory, with a '-' as the only argument, chdirs to directory previously in, the same as tcsh does. [Review Stevens and Rago's Section 4.23 for details]
    pwd - print the current working directory. review Stevens and Rago's Section 4.23 for details]
    list - with no arguments, lists the files in the current working directory one per line. With arguments, lists the files in each directory given as an argument, with a blank line then the name of the directory followed by a : before the list of files in that directory. You will need to use opendir(3) and readdir(3). (Hint: read their respective man pages carefully, and refer to Fig. 1.3 of Stevens and Rago's APUE book)
    pid - prints the pid of the shell
    kill - When given just a pid sends a SIGTERM to it with kill(2). When given a signal number (with a - in front of it) sends that signal to the pid. (e.g., kill 5678, kill -9 5678).
    prompt - When ran with no arguments, prompts for a new prompt prefix string. When given an argument make that the new prompt prefix. For instance, let's assume cwd is /usa/cshen.

    Executing:
        Use the terminal to compile the program. 
            - In the terminal, type 'make'
        Then to execute the shell, 
            - Use "./mysh" 

Note: Segmentation Fault 11 when 'return'. Don't do it or it will exit shell. 