#include <iostream>
#include "../include/display.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include <windows.h>
    #include "../src/displayWin.cpp"
#elif __APPLE__
    #include <unistd.h>
    #include "../src/displayMacTest.cpp"
#endif


#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define lrSleep(a) Sleep(a)
#elif __APPLE__
    #define lrSleep(a) usleep(a)
#endif


static void key_callback(window_t *window, keycode_t key, int pressed) {
    std::cout << "press key: " << key << " pressed: " << pressed << std::endl;
    if(key == keycode_t::KEY_ESC){
        window->should_close = 1;
    }
}


int main(){

    float prev_time;
    int temp = -1;
    callbacks_t callbacks;
    memset(&callbacks, 0, sizeof(callbacks_t));
    callbacks.key_callback = key_callback;

    std::cout << "LRender Main" <<std::endl;

    window_t* window = window_create("LRender", 800, 600);



    input_set_callbacks(window, callbacks);
    prev_time = platform_get_time();

    while(!window_should_close(window)){
        float curr_time = platform_get_time();
        float delta_time = curr_time - prev_time;

        //std::cout<<"current time: "<< curr_time <<std::endl;
        //if()


        //lrSleep(1000);
        prev_time = curr_time;

        input_poll_events();
    }
    window_destroy(window);


    return 0;
}