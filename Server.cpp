#include <iostream>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "includes/input_validation.h"
using namespace std;

int main(int argc, char** argv){

    // check if number of argument is valid
    if (argc != 3) {
        cout << "Expected 2 arguments but " << argc-1 << " were given" <<  endl;
        return 0;
    }

    ////////////// check if arg valid
    // put argument in variables
    const string path = argv[1];
    string port_str = argv[2];

    vector<string> data;
    vector<vector<double>> train;
    vector<string> labels;
    int vecSize = 0;
    int numberOfSamples = 0;
    int server_port;

    // read from file
    if (!fileReader(path, data, train, labels, vecSize, numberOfSamples)) {
        return 0;
    }

    // port value validation
    if(!portValidation(port_str, server_port)){
        cout << "k is not valid integer" << endl;
    }

    int sock = socket(AF_INET, SOCK_STREAM,0);
	if (sock < 0)
	{
		perror("error creating socket");
	}
	struct sockaddr_in sin;
	memset(&sin,0, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(server_port);
	if (bind(sock,(struct sockaddr*)&sin, sizeof(sin))<0){
		perror("error binding socket");
	}

    //////// change to 1?
	if (listen(sock,5)<0){
		perror("error listening to a socket");
	}
	struct sockaddr_in client_sin;
	unsigned int addr_len = sizeof(client_sin);
	int client_sock = accept(sock,(struct sockaddr*)&client_sin, &addr_len);
	if (client_sock<0){
		perror("error accepting client");
	}
	char buffer[4096];
	int expected_data_len = sizeof(buffer);
	int read_bytes = recv(client_sock, buffer, expected_data_len, 0);
	if (read_bytes == 0){
		// connection is closed
	}
	else if (read_bytes<0){
		// error
	}
	else{
		cout << buffer;
	}
	int sent_bytes = send(client_sock, buffer, read_bytes, 0);
	if (sent_bytes < 0){
		perror("error sending to client");
	}
	close(sock);
}