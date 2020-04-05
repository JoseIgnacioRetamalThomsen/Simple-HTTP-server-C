//
// Created by pepe on 05/04/2020.
//

#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

/*
struct sockaddr_in
{
    __uint8_t         sin_len;
    sa_family_t       sin_family; // the address family
    in_port_t         sin_port; // port number
    struct in_addr    sin_addr; // address for socker
    char              sin_zero[8];
};
 */


const int PORT = 8080;

int main(){

    struct sockaddr_in address;
    int addrlen = sizeof(address);
    int new_socket;
    int server_fd;
    long valread;

    char *hello = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";


    // set memory for address
    memset((char *)&address, 0, sizeof(address));

    // set adress family
    address.sin_family = AF_INET;



    // create socket

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("cannot create socket");
        return 0;
    }

    // hton convert long interger/ short integer into network representation(htonl/htons)
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    //bind socket
    if (bind(server_fd,(struct sockaddr *)&address,sizeof(address)) < 0){
        perror("bind failed");
        return 0;
    }

    //listen for connection
    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        exit(EXIT_FAILURE);
    }if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    {
        perror("In accept");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            exit(EXIT_FAILURE);
        }

        char buffer[30000] = {0};
        valread = read( new_socket , buffer, 30000);
        printf("%s\n",buffer );
        write(new_socket , hello , strlen(hello));
        printf("------------------Hello message sent-------------------\n");
        close(new_socket);
    }

    return 1;
}