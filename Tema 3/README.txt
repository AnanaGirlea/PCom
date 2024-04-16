Name: Anana Girlea
Class: 323CA

Homework 3 PCom - Client Web. Comunication with REST API. 

The assignment entailed implementing a library system that has client-server communication 
during this assignment.

The commands implemented are: register, login, enter_library, get_books, get_book, add_book, 
delete_book, logout, and exit. They are executed in the main function by 
comparing the input string with the names of the commands.

I used the laboratory 10's solutions as an inspiration for the assignment. I kept the "buffer.c", 
"buffer.h", "requests.c", "request.h", "heper.c", "helper.h".

I used the "parson.c" and "parson.h" as suggested to us in the homework task. I used the 
libraries to save user information, create object hierarchies, and print in JSON format.

Once the connection with the server is established, we wait for a command. 

The "register" command allows a new user to register. The client will have to enter a username 
and a password. If the username isn't taken, the registration process will be completed 
successfully and the message "Registration was executed successfully!" will be printed. If the 
username is taken, there will be an error message and the registration will not go through.

The "login" command allows a new user to log into their account. The client will have to enter 
a username and a password. If the username exists and the password is correct, the user will have 
received a cookie that allows them to navigate and access the library server and the message 
"Login was executed successfully!" will be printed. If the username or the password is not 
correct, there will be an error message and the login will not go through.

The "enter_library" command allows the user to virtually "enter" the library. If the user is 
logged in, the user will receive a token that proves access and the message "Entering the library 
was executed successfully" will be printed. If they are not logged in, there will be an error 
message and the entrance to the library will be denied.

The "get_books" command will print a list of all the books (name and id) in the library of the 
user that is logged in.

The "get_book" command will print the details of a particular book that was added to the system. 
It searches for the book by its id.If the id that we searched for isn't found, there will be 
an error message. 

The "add_book" command will add a book to the library of the user that is logged in. If 
successful, the message "Book was added successfully" will be printed. If the user isn't logged 
in or hasn't entered the library, there will be an error message.

The "delete_book" command will delete a book from the personal library of the user. I created the 
function "compute_delete_request" in "requests.c" which helps me delete a book. If successful, 
the message "Book deleted successfully" will be printed. If the user isn't logged in or hasn't 
entered the library, there will be an error message. Additionally, there is an error when the 
book id isn't found.

The "logout" command will allow a user to log out of their account. If successful, the message 
"Logout was executed successfully" will be printed. The token and the cookie will be deleted. 
After logging out, there will be no access to the library.

The "exit" command will end the program.

References and souces:

Laboratory 10 
https://ocw.cs.pub.ro/courses/pc/laboratoare/10

Parson JSON 
https://github.com/kgabis/parson
