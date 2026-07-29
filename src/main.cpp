#include <iostream>
#include <string>
#include "gtp.hpp"

int main(){
    std::setvbuf(stdout, NULL, _IONBF, 0);

    std::string line;
    GTP gtp;
    while(std::getline(std::cin, line)){
        if (line == "quit") {
            std::cout << "= \n\n";
            break;
        }
        gtp.ProcessLine(line);
   }
   return 0;
}