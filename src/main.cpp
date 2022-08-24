#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <string>
// #include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
// #include <sys/socket.h>
// #include <sys/types.h>
#include <unistd.h>
// #include <fcntl.h>

int main() {
  int port_address = 5002;
  std::string ip_address = "192.168.43.101";
  // Create a socket
  int requesting = socket(AF_INET, SOCK_STREAM, 0);
  std::cout << "requesting " << requesting << std::endl;
  // Create a hint structure for the server we're connecting with
  sockaddr_in hint;
  hint.sin_family = AF_INET;
  hint.sin_port = htons(port_address);
  inet_pton(AF_INET, std::to_string(port_address).c_str(), &hint.sin_addr);
  // connect to the server on the socket
  int connectRes = connect(requesting, (sockaddr *)&hint, sizeof(hint));
  if (connectRes == -1) {
    std::cout << "Can't connect to the Server " << std::endl;
    std::cout << "connectRes " << connectRes << std::endl;
  } else {
    // create a while loop to keep the client running
    char buf[4096];
    std::string userInput;
    do {
      // prompt the user for input
      std::cout << ">";
      std::getline(std::cin, userInput);
      // send the input to the server
      int sendRes =
          send(requesting, userInput.c_str(), userInput.size() + 1, 0);
      if (sendRes == -1) {
        std::cout << "Could not send to server! Whoops!" << std::endl;
      }
      // wait for response
      int bytesReceived = recv(requesting, buf, 4096, 0);
      if (bytesReceived == -1) {
        std::cout << "there was an error getting response from the server "
                  << std::endl;
      } else {
        // display response
        std::cout << "SERVER>" << std::string(buf, bytesReceived) << std::endl;
      }
    } while (userInput.size() > 0);
  }
  // close the socket
  close(requesting);
}