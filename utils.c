#include "minirt.h"

// Function to draw pixels on the canvas
void put_pixel(t_canvas *canvas, int x, int y, int color)
{
    char *dst = canvas->addr + (y * canvas->line_length + x * (canvas->bits_per_pixel / 8));
    *(unsigned int *)dst = color;
}