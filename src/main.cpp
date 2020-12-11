#include <iostream>
#include "../include/lrDisplay.h"
#include "../include/lrMath.h"
#include "../include/lrGraphic.h"
#include "../include/lrMesh.h"
#include "../include/lrTexture.h"
#include "../include/lrCamera.h"


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


static const int WINDOW_WIDTH = 800;
static const int WINDOW_HEIGHT = 800;

static bool PRINT_INFO = false;
vec3f_t currentEye(0,-5,1);

static void key_callback(window_t *window, keycode_t key, int pressed) {
    //std::cout << "press key: " << key << " pressed: " << pressed << std::endl;
    if(key == keycode_t::KEY_ESC){
        window->should_close = 1;
    }else if(key == keycode_t::KEY_P){
        PRINT_INFO = true;
    }else if(key == keycode_t::KEY_A){
        currentEye.x-=1;
    }else if(key == keycode_t::KEY_D){
        currentEye.x+=1;
    }else if(key == keycode_t::KEY_W){
        currentEye.z+=1;
    }else if(key == keycode_t::KEY_S){
        currentEye.z-=1;
    }
}


void test_lrMath(){
    vec4f_t vec1(1,1,1,1);
    vec4f_t vec2(1,1,1,1);

    mat4f_t mat1(1,2,3,4,0,1,4,6,5,6,0,7,0,0,0,1);
    mat4f_t mat2(1,2,3,4,0,1,4,6,5,6,0,7,0,0,0,1);

    mat4f_t mat3 = mat1*mat2;

    std::cout << mat1 <<std::endl;
    std::cout << mat2 <<std::endl;
    std::cout << mat3 <<std::endl;
}

void printInfo(float deltaTime){
    int freq = 1.0/deltaTime;
    std::cout << "CURRENT STATUS:" << std::endl;
    std::cout << "freq: " << freq << std::endl;
    std::cout << "currentEye: " << currentEye << std::endl;
}



int main(){

    
    float prev_time;
    callbacks_t callbacks;
    memset(&callbacks, 0, sizeof(callbacks_t));
    callbacks.key_callback = key_callback;

    std::cout << "LRender Main" <<std::endl;

    window_t* window = window_create("LRender", WINDOW_WIDTH, WINDOW_HEIGHT);
    framebuffer_t *framebuffer = lrCreateFramebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

    lrMesh *mesh= new lrMesh("../resource/witch/witch.obj");    
    image_t * image = lrLoadTGAImage("../resource/witch/witch_diffuse.tga");

    // lrMesh *mesh = new lrMesh("../resource/obj/african_head.obj");
    // image_t * image = lrLoadTGAImage("../resource/obj/african_head_diffuse.tga");

    lrColorTexture *texture = new lrColorTexture(image);\

    lrCamera *camera = new lrCamera();

    input_set_callbacks(window, callbacks);
    prev_time = platform_get_time();

    vec3f_t front(0,0,-1);


    while(!window_should_close(window)){
        

        lrClearColorFramebuffer(framebuffer, vec4f_t(0,0,0,1));
        lrClearDepthFramebuffer(framebuffer, 0);



        float curr_time = platform_get_time();
        float delta_time = curr_time - prev_time;
        prev_time = curr_time;

        if(PRINT_INFO){
            printInfo(delta_time);
            PRINT_INFO = false;
        }


        camera->setEye(currentEye);
        mat4f_t view = camera->lrLookAt();
        //mat4f_t viewPort = lrViewPort(WINDOW_WIDTH/8, WINDOW_HEIGHT/8, WINDOW_WIDTH*3/4, WINDOW_HEIGHT*3/4);
        front =  camera->lrFront();
        mat4f_t viewPort = lrViewPort(0, 0, WINDOW_WIDTH*3/4, WINDOW_HEIGHT*3/4);
        mat4f_t projection = lrProjection(front);



        //std::cout << currentEye << std::endl;
        // std::cout << "view: \n" << view << std::endl;
        // std::cout << "viewPort: \n" << viewPort << std::endl;

        
        for(int i=0;i<mesh->countEBO();i++){
            vec3i_t EBOVetex = mesh->getEBOVetex(i);
            vec3i_t EBOTesture = mesh->getEBOTexture(i);
            vec3f_t worldCoords[3];
            vec3i_t screenCoords[3];
            vec2f_t uvs[3];




            for(int j=0;j<3;j++){
                worldCoords[j] = mesh->getScaledVBOPostion(EBOVetex[j]);

                vec4f_t temp1 = vec4f_t(worldCoords[j][0],worldCoords[j][1],worldCoords[j][2],1);

                
                vec4f_t temp2 = viewPort * projection * view * temp1;

                int x = temp2.x/temp2.w;
                int y = temp2.y/temp2.w;
                int z = temp2.z/temp2.w;

                screenCoords[j]=vec3i_t(x,y,z);

                uvs[j]=mesh->getTextureUV(EBOTesture[j]);
            }
            vec3f_t n = (worldCoords[2]-worldCoords[0])^(worldCoords[1]-worldCoords[0]); 

            n.normalize();
            float intensity = n*front;
            if(intensity>0){
                lrDrawTriangle3DTexture(framebuffer, texture, screenCoords, uvs);
            }
        }

        //window_draw_image(window,image);
        window_draw_buffer(window, framebuffer);
        input_poll_events();
    }
    window_destroy(window);
    lrReleaseImage(image);
    lrReleaseFramebuffer(framebuffer);

    return 0;
}