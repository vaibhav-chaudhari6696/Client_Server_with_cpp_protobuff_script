#include<bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "clientserver.pb.h"
using namespace std;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // creating a protocol buffer message object
    assignment :: messageinfo  protoMessageObj;

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1024];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    
    // create a socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    // clear address structure
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portno);

    // This connect() call makes connection with server
    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    
    //Get Data Enter at Client Side
    string abc;
    cout << "Enter Message: ";
    getline(cin, abc);

    int count=0;
  

    bool flg=false;

    // Setting data in Protocol buffer message object
    protoMessageObj.set_txtmessage(abc);
    protoMessageObj.set_id(count);
    protoMessageObj.set_flg(flg);

    /*
    Serializing Protocol buffer.
    SerializeToString() method Serializes protocol buffer   and 
    stores the bytes in the given string. Note that the bytes are binary, not text; we only use the string class as a convenient container.
    */
    string objHolder;
    protoMessageObj.SerializeToString(&objHolder);

    // Sending  string containing bytes to server 
    n = write(sockfd, objHolder.c_str() , objHolder.length());
    if (n < 0) 
         error("ERROR writing to socket");

    bzero(buffer,1024);

    // Reading data from sever 
    n = read(sockfd, buffer, 1023);
   
    if (n < 0) 
         error("ERROR reading from socket");

    objHolder=buffer;

    // ParseFromString() method parses protocol buffer contained in string.
    protoMessageObj.ParseFromString(objHolder);

    cout<<"\n\n\nMessage From Server: "<<protoMessageObj.txtmessage()<<endl;
    cout<<"Number of letters in Provided Message : "<<protoMessageObj.id()<<endl;
    cout<<"Flag:  "<<protoMessageObj.flg()<<endl;


    close(sockfd);
    return 0;
}