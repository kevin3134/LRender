#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../include/image.h"
#include "../include/macro.h"
#include "../include/display.h"

struct window {
    void *userdata;
};

window_t *window_create(const char *title, int width, int height){
    return nullptr;
}
void window_destroy(window_t *window){
    return;
}

int window_should_close(window_t *window){
    return 0;
}
void window_set_userdata(window_t *window, void *userdata){
    return;
}
void *window_get_userdata(window_t *window){
    return nullptr;
}
void window_draw_image(window_t *window, image_t *image){
    return;
}
//void window_draw_buffer(window_t *window, framebuffer_t *buffer);

/* input related functions */
void input_poll_events(void){
    return;
}
int input_key_pressed(window_t *window, keycode_t key){
    return 0;
}
int input_button_pressed(window_t *window, button_t button){
    return 0;
}
void input_query_cursor(window_t *window, float *xpos, float *ypos){
    return;
}
void input_set_callbacks(window_t *window, callbacks_t callbacks){
    return;
}
