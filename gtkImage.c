#include <stdio.h>
#include <string.h>
#include <math.h>

#include <gtk/gtk.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

void set_pixel(GdkPixbuf *pixbuf, int x, int y, guchar red, guchar green, guchar blue, guchar alpha, int n_channels, int rowstride)
{
    static count = 0;
/*    fprintf(stderr, "count:%d\n", count++);*/
    guchar *pixels, *p;

    pixels = gdk_pixbuf_get_pixels(pixbuf);
    p = pixels + y * rowstride  + x * n_channels;

/*    fprintf(stderr, "%p, %d, %d, %d, %d\n", p, p[0], p[1], p[2], p[3]);*/

    p[0] = red;
    p[1] = green;
    p[2] = blue;
    p[3] = alpha;

/*    if (y >= 500 && x < 512)*/
/*        fprintf(stderr, "set (%d, %d) R: %d G: %d B: %d\n", x, y, p[0], p[1], p[2], p[3]);*/

/*    if (count!=1025)*/
/*        return;*/

/*    fprintf(stderr, "%p ", &p[0]);*/
/*    fprintf(stderr, "%p ", &p[1]);*/
/*    fprintf(stderr, "%p ", &p[2]);*/
/*    fprintf(stderr, "%p\n", &p[3]);*/

/*    fprintf(stderr, "%p(%d) ", &p[0], p[0]);*/
/*    fprintf(stderr, "%p(%d) ", &p[1], p[1]);*/
/*    fprintf(stderr, "%p(%d) ", &p[2], p[2]);*/
/*    fprintf(stderr, "%p(%d)\n", &p[3], p[3]);*/
}

static void get_pixel(GdkPixbuf *pixbuf, int x, int y, guchar* red, guchar* green, guchar* blue, guchar* alpha, int n_channels, int rowstride)
{
    guchar *pixels, *p;

    pixels = gdk_pixbuf_get_pixels(pixbuf);
    p = pixels + y * rowstride + x * n_channels;
    *red   = p[0];
    *green = p[1];
    *blue  = p[2];
    *alpha = p[3];
/*    if (y >= 510)*/
/*        fprintf(stderr, "get (%d, %d) R: %d G: %d B: %d\n", x, y, *red, *green, *blue, *alpha);*/
}

void ImageProcessing(void)
{
    float multFactor = (3.0/2.0);

    // input
    char* inputImageName = "lena.bmp";
    GdkPixbuf *inputImagePixBuf = NULL;
    inputImagePixBuf = gdk_pixbuf_new_from_file(inputImageName, NULL);
    int width      = gdk_pixbuf_get_width(inputImagePixBuf);
    int height     = gdk_pixbuf_get_height(inputImagePixBuf);
    int n_channels = gdk_pixbuf_get_n_channels(inputImagePixBuf);
    int rowstride  = gdk_pixbuf_get_rowstride(inputImagePixBuf); //FIXME: float rwostride*multFactor????
    int colorspace  = gdk_pixbuf_get_colorspace(inputImagePixBuf);
    int bps  = gdk_pixbuf_get_bits_per_sample(inputImagePixBuf);
    gboolean has_alpha  = gdk_pixbuf_get_has_alpha(inputImagePixBuf);
/*    fprintf(stderr, "color: %d, bps: %d, has_alpha: %d\n", colorspace, bps, has_alpha);*/

    // output
    GdkPixbuf* outputImagePixBuf = NULL;
    int output_width = width*multFactor;
    int output_height = height*multFactor;
    outputImagePixBuf = gdk_pixbuf_new(colorspace, has_alpha, bps, output_width, output_height);

    int output_n_channels = gdk_pixbuf_get_n_channels(outputImagePixBuf);
    int output_rowstride = gdk_pixbuf_get_rowstride(outputImagePixBuf);

    // pixel processing
    int x, y;
    int i, j;
    for (j = 0; j < output_height; j++) {
        for (i = 0; i < output_width; i++) {
            x = round(i/multFactor);
            y = round(j/multFactor);
            guchar red = 0;
            guchar green = 0;
            guchar blue = 0;
            guchar alpha = 0;
            get_pixel(inputImagePixBuf, x, y, &red, &green, &blue, &alpha, n_channels, rowstride);
            set_pixel(outputImagePixBuf, i, j, red, green, blue, alpha, output_n_channels, output_rowstride);
        }
    }
    // gdk_pixbuf_finalize()?

    // output
    char* outputImageName = "test.bmp";
    gdk_pixbuf_save(outputImagePixBuf, outputImageName, "bmp", NULL, "quality", "100", NULL);

    return;
}

int main( int argc, char *argv[])
{
    gtk_init(&argc, &argv);

    ImageProcessing();

/*    gtk_main();*/
    return 0;
}
