# Assignment 2
## Compilation and Execution
1. First, extract the files
2. Use 'make' command to compile the files
3. Finally use ./a.out to run the shell (No funky name used since, I was already down by then)
4. There runs my shell in your computer, bravo!
## Assumptions
1. The maximum length of file path allowed = 4096 bytes (unix/linux standard), if the length is maxed out then filename too long error is recieved(just like any other shell)
2. Though pinfo is said to be user defined, it wasn't clearly mentioned as to what is expected from it. Normally, user defined command '.exe' files would be stored in /bin/ folder. Since, nothing was mentioned, I just made it work like a builtin.
## Code files
1. cd command - chdir.c
2. pwd command - run_builtin.c
3. ls command - ls_com.c
4. echo command - echo_com.c
5. pinfo command - pinfo_com.c
6. System commands - run_syscom.c
7. Builitn commands(where we seperate and execute the aforementioned builtins) - run_builtin.c
8. main.c initialises everything
9. Printing prompt - prompt.c
## How the code works(step by step)
1. main.c first reads the input and tokenizes it with ';' as delimiter.
2. It then executes run_builtin.c if the input command is a builtin, otherwise, it runs run_syscom.c
3. run_builtin.c will then check as to which command it is and then runs the respective file(cd,ls,echo,pinfo,pwd)
4. run_syscom.c will check for '&' to decide if the process has to be run in background
## Extras
1. I did not implement bonus commands(history and nightswatch) due to lack of time.
## Feedback( Thought it was important to provide this time )
1. Though I learnt a lot during the assignment, I wasn't able to learn other subjects and read text books because figuring out lot of stuff on my own took immense amounts of time.
2. And in some cases, I had to debug the whole day to figure out the reasons behind some segmentation faults.
3. I think shorter but more number of assignments would be better because, that way maybe we can be confident and give our best.(Not so important suggestion)
4. Finally, it would be great if you provide with the ideal terminal code for assignment 2 after evals, so that we can see, learn and improve. (Most Important suggestion imo)
