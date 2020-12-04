#ifndef LRIMAGE_H
#define LRIMAGE_H

typedef struct {int width, height, channels; unsigned char *buffer;} image_t;

/* image creating/releasing */
image_t *lrCreateImage(int width, int height, int channels);
void lrRelaseImage(image_t *image);
//image_t *lrLoadImage(const char *filename);
void lrSaveImage(image_t *image, const char *filename);

/* image processing */
void lrFlipImageHorzon(image_t *image);
void lrFlipImageVertical(image_t *image);
// image_t *image_resize(image_t *image, int width, int height);

//return the position of the pixel (channel with color)
unsigned char *lrGetPixel(image_t *image, int row, int col);


//tga 
image_t *lrLoadTGAImage(const char *filename);





#endif