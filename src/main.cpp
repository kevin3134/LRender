#include <iostream>
#include "../include/lrDisplay.h"
#include "../include/lrMath.h"
#include "../include/lrGraphic.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include <windows.h>
    #include "../src/lrDisplayWin.cpp"
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

static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 600;

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
///*
    float prev_time;
    callbacks_t callbacks;
    memset(&callbacks, 0, sizeof(callbacks_t));
    callbacks.key_callback = key_callback;

    std::cout << "LRender Main" <<std::endl;

    window_t* window = window_create("LRender", 800, 600);
    framebuffer_t *framebuffer = framebuffer_create(WINDOW_WIDTH, WINDOW_HEIGHT);


    input_set_callbacks(window, callbacks);
    prev_time = platform_get_time();

    while(!window_should_close(window)){
        float curr_time = platform_get_time();
        float delta_time = curr_time - prev_time;

        prev_time = curr_time;

        window_draw_buffer(window, framebuffer);
        input_poll_events();
    }
    window_destroy(window);
//*/

    return 0;
}