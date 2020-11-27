#include <iostream>
#include "../include/display.h"
#include "../include/lrMath.h"

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


void test_lrMath(){
    // vec2_t<int> testV1 = vec2_t<int>(1,1);
    // vec2_t<int> testV2 = vec2_t<int>(2,2);
    vec2f_t testV3 = vec2f_t(2,2);
    vec3f_t testV = vec3f_t(testV3,1.0);

    //std::cout << testV << std::endl;
    mat3f_t mat(1,2,3,0,1,4,5,6,0);

    //test mult float
    // mat = mat*2;
    //std::cout << mat << std::endl;

    //test transpost
    // mat = mat3f_transpose(mat);
    // std::cout << mat << std::endl;

    //test inverse
    // mat = mat3f_inverse(mat);
    // std::cout << mat << std::endl;

    //test  inverse transpose
    mat = mat3f_inverse_transpose(mat);
    std::cout << mat << std::endl;

}

int main(){
    //test_lrMath();
///*
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


        //lrSleep(1000);
        prev_time = curr_time;
        input_poll_events();
    }
    window_destroy(window);
//*/

    return 0;
}