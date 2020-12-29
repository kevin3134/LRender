#include <iostream>
#include "../include/lrDisplay.h"
#include "../include/lrMath.h"
#include "../include/lrGraphic.h"
#include "../include/lrMesh.h"
#include "../include/lrTexture.h"
#include "../include/lrCamera.h"
#include "../include/lrShader.h"
#include "../include/lrStatus.h"



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

CameraMovement currentMove = CameraMovement::STOP;

lrStatus *status = new lrStatus();

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


void printInfo(float deltaTime, framebuffer_t* framebuffer){
    int freq = 1.0/deltaTime;
    std::cout << "CURRENT STATUS:" << std::endl;
    std::cout << "freq: " << freq << std::endl;
    std::cout << "drawCount: " << framebuffer->drawCount << std::endl;

    vec3f_t eye = status->camera->lrgetEye();
    vec3f_t front = status->camera->lrFront();
    vec3f_t right = status->camera->lrgetRight();
    vec3f_t up = status->camera->lrgetUp();
    // std::cout << "front: " << front << std::endl;
    // std::cout << "eye: " << eye << std::endl;
    // std::cout << "right: " << right << std::endl;
    // std::cout << "up: " << up << std::endl;
    // std::cout << "distance:" << status->camera->getDistance() << std::endl;
    // std::cout << "view matrix: \n" << status->view << std::endl;
}


int main(){
    float prev_time;
    callbacks_t callbacks;
    memset(&callbacks, 0, sizeof(callbacks_t));
    callbacks.key_callback = key_callback;

    std::cout << "LRender Main" <<std::endl;

    window_t* window = window_create("LRender", WINDOW_WIDTH, WINDOW_HEIGHT);
    framebuffer_t *framebuffer = lrCreateFramebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

    // status->mesh= new lrMesh("../resource/witch/witch.obj");    
    // image_t * image = lrLoadTGAImage("../resource/witch/witch_diffuse.tga");

    // status->mesh = new lrMesh("../resource/obj/african_head.obj");
    // image_t * imageDiff = lrLoadTGAImage("../resource/obj/african_head_diffuse.tga");
    // image_t * imageNorm = lrLoadTGAImage("../resource/obj/african_head_nm.tga");
    // image_t * imageSpec = lrLoadTGAImage("../resource/obj/african_head_spec.tga");
    // image_t * imageNormTang = lrLoadTGAImage("../resource/obj/african_head_nm_tangent.tga");


    status->mesh = new lrMesh("../resource/crab/crab.obj");
    image_t * imageDiff = lrLoadTGAImage("../resource/crab/crab_diffuse.tga");
    image_t * imageNorm = lrLoadTGAImage("none");
    image_t * imageNormTang = lrLoadTGAImage("../resource/crab/crab_normal.tga");
    image_t * imageSpec = lrLoadTGAImage("../resource/crab/crab_specular.tga");


    //init status
    status->texture0 = imageDiff == nullptr ? nullptr : new lrColorTexture(imageDiff);
    status->texture1 = imageNorm == nullptr ? nullptr : new lrNormTexture(imageNorm);
    status->texture2 = imageSpec == nullptr ? nullptr : new lrSpecTexture(imageSpec);
    status->texture3 = imageNormTang == nullptr ? nullptr : new lrNormTexture(imageNormTang);

    status->camera = new lrCamera();
    status->camera->setEye(vec3f_t(0,0,0));

    input_set_callbacks(window, callbacks);
    prev_time = platform_get_time();

    //lrShader *shader = new lrGouraudShader();
    //lrShader *shader = new lrGeneralShader();
    //lrShader *shader = new lrPhongShader();
    lrShader *shader = new lrPhongTangentShader();

    while(!window_should_close(window)){
        float curr_time = platform_get_time();
        float delta_time = curr_time - prev_time;
        prev_time = curr_time;

        status->camera->cameraMove(currentMove);

        status->view = status->camera->lrLookAt();
        status->projection =  lrPerspective(TO_RADIANS(30.0f), (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 100.0f);
        //status->viewPort = lrViewPort(0, 0, 500, 500);
        status->viewPort = lrViewPort(200, 200, 200, 200);


        //current only used in phong
        shader->update(status);

        //print information when press key_P
        if(PRINT_INFO){
            printInfo(delta_time, framebuffer);
            PRINT_INFO = false;
        }

        framebuffer->drawCount = 0;
        //clear states every round
        currentMove = CameraMovement::STOP;

        lrClearDefault(framebuffer);

        
        for(int i=0;i<status->mesh->countEBO();i++){
            vec3i_t EBOVetex = status->mesh->getEBOVetex(i);
            vec3i_t EBOTesture = status->mesh->getEBOTexture(i);
            vec3i_t screenCoords[3];


            for(int j=0;j<3;j++){
                vec4f_t screenCoords4D = shader->vertex(i,j,status);

                int x = screenCoords4D.x/screenCoords4D.w;
                int y = screenCoords4D.y/screenCoords4D.w;
                int z = screenCoords4D.z/screenCoords4D.w;

                screenCoords[j]=vec3i_t(x,y,z);
            }

            lrDrawTriangle(framebuffer, screenCoords, shader, status);
        }

        window_draw_buffer(window, framebuffer);
        input_poll_events();
    }

    window_destroy(window);


    lrReleaseFramebuffer(framebuffer);


    return 0;
}