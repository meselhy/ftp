Messelhi's FTP

This FTP program has the below commands:
get          Download & view file
put		       Upload & view file
ls		       List files in directory
del		       Delete file N/A
quit		     Exit

User can login to the server using username and password: admin,admin, then can use any of these commands apart from del is under development.

ON CLIENT SIDE:
First you should compile the C file to be able to run it, to do so use this command line : gcc cs.c -o cs

ON SERVER SIDE:
You should compile the C file as well, to do so use this command line : gcc sc.c -o sc

Then use this command to run the file:  ./sc on server side and ./cs on client side.
PS: Server side must be working in order to client side create a connection.


For any issues feel free to drop me a line.
