/**
 * @file quarterColorPicker.cpp
 * @description Implements the QuarterColorPicker constructor and () function
 *              for CPSC 221 2023S PA2
 *
 *              THIS FILE WILL BE SUBMITTED
 */

#include "quarterColorPicker.h"

QuarterColorPicker::QuarterColorPicker(PNG& inputimg)
{
    // Complete your implementation below
    referenceimg = inputimg;
}

/**
 * Picks the color for pixel (x, y).
 *
 * Using the private reference image, scale each dimension by half
 * and tile the smaller image in a 2 x 2 grid over the original dimensions
 * of the image, and return the pixel at the appropriate coordinate from
 * the tiled image.
 *
 * The value of each pixel in the scaled image will be a bilinear interpolation
 * of a 2x2 pixel region from the original image, with each of the H/S/L/A channels
 * processed individually. Interpolate over the x-axis before the y-axis.
 *
 * Since interpolation will be the exact average between exactly two pixels, then
 * the Hue channel should be averaged over the shortest arc between the two values.
 * e.g. The average of H_1 = 10 and H_2 = 320 is H_avg = 345 and NOT 165.
 * In the case of two diametric hues being averaged, e.g. 10 and 190, the average will
 * have the smaller value, i.e. 100 and NOT 280.
 * 
 * @pre referenceimg width and height are both even
 */
HSLAPixel QuarterColorPicker::operator()(PixelPoint p)
{
    // Replace the line below with your implementation

    unsigned int scaled_x = p.x * 2;
    unsigned int scaled_y = p.y * 2;

    unsigned int rw = referenceimg.width();
    unsigned int rh = referenceimg.height();

    // get 4 pixels from the original image for interpolation
    HSLAPixel top_left = *referenceimg.getPixel(scaled_x % rw, scaled_y % rh);
    HSLAPixel top_right = *referenceimg.getPixel(scaled_x % rw + 1, scaled_y % rh);
    HSLAPixel bot_left = *referenceimg.getPixel(scaled_x % rw, scaled_y % rh + 1);
    HSLAPixel bot_right = *referenceimg.getPixel(scaled_x % rw + 1, scaled_y % rh + 1);

    double top_h = angleBtwn(top_left.h, top_right.h);
    double top_s = (top_left.s + top_right.s)/2; 
    double top_l = (top_left.l + top_right.l)/2; 
    double top_a = (top_left.a + top_right.a)/2;
    double bot_h = angleBtwn(bot_left.h, bot_right.h);
    double bot_s = (bot_left.s + bot_right.s)/2; 
    double bot_l = (bot_left.l + bot_right.l)/2;
    double bot_a = (bot_left.a + bot_right.a)/2;

    p.color.h = angleBtwn(top_h, bot_h);
    p.color.s = (top_s + bot_s)/2;
    p.color.l = (top_l + bot_l)/2;
    p.color.a = (top_a + bot_a)/2;

    return p.color;
}

/**
 * Add your private QuarterColorPicker function implementations below
 */

double QuarterColorPicker::angleBtwn(double a, double b) {
  double y = max(a, b);
  double x = min(a, b);

  if (y-x < 180) { // less than 180, return the average
    return (a + b)/2;
  } else if (y-x == 180) { // exactly equal to 180, return average < 180
    return ((a+b)/2 > 180) ? (a+b)/2-180 : (a+b)/2;
  } else { // greater than 180, 
    double c = y+((360-y)+x)/2;
    return (c > 360) ? c - 360 : c;
  }
}
