/*
 * @path src/main.cpp
 * @file main.cpp
*/

#include <iostream>
#include "ui/launcher.hpp"

int main(void) {
   
    std::cout << Launcher::TITLE << std::endl;
    std::cout << Launcher::AUTHOR << std::endl;
    std::cout << Launcher::VERSION << std::endl;
    std::cout << Launcher::PROJECT_URL << std::endl;

    Launcher launcher;
    launcher.start();

   return 0;
}
