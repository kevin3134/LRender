#include <iostream>
#include "../include/display.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include <windows.h>
#elif __APPLE__
    #include <unistd.h>
    #include "../src/displayMacTest.cpp"
#endif



int main(){
    std::cout << "LRender Main" <<std::endl;

    window_create("LRender", 1000, 1000);

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    Sleep(10000);
#elif __APPLE__
    usleep(10000);
#endif


    return 0;
}