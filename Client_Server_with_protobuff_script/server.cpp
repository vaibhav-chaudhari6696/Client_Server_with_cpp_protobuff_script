/* The port number is passed as an argument */
#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "clientserver.pb.h"
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // creating a protocol buffer message object
    assignment :: messageinfo  protoMessageObj;


     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[1024];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }

     // create a socket
     sockfd =  socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0) 
        error("ERROR opening socket");

     // clear address structure
     bzero((char *) &serv_addr, sizeof(serv_addr));

     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;  
     serv_addr.sin_addr.s_addr = INADDR_ANY;  
     serv_addr.sin_port = htons(portno);

     // This bind() call will bind  the socket to the current IP address on port, portno
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) 
              error("ERROR on binding");

     // This listen() call tells the socket to listen to the incoming connections.
     listen(sockfd,5);
     clilen = sizeof(cli_addr);

     cout<<"Server Started"<<endl;

    // The accept() call actually accepts an incoming connection
    // The accept() returns a new socket file descriptor for the accepted connection.
     newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
     if (newsockfd < 0) 
          error("ERROR on accept");

    //printf("server: got connection from %s port %d\n",inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));

    cout<<"server: got connection from "<<inet_ntoa(cli_addr.sin_addr)<<" port "<<ntohs(cli_addr.sin_port)<<"\n\n\n";

    bzero(buffer,1024);
    
    // Reading data from client. 
    string objHolder;
    n = read(newsockfd,buffer,1024);
    if (n < 0) error("ERROR reading from socket");

     // ParseFromString() method parses protocol buffer contained in string.
    objHolder=buffer;
    protoMessageObj.ParsePartialFromString(objHolder);


    // Calling Script to count number letters in message
    string msg=protoMessageObj.txtmessage();
    string command = "./script.sh  ";
    command+=msg;
    int statusc =system(command.c_str());
    if(statusc==-1)
    {
      cout<<"Error"<<endl;
      return -1;
    }
    int result=WEXITSTATUS(statusc);
    

    cout<<"Mssage From Client: "<<protoMessageObj.txtmessage()<<endl;
    cout<<"Result Count: "<<result<<endl;
    cout<<"Flg: "<<protoMessageObj.flg()<<endl;



    objHolder="";
    protoMessageObj.set_txtmessage("Done by server");
    protoMessageObj.set_id(result);
    protoMessageObj.set_flg(true);


     /*
    Serializing Protocol buffer.
    SerializeToString() method Serializes protocol buffer   and 
    stores the bytes in the given string. Note that the bytes are binary, not text; we only use the string class as a convenient container.
    */
    protoMessageObj.SerializeToString(&objHolder);

    // Sending  string containing bytes to client 
    n = write(newsockfd, objHolder.c_str(), objHolder.length());

     close(newsockfd);
     close(sockfd);
     return 0; 
}