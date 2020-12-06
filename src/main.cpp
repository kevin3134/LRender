#include <iostream>
#include "../include/lrDisplay.h"
#include "../include/lrMath.h"
#include "../include/lrGraphic.h"
#include "../include/lrMesh.h"


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
    framebuffer_t *framebuffer = lrCreateFramebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);
    image_t * image = lrLoadTGAImage("../resource/witch/witch_diffuse.tga");

    lrMesh mesh("../resource/witch/witch.obj");

    input_set_callbacks(window, callbacks);
    prev_time = platform_get_time();

    while(!window_should_close(window)){
        float curr_time = platform_get_time();
        float delta_time = curr_time - prev_time;

        prev_time = curr_time;

        //lrDrawTriangle2D(framebuffer, vec2i_t(300,100), vec2i_t(200,200), vec2i_t(500,500), vec4f_t(1,1,1,1));
        
        for(int i=0;i<mesh.countEBO();i++){
            vec3i_t face = mesh.getEBOVetex(i);
            vec2i_t screenCoords[3];

            for(int j=0;j<3;j++){
                vec3f_t worldCoords = mesh.getVBOPostion(face[j]);
                int x = worldCoords[0];
                int y = worldCoords[1];
                int z = worldCoords[2];


                screenCoords[j]=vec2i_t(z*9, x*9+300);
            }
            lrDrawTriangle2D(framebuffer, screenCoords[0], screenCoords[1], screenCoords[2], vec4f_t(rand()%255,rand()%255,rand()%255,rand()%255));
        }

        //window_draw_image(window,image);
        window_draw_buffer(window, framebuffer);
        input_poll_events();
    }
    window_destroy(window);
    lrRelaseImage(image);
    lrReleaseFramebuffer(framebuffer);
//*/

    return 0;
}