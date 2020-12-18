#include <iostream>
#include "../include/lrDisplay.h"
#include "../include/lrMath.h"
#include "../include/lrGraphic.h"
#include "../include/lrMesh.h"
#include "../include/lrTexture.h"
#include "../include/lrCamera.h"
#include "../include/lrShader.h"


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

lrMesh *mesh;

mat4f_t view;
mat4f_t viewPort;
mat4f_t projection;
mat4f_t model;

vec3f_t front(0,0,1);

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


void printInfo(float deltaTime, mat4f_t view, lrCamera * cam, framebuffer_t* framebuffer){
    int freq = 1.0/deltaTime;
    std::cout << "CURRENT STATUS:" << std::endl;
    std::cout << "freq: " << freq << std::endl;
    std::cout << "drawCount: " << framebuffer->drawCount << std::endl;

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


class lrGouraudShader : public lrShader {
    public:
        virtual ~lrGouraudShader(){};
        virtual vec4f_t vertex(int EBO, int nthvert){
            vec3f_t norm = mesh->getNorm(mesh->getEBONorm(EBO)[nthvert]);
            vertexIntensity[nthvert] = lrMax(0.f, norm*front );
            vec4f_t gl_Vertex = vec4f_t(mesh->getVBOPostion(mesh->getEBOVetex(EBO)[nthvert]),1);
            return viewPort * projection * view * model * gl_Vertex;
        }

        virtual bool fragment(vec3f_t bar, vec4f_t &color){
            float intensity = vertexIntensity*bar;
            color = vec4f_t(intensity,intensity,intensity,1);
            return false;
        }
    private:
        vec3f_t vertexIntensity;
};


int main(){
    float prev_time;
    callbacks_t callbacks;
    memset(&callbacks, 0, sizeof(callbacks_t));
    callbacks.key_callback = key_callback;

    std::cout << "LRender Main" <<std::endl;

    window_t* window = window_create("LRender", WINDOW_WIDTH, WINDOW_HEIGHT);
    framebuffer_t *framebuffer = lrCreateFramebuffer(WINDOW_WIDTH, WINDOW_HEIGHT);

    // mesh= new lrMesh("../resource/witch/witch.obj");    
    // image_t * image = lrLoadTGAImage("../resource/witch/witch_diffuse.tga");

    mesh = new lrMesh("../resource/obj/african_head.obj");
    image_t * image = lrLoadTGAImage("../resource/obj/african_head_diffuse.tga");

    lrColorTexture *texture = new lrColorTexture(image);

    lrCamera *camera = new lrCamera();
    camera->setEye(currentEye);

    input_set_callbacks(window, callbacks);
    prev_time = platform_get_time();

    lrShader *shader = new lrGouraudShader();

    while(!window_should_close(window)){
        float curr_time = platform_get_time();
        float delta_time = curr_time - prev_time;
        prev_time = curr_time;

        camera->cameraMove(currentMove);
        front =  camera->lrFront();

        view = camera->lrLookAt();
        projection =  lrPerspective(TO_RADIANS(30.0f), (float)WINDOW_WIDTH/WINDOW_HEIGHT, 0.1f, 1000.0f);
        viewPort = lrViewPort(200, 200, 300, 300);

        //print information when press key_P
        if(PRINT_INFO){
            printInfo(delta_time, view, camera, framebuffer);
            PRINT_INFO = false;
        }

        framebuffer->drawCount = 0;
        //clear states every round
        currentMove = CameraMovement::STOP;
        lrClearColorFramebuffer(framebuffer, vec4f_t(0,0,0,1));
        lrClearDepthFramebuffer(framebuffer, 10000);

        
        for(int i=0;i<mesh->countEBO();i++){
            vec3i_t EBOVetex = mesh->getEBOVetex(i);
            vec3i_t EBOTesture = mesh->getEBOTexture(i);
            vec3i_t screenCoords[3];
            vec2f_t uvs[3];


            for(int j=0;j<3;j++){
                vec4f_t screenCoords4D = shader->vertex(i,j);

                int x = screenCoords4D.x/screenCoords4D.w;
                int y = screenCoords4D.y/screenCoords4D.w;
                int z = screenCoords4D.z/screenCoords4D.w;

                screenCoords[j]=vec3i_t(x,y,z);

                uvs[j]=mesh->getTextureUV(EBOTesture[j]);
            }
            //lrDrawTriangle3DTexture(framebuffer, texture, screenCoords, uvs);
            lrDrawTriangleShader(framebuffer, screenCoords, shader);
        }

        window_draw_buffer(window, framebuffer);
        input_poll_events();
    }
    window_destroy(window);
    lrReleaseImage(image);
    lrReleaseFramebuffer(framebuffer);

    return 0;
}