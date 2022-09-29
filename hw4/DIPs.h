#ifndef DIPS_H
#define DIPS_H

#include "Constants.h"
#include "Image.h"

IMAGE *BlackNWhite(IMAGE *image);

IMAGE *Negative(IMAGE *image);

IMAGE *ColorFilter(IMAGE *image, int target_r, int target_g, int target_b, int threshold, int replace_r, int replace_g, int replace_b);

IMAGE *Edge(IMAGE *image);

IMAGE *VFlip(IMAGE *image);

IMAGE *VMirror(IMAGE *image);

IMAGE *AddBorder(IMAGE *image, char *color, int border_width);

IMAGE *Shuffle(IMAGE *image);

#endif