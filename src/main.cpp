#include <iostream>
#include "../include/lrDisplay.h"
#include "../include/lrMath.h"
#include "../include/lrGraphic.h"
#include "../include/lrMesh.h"
#include "../include/lrTexture.h"


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

    window_t* window = window_create("LRender", WINDOW_WIDTH, WINDOW_HEIGHT);
    framebuffer_t *framebuffer = lrCreateFramebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);




    //lrMesh mesh("../resource/witch/witch.obj");    
    //image_t * texture = lrLoadTGAImage("../resource/witch/witch_diffuse.tga");

    lrMesh mesh("../resource/obj/african_head.obj");
    image_t * image = lrLoadTGAImage("../resource/obj/african_head_diffuse.tga");

    lrTexture texture(image);

    input_set_callbacks(window, callbacks);
    prev_time = platform_get_time();

    vec3f_t lightDir(0,0,-1);

    

    while(!window_should_close(window)){
        float curr_time = platform_get_time();
        float delta_time = curr_time - prev_time;

        prev_time = curr_time;
        
        for(int i=0;i<mesh.countEBO();i++){
            vec3i_t EBOVetex = mesh.getEBOVetex(i);
            vec3i_t EBOTesture = mesh.getEBOTexture(i);
            //vec2i_t screenCoords[3];
            vec3f_t worldCoords[3];
            vec3i_t screenCoords[3];

            for(int j=0;j<3;j++){
                worldCoords[j] = mesh.getScaledVBOPostion(EBOVetex[j]);
                int x = worldCoords[j][0] *500;
                int y = worldCoords[j][1] *500;
                int z = worldCoords[j][2] *500;

                screenCoords[j]=vec3i_t(x,y,z);
            }
            vec3f_t n = (worldCoords[2]-worldCoords[0])^(worldCoords[1]-worldCoords[0]); 

            n.normalize();
            float intensity = n*lightDir;

            //std::cout << intensity << std::endl;
            //intensity = 0.5;
            if(intensity>0){
                vec2f_t uv = mesh.getTextureUV(EBOTesture.y);

                // unsigned char * a = lrGetPixel(image, uv.x*1024, uv.x*1024);

                // unsigned char x = *a;
                // unsigned char y = *(a+1);
                // unsigned char z = *(a+2);


                // std::cout << (int)x << " "<< (int)y << " "<< (int)z << std::endl;
                // vec3f_t color = vec3f_t(z/255.0,y/255.0,x/255.0);

                //std::cout << uv <<std::endl;

                //unsigned char *dst_pixel = lrGetPixel(image, row, col);
                //std::cout<< texture.lrGetTextureRGB(screenCoords[0][0], screenCoords[0][1]).x << std::endl;
                vec3f_t color = texture.lrGetTextureRGBbyUV(uv.x,uv.y);
                vec4f_t color2 = vec4f_t(color.z,color.y,color.x,0.0);

                std::cout << color2 <<std::endl;
                lrDrawTriangle3D(framebuffer, screenCoords[0], screenCoords[1], screenCoords[2], color2);
            }
        }

        //lrDrawPoint2D(framebuffer, vec2i_t(1,300),vec4f_t(1,1,1,1));
        //lrDrawTriangleLine2D(framebuffer, vec2i_t(100,0), vec2i_t(100,300), vec2i_t(500,300), vec4f_t(1,1,1,1));


        //window_draw_image(window,texture);
        window_draw_buffer(window, framebuffer);
        input_poll_events();
    }
    window_destroy(window);
    lrReleaseImage(image);
    lrReleaseFramebuffer(framebuffer);
//*/

    return 0;
}