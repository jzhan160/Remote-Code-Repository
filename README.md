# Remote-Code-Repository
The final project of OOD
A Code Repository is a Program responsible for managing source code resources. A fully developed Repository will support file persistance,
managment of versions, and the acquisition and publication of source and document files. 
A Remote Repository adds the capability to access the Repository's functionality over a communication channel, e.g., interprocess 
communication, inter-network communication, and communication across the internet.
In this project I developed a Remote Code Repository, local Client, and communication channel that supports client access to the 
Repository from any internet enabled processor. The communication channel will use sockets and support an HTTP like message structure. 
It supports HTTP style request/response transactions, one-way communication, allowing asynchronous messaging between any two endpoints 
that are capable of listening for connection requests and connecting to a remote listener.

