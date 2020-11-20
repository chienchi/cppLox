//
// Created by osboxes on 10/23/20.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>

// Q : what parameter
// 1.  lvalue vs rvalue
// 2.  copy elision of Cpp17
void run(const std::string& source){
    std::string expected{"// our first Lox program\n"
                         "print \"Hello World!\";\n"};
    if (source != expected){
        std::cout << "Something Wrong\n";
    }
}
void runFile(char *path){


    //source.exceptions(source.exceptions()| std::ios::failbit | std::ifstream::badbit );

    try {
        std::ifstream source(path);
        source.exceptions(source.exceptions()| std::ios::failbit | std::ifstream::badbit );
        std::stringstream content;
        content << source.rdbuf();
       // if (! source.is_open())
        //    throw std::ios_base::failure(std::strerror(errno));
        // Q: is this the 'best' way to call run()
        run(content.str());
    } catch (std::ios_base::failure &failure){

        std::cerr << failure.what() << std::endl;
        std::cerr << std::strerror(errno) << std::endl;
    }// catch (std::exception& e){
     //   std::cerr << e.what() << std::endl;
   // }
}

void runPrompt(){
    std::string line;
    while(std::cin){
        std::cout << "> ";
        //std::cin >> line;
        // Todo:: still missing the new line character, does it matter?
        std::getline(std::cin,line);
        run(line);
    }
}
int main(int argc, char *argv[]){
    if (argc > 2){
        std::cerr << "Usage: CppLox [script]\n";
    } else if (argc == 2){
        runFile(argv[1]);
    } else {
        runPrompt();
    }
}

