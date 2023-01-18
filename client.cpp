#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;
#include <string>
#include <string.h>
#include <algorithm>
#include <vector>
#include "input_validation.h"
#include "Command1_UploadData.h"
#include "Command.h"
#include "DefaultIO.h"
#include "StandardIO.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <pthread.h>
void readFileToString(string path, string &outputData) {
    ifstream file(path);
    if (file) {
        string line = "";
        while (getline(file, line)) {
            outputData += line;
            outputData += "\n";
        }
        file.close();
    }
    // problem reading file
    if(outputData.size() == 0) {
        cout << "invalid input";
    }
}


void *receiveThread(void* s) {
    cout<<"this is the client receive thread function"<<endl;
    int *sock = (int*)s;
    int i =0;
    while(true) {
        // receive info from server
        char buffer[4096];
        bzero(buffer, 4096);
        int expected_data_len = sizeof(buffer);
        // receive
        int read_bytes = recv(*sock, buffer, expected_data_len, 0);
        i++;
        if (read_bytes == 0) {
            // connection is closed
            close(*sock);
            return 0;
        } else if (read_bytes < 0) {
            cout << "Error receiving data from server";
            close(*sock);
            return 0;
        }
        // print to user
        cout << buffer << flush;
    }
}

void *sendThread(void* s) {
    cout<< "this is the client send thread function"<<endl;
    int* sock = (int*)s;
    while(true) {
        // create a buffer to send user input to the server
        char data_addr[4096];
        // clean the buffer
        bzero(data_addr, 4096);
        // get user input
        string userInput = "";
        getline(cin, userInput);
        if(userInput == ""){
            userInput = "enter";
        }

        // send user choice to server
        strcpy(data_addr, userInput.c_str());
        int data_len = strlen(data_addr);
        int sent_bytes = send(*sock, data_addr, data_len, 0);
        if (sent_bytes < 0) {
            //error
            cout << "Error sending data to server" << endl;
            close(*sock);
            return 0;
        }
        // if command number 1 - check for file path
        if (userInput == "1") {
            // get user file path
            string path = "";
            getline(cin, path);
            // save the file into a new string to pass to the buffer later
            string data = "";
            readFileToString(path, data);
            // copy the file info into the buffer data_addr
            strcpy(data_addr, data.c_str());
            int data_len = strlen(data_addr);
            int sent_bytes = send(*sock, data_addr, data_len, 0);
            if (sent_bytes < 0) {
                //error
                cout << "Error sending data to server" << endl;
                close(*sock);
                return 0;
            }
        }
        else if (userInput == "8") {
            // exit
            close(*sock);
            return 0;
        }
    }
}
//this is the main client program
int main(int argc, char** argv) {
    cout << "this is the client program"<<endl;
    // check if number of argument is valid
    if (argc != 3) {
        cout << "Expected 2 arguments but " << argc - 1 << " were given" << endl;
        return 0;
    }
    // save the arguments inside the corresponding variables
    const char *ip = argv[1];
    string str_port = argv[2];

    // input validation of ip number and port number
    int clientPort;
    if (!portValidation(str_port, clientPort)) {
        cout << "invalid port number" << endl;
        return 0;
    }
    if (!ip_validation(ip)) {
        cout << "invalid ip number" << endl;
        return 0;
    }
    cout << "before connecting to server sock"<< endl;
    // socket initialization
    const char *ip_address = ip;
    const int port_no = clientPort;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    //if socket was successfully created
    if (sock < 0) {
        perror("error creating socket");
        return 0;
    }
    // trying to connect to server
    struct sockaddr_in sin;
    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);
    if (connect(sock, (struct sockaddr *) &sin, sizeof(sin)) < 0) {
        perror("error connecting to server");
        return 0;
    }
    cout << "after connecting to server sock"<<endl;
    // the tread identifiers
    pthread_t pthread_receive;
    pthread_t pthread_send;
    // set of thread attributes
    pthread_attr_t attr;
    // set the default attributes of the thread
    pthread_attr_init(&attr);
    //create the threads
    pthread_create(&pthread_receive, &attr, receiveThread, (void *)&sock);
    pthread_create(&pthread_send, &attr, sendThread, (void *)&sock);
    // wait for the threads to exit;
    pthread_join(pthread_receive, NULL);
    pthread_join(pthread_send, NULL);
    close(sock);
    return 0;

}