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
vec3f_t currentEye(0,0,0);
float currentZoom = 30;

CameraMovement currentMove = CameraMovement::STOP;


static void key_callback(window_t *window, keycode_t key, int pressed) {
    //std::cout << "press key: " << key << " pressed: " << pressed << std::endl;
    if(pressed==0) return;

    if(key == keycode_t::KEY_ESC){
        window->should_close = 1;
    }else if(key == keycode_t::KEY_P){
        PRINT_INFO = true;
    }else if(key == keycode_t::KEY_A){
        currentMove = CameraMovement::LEFT;
    }else if(key == keycode_t::KEY_D){
        currentMove = CameraMovement::RIGHT;
    }else if(key == keycode_t::KEY_W){
        currentMove = CameraMovement::UPWARD;
    }else if(key == keycode_t::KEY_S){
        currentMove = CameraMovement::DOWNWARD;
    }else if(key == keycode_t::KEY_PLUS){
        currentMove = CameraMovement::FORWARD;
    }else if(key == keycode_t::KEY_MINS){
        currentMove = CameraMovement::BACKWARD;
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

void printInfo(float deltaTime, mat4f_t view, lrCamera * cam){
    int freq = 1.0/deltaTime;
    std::cout << "CURRENT STATUS:" << std::endl;
    std::cout << "freq: " << freq << std::endl;

    vec3f_t eye = cam->lrgetEye();
    vec3f_t front = cam->lrFront();
    vec3f_t right = cam->lrgetRight();
    vec3f_t up = cam->lrgetUp();
    std::cout << "front: " << front << std::endl;
    std::cout << "eye: " << eye << std::endl;
    std::cout << "right: " << right << std::endl;
    std::cout << "up: " << up << std::endl;
    std::cout << "distance:" << cam->getDistance() << std::endl;


    std::cout << "view matrix: \n" << view << std::endl;
}


int main(){
    float prev_time;
    callbacks_t callbacks;
    memset(&callbacks, 0, sizeof(callbacks_t));
    callbacks.key_callback = key_callback;

    std::cout << "LRender Main" <<std::endl;

    window_t* window = window_create("LRender", WINDOW_WIDTH, WINDOW_HEIGHT);
    framebuffer_t *framebuffer = lrCreateFramebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

    // lrMesh *mesh= new lrMesh("../resource/witch/witch.obj");    
    // image_t * image = lrLoadTGAImage("../resource/witch/witch_diffuse.tga");

    lrMesh *mesh = new lrMesh("../resource/obj/african_head.obj");
    image_t * image = lrLoadTGAImage("../resource/obj/african_head_diffuse.tga");

    lrColorTexture *texture = new lrColorTexture(image);

    lrCamera *camera = new lrCamera();
    camera->setEye(currentEye);

    input_set_callbacks(window, callbacks);
    prev_time = platform_get_time();


    //front is eye - center
    vec3f_t front(0,0,1);


    while(!window_should_close(window)){
        float curr_time = platform_get_time();
        float delta_time = curr_time - prev_time;
        prev_time = curr_time;

        camera->cameraMove(currentMove);
        //mat4f_t view;
        mat4f_t view = camera->lrLookAt();
        front =  camera->lrFront();

        
        //mat4f_t viewPort;
        mat4f_t viewPort = lrViewPort(200, 200, 300, 300);


        //mat4f_t projection;
        mat4f_t projection =  lrPerspective(TO_RADIANS(60.0f), (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 1000.0f);

        //print information when press key_P
        if(PRINT_INFO){
            printInfo(delta_time, view, camera);
            PRINT_INFO = false;
        }


        //clear states every round
        currentMove = CameraMovement::STOP;
        lrClearColorFramebuffer(framebuffer, vec4f_t(0,0,0,1));
        lrClearDepthFramebuffer(framebuffer, 10000);


        float min = 10000;
        float max = -10000;

        
        for(int i=0;i<mesh->countEBO();i++){
            vec3i_t EBOVetex = mesh->getEBOVetex(i);
            vec3i_t EBOTesture = mesh->getEBOTexture(i);
            vec3f_t worldCoords[3];
            vec3i_t screenCoords[3];
            vec2f_t uvs[3];


            for(int j=0;j<3;j++){
                //worldCoords[j] = mesh->getScaledVBOPostion(EBOVetex[j]);

                worldCoords[j] = mesh->getVBOPostion(EBOVetex[j]);

                vec4f_t currentWorldCoords = vec4f_t(worldCoords[j][0],worldCoords[j][1],worldCoords[j][2],1);      
                vec4f_t screenCoords4D = viewPort * projection * view * currentWorldCoords;

                int x = screenCoords4D.x/screenCoords4D.w;
                int y = screenCoords4D.y/screenCoords4D.w;
                int z = screenCoords4D.z/screenCoords4D.w;

                screenCoords[j]=vec3i_t(x,y,z);

                uvs[j]=mesh->getTextureUV(EBOTesture[j]);
            }
            vec3f_t n = (worldCoords[2]-worldCoords[0])^(worldCoords[1]-worldCoords[0]); 

            n.normalize();
            float intensity = n*front;
            //if(intensity<0){
                lrDrawTriangle3DTexture(framebuffer, texture, screenCoords, uvs);
            //}
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