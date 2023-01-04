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

//this is the main client program
int main(int argc, char** argv){
    // check if number of argument is valid
    if (argc != 3) {
        cout << "Expected 2 arguments but " << argc-1 << " were given" <<  endl;
        return 0;
    }
    // save the arguments inside the corresponding variables
    const char* ip = argv[1];
    string str_port = argv[2];

    // input validation of ip number and port number
    int clientPort;
    if (!portValidation(str_port, clientPort)) {
        cout<<"invalid port number"<<endl;
        return 0;
    }
    if (!ip_validation(ip)){
        cout<<"invalid ip number"<<endl;
        return 0;
    }
    // socket initialization
    const char* ip_address = ip;
    const int port_no = clientPort;
    int sock = socket(AF_INET, SOCK_STREAM,0);
    //if socket was successfully created
    if (sock<0){
        perror("error creating socket");
        return 0;
    }
    // trying to connect to server
    struct sockaddr_in sin;
    memset(&sin,0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = inet_addr(ip_address);
    sin.sin_port = htons(port_no);
    if (connect(sock,(struct sockaddr*)&sin, sizeof(sin)) < 0) {
        perror("error connecting to server");
        return 0;
    }

    while(true){
        // get user input.
        string userInput;
        getline(cin, userInput);
        if (userInput=="-1"){
            // close the program
            close(sock);
            return 0;
        }
        // split the user input into 3 relevant inputs - vector, function name and number k.
        string str_vec;
        string distance_metric_name;
        string str_k;
        splitUserInput(userInput,str_vec, distance_metric_name, str_k);

        // sample validation before sending it to the server for classification
        vector<double> sample_vec;
        int k;
        // getting vector size
        string vec_without_spaces = str_vec;
        vec_without_spaces.erase(remove(vec_without_spaces.begin(),
                                        vec_without_spaces.end(), ' '), vec_without_spaces.end());
        int sample_vec_size = (str_vec).length()- vec_without_spaces.length()+1;
        //vector validation
        if (!testSampleValidation(str_vec, sample_vec, sample_vec_size)){
            cout << "invalid input"<<endl;
            continue;
        }
        // metric validation
        else if (!DistFuncValid(distance_metric_name)){
            cout << "invalid input"<<endl;
            continue;
        }
        // k validation
        else if(!isInteger(str_k)){
            cout << "invalid input"<<endl;
            continue;
        }
        else {
            // save the user vector as valid data for the server
            char data_addr[(userInput).length()];
            strcpy(data_addr, userInput.c_str());
            // send info to server
            int data_len = strlen(data_addr);
            int sent_bytes = send(sock, data_addr, data_len, 0);
            if (sent_bytes < 0) {
                // error
                cout<< "Error sending data to server"<< endl;
                close(sock);
                return 0;
            }
            //receive info from server
            char buffer[4096];
            bzero(buffer, 4096);
            int expected_data_len = sizeof(buffer);
            int read_bytes = recv(sock, buffer, expected_data_len, 0);
            if (read_bytes == 0) {
                // connection is closed
                close(sock);
                return 0;
            }
            else if (read_bytes < 0) {
                cout<<"Error receiving data from server";
                close(sock);
                return 0;
            }
            else {
                //print the classification or error message from server
                cout<< buffer<< endl;
            }
        }
    }
}