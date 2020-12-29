#ifndef LRIMAGE_H
#define LRIMAGE_H


class image_t{
    public:
        void init(int w, int h, int c);
        image_t(int w, int h, int c){init(w,h,c);}
        image_t(const char *filename);
        ~image_t();

        int width, height, channels; 
        unsigned char *buffer;
        // bool valid = false;
    
};

/* image processing */
void lrFlipImageHorzon(image_t *image);
void lrFlipImageVertical(image_t *image);

//return the position of the pixel (channel with color)
unsigned char *lrGetPixel(image_t *image, int row, int col);

//tga 
image_t *lrLoadTGAImage(const char *filename);





#endif