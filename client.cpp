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

//this is the main client program
int main(int argc, char** argv) {
//    // check if number of argument is valid
//    if (argc != 3) {
//        cout << "Expected 2 arguments but " << argc-1 << " were given" <<  endl;
//        return 0;
//    }
//    // save the arguments inside the corresponding variables
//    const char* ip = argv[1];
//    string str_port = argv[2];
//
//    // input validation of ip number and port number
//    int clientPort;
//    if (!portValidation(str_port, clientPort)) {
//        cout<<"invalid port number"<<endl;
//        return 0;
//    }
//    if (!ip_validation(ip)){
//        cout<<"invalid ip number"<<endl;
//        return 0;
//    }
//    // socket initialization
//    const char* ip_address = ip;
//    const int port_no = clientPort;
//    int sock = socket(AF_INET, SOCK_STREAM,0);
//    //if socket was successfully created
//    if (sock<0){
//        perror("error creating socket");
//        return 0;
//    }
//    // trying to connect to server
//    struct sockaddr_in sin;
//    memset(&sin,0, sizeof(sin));
//    sin.sin_family = AF_INET;
//    sin.sin_addr.s_addr = inet_addr(ip_address);
//    sin.sin_port = htons(port_no);
//    if (connect(sock,(struct sockaddr*)&sin, sizeof(sin)) < 0) {
//        perror("error connecting to server");
//        return 0;
//    }

    while(true){
        char user_selection = '0';
        char data_addr[4096];
        bzero(data_addr, 4096);
        // receive from server
        char buffer[4096];
        bzero(buffer, 4096);
        int expected_data_len = sizeof(buffer);
        // receive
//        int read_bytes = recv(sock, buffer, expected_data_len, 0);
//        if (read_bytes == 0) {
//            // connection is closed
//            close(sock);
//            return 0;
//        }
//        else if (read_bytes < 0) {
//            cout<<"Error receiving data from server";
//            close(sock);
//            return 0;
//        }
        // print menu
//        cout<< buffer<< endl;
        cout<< "enter path"<< endl;
        //TODO init data addr
        if(user_selection == '0') {
            // selection from menu
            cin >> user_selection;
            data_addr[0] = user_selection;
        }
        if(user_selection == '1') {
            // get user input
            string path = "";
            cin >> path;
            string data;
            ifstream file(path);
            if (file) {
                string line = "";
                while (getline(file, line)) {
                    data += line;
                    data += "\n";
                }
                file.close();
            }
            // problem reading file
            if(data.size() == 0){
                continue;
            }
            strcpy(data_addr, data.c_str());
        }
//        // send info to server
//        int data_len = strlen(data_addr);
//        int sent_bytes = send(sock, data_addr, data_len, 0);
//        if (sent_bytes < 0) {
//            // error
//            cout << "Error sending data to server" << endl;
//            close(sock);
//            return 0;
//        }
    }
}