#include <iostream>
#include <windows.h>

#include "../include/display.h"

int main(){
    std::cout << "LRender Main" <<std::endl;

    window_create("LRender", 1000, 1000);

    Sleep(10000);

    return 0;
}