CMPE-220 Spring 2015

Group-SS05 Assignment 6 Echo Client/Server

Group Members:
------------------
Tej Kogekar - 009392526
Srushti Patel - 010122918
Khushbu Sheth - 010004033
Deep Tuteja - 010119941

Steps to compile program:
----------------------------
1)Copy all 3 files "server.c", "client.c", "Makefile" to a single folder.
2)Give "make clean" command. This command will clean old executable files if present.
3)Give "make" command that will complie both "client.c" and "server.c" files and will produce 2 executables files named "client" and "server" respectively.

Steps to run program:
-------------------------
1)Open two terminals.
2)First run server executable file by giving "./server" command on first terminal.
3)Run client executable file by giving "./client" command on second terminal.
4)Give input string on terminal where client is running.

Program Description:
------------------------
- We have used TCP socket to create our client and server.
- Following are the steps that we will have to perform to establish connection between client and server.

- Client
1) Create a Socket with the help of "socket()" System call.
2) Connect this socket to the address of server using "connect()" System call.
3) Send and receive data using "write()" and "read()" system calls.

- Server
1) Create a socket with the help of "socket()" System call.
2) Bind the socket to an address using the "bind()" System call.
3) Listen for connections with the "listen()" System call.
4) Accept a connection with the "accept()" System call. This call blocks untill a client connects with the server.
5) Send and receive data using "write()" and "read()" system calls.

- In our code server is listening on port number "22000". 
- Cient is using "127.0.0.1" IP address which is loop back IP address and port number "22000" to connect to server.