# Bug 1

## A) How is your program acting differently than you expect it to?
- Failed to access to query result hyperlinks on the webpage (static files). 
  When try to access the static file, the webpage prompt that it could not find the file, which means
  fr.ReadFile(&response_body) on HttpServer.cc:208 failed.

## B) Brainstorm a few possible causes of the bug
- The paths passed into the file reader is not safe so the IsPathSafe checker on FileReader.cc:49 failed. This means either one of the call to realpath on HttpUtils.cc:60 failed or comparison between two absolute paths failed on HttpUtils:68
- The URLParser failed or I extracted the file_name incorrectly on HttpServer.cc:200-202, and the incorrecly file_name lead to a failure when checking if the path is safe, or when the ReadFile function in the FileReader failed to read when calling ::ReadFileToString on line 53.
- The comparison in IsPathSafe (line 68-70) might be incorrect, which invalidates valid file names and root directory, which lead to failiures in ReadFile in FileReader

## C) How you fixed the bug and why the fix was necessary
- I used printing statements in each of the if-else branches to locate the bug. I found out the bug occured due to a failure when calling realpath on the file_name. Since the file_name we are looking for is not in the current working directory (hw4), when realpath try to look for the file_name in the current directory, it could not find any matching files, so it returned a nullptr which leads to failure of IsPathSafe that leads to a failure in ReadFile. 
To solve this, I added the root_dir before the file_name, before passing it into realpath, so realpath know where to find the file with the given file_path.


## Processes 1 that helped me avoid bugs

- I constantly refer back to the lecture/section slides and solution files from exercise 10 and 11 whenever I encounter anything uncertain when doing this homework. This helped me avoid potential bugs that might have been introduced by misusing library functions


## Processes 2 that helped me avoid bugs

- I added debugging printing statements while I implementing the functions, and testing my code more frequently. This helped me to quickly identify and clear small bugs instead of spending time figuring out where the bug is later.