/**
 * @file negativeBorderColorPicker.cpp
 * @description Implements the NegativeBorderColorPicker constructor and () function
 *              for CPSC 221 2023S PA2
 *
 *              THIS FILE WILL BE SUBMITTED
 */

#include "negativeBorderColorPicker.h"
#include <math.h>

NegativeBorderColorPicker::NegativeBorderColorPicker(PNG& inputimg, PixelPoint src_px, unsigned int width, double tol)
{
    // Complete your implementation below
    
    referenceimg = inputimg;
    source_px = src_px;
    borderwidth = width;
    tolerance = tol;
}

/**
 * Picks the color for pixel (x, y).
 *
 * Leaves a pixel unchanged if it is not on the border of a fill region
 *
 * If a pixel is on the border of a fill region, change it to the "negative"
 * of the original colour value in the Hue and Luminance channels.
 * Saturation and Alpha should remain unchanged.
 * Based on your understanding of HSL colour space from Lab_intro,
 * think about what would be an appropriate "negative" transformation for the
 * Hue and Luminance channels.
 *
 * Hint: a pixel is on the border of a fill region, if it is within "width"
 * pixels of a pixel which is NOT in the fill region!
 * 
 * Also: the border of the image is considered to be a border of the fill region.
 */
HSLAPixel NegativeBorderColorPicker::operator()(PixelPoint p)
{
    if (p.x < borderwidth || p.y < borderwidth || 
        referenceimg.width() - 1 - p.x < borderwidth || referenceimg.height() - 1 - p.y < borderwidth) {
            HSLAPixel pixel = *referenceimg.getPixel(p.x, p.y);
            pixel.h = (pixel.h >= 180.0) ? (pixel.h - 180.0) : (pixel.h + 180.0);
            pixel.l = 1.0 - pixel.l;
            return pixel;
    }
    for (unsigned int y = 0; y <= 2*borderwidth; y++) {
        for (unsigned int x = 0; x <= 2*borderwidth; x++) {
            HSLAPixel pixel = *referenceimg.getPixel(p.x-borderwidth+x, p.y-borderwidth+y);
            
            //double dist = sqrt(pow(x-borderwidth,2) + pow(y-borderwidth,2));
            double dist = sqrt((x-borderwidth)*(x-borderwidth) + (y-borderwidth)*(y-borderwidth));
            
            // check euclidian
            
            if (dist <= borderwidth && pixel.distanceTo(source_px.color) > tolerance) {
                HSLAPixel org_pixel = *referenceimg.getPixel(p.x, p.y);
                org_pixel.h = (org_pixel.h >= 180.0) ? (org_pixel.h - 180.0) : (org_pixel.h + 180.0);
                org_pixel.l = 1.0 - org_pixel.l;
                return org_pixel;
            }
        }
    }
    return *referenceimg.getPixel(p.x, p.y);
}

/**
 * Add your private QuarterColorPicker function implementations below
 */
