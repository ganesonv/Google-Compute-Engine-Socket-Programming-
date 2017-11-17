# Google-Compute-Engine-Socket-Programming-
A simple model of computational offloading is done. The client offloads a certain computations to the server which in turn distributes the load to the Backend servers. The servers together constitute a Google compute Engine.

In this project, client is offloading the and & or bit operations to the EDGE server. This inturn offloads the "AND" operation to the Backend server named server_and and "OR" operations to the backend_server_or. The client and edgeserver communicates over TCP connection and thus uses a Stream socket. The edgeserver communicates with its Backend servers over UDP connection and thus sues Datagram socket. 

--------------------------------------
Layout of the project
--------------------------------------
client.c
It opens the file given by the user through the command line and gets each line of the file. The lines are read using the fgets. Through the TCP connection, these lines are offloaded to the edge server and the total number of lines sent is counted. After all the computations are performed, the client receives the computed results line by line from the Edge server via the TCP connection.

edge.c
The edge server recieves line by line from the client using a TCP connection through port number: 23457.
The received lines are sequenced to ensure that the order of  the input file is maintained at the end of computation. The sequenced line is then sent to the respective backend servers through the UDP connection. Each of these backend servers have their own UDP connection. Once the computation is completed at the Backend server, it receives the computed results via UDP connection through port number: 24457. The received computations are then sent to the client via a TCP connection.

server_or.c
The server_or is a Backend server that receives the lines from the edge server via a UDP connection through port number: 21457. The received lines are decimated and the operands are extracted. These operators are in string format. Thus they are converted to binary and futher binary to integers. Bitwise OR operation is performed on the integers and the result is converted back to binary and then string. The resultatnt string is then sent to the edge server using the UDP connection.

server_and.c
The server_and is a Backend server that receives the lines from the edge server via a UDP connection through port number: 22457. The received lines are decimated and the operands are extracted. These operators are in string format. Thus they are converted to binary and futher binary to integers. Bitwise AND operation is performed on the integers and the result is converted back to binary and then string. The resultatnt string is then sent to the edge server using the UDP connection.

Makefile
Compiles all the code files and runs the individual source code files server_or, server_and and edge

--------------------------------------
Excecution of the project
--------------------------------------
Please perform the following steps to run the code:
	- open a terminal
	- Run: make all
	- Open four terminals and in each of those terminals:
	- Run: make server_or
	- Run: make server_and
	- Run: make edge
	- Run: ./client <input_filename.txt>

---------------------------------------
Format of all the lines exchanged
---------------------------------------
 
A file is read by the client from the command line argument. This file is of the form .txt

The lines are read from the .txt file given by the user, is sent as a string to the edge server via TCP connection. The string contains operator,operand1,operand2.

These lines are sequenced at edge and sent to the Backend server OR, AND as a string through udp connection. The string contains Sequence, operator, operand1, operand2

Both the backend servers receive the string sent by the edge through udp and once computation is performed, it sends back a string to the edge server for every line it receives. The string contains From server_or: Sequence, result, operand1 or operand2=Result
From server_and: Sequence, result, operand1 and operand2=Result

Once the string with the result is obtained throgh udp connection to the edge, it decimates the string and transfers the result to the cliet via TCP connection in the format of a string

---------------------------------------
Idiosyncrasies of the project
---------------------------------------

- The format of the lines in the file should be operator,operand1,operand2
- At the last line of the file may end with an enter line or not.
- The operator should be in lowercase
- Each of these operands in the file should contain atmost 10bits. The code is designed, keeping in mind that the user will not specify operands greater than 10 bits
- The code works fine when the client sends data to the server, one after the other but will fail when multiple clients try to connect to the server at the same time
- It is essential to boot up all the backend servers and the edge server before the client request.
