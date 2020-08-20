# Assignment 1
## Assumptions:
1. Minimum RAM size > 2MB
2. Regarding Q2.c --->(a) It is supposed to print the permissions of all existing files/directories i.e even if argv[2] is invalid, if argv[1] or argv[3] are valid then it must proceed to print their permissions.
--->(b) Does not print any specific error message for the directory(even if the name is long) because the status of directory is already being covered in the question of it's existence
3. exit() is allowed (as confirmed earlier by one of the TAs) 
4. code need not work on special files such as .pdf or .ppt
5. Additional info: Q1.c takes arround 14 sec to reverse a 1GB file on dell G3 15 i7 processor
