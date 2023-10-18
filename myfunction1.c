// 211601653 Etai Wilentzik
#include <stdlib.h>
#include <stdbool.h>
#include "myfunction1.h"
#include "showBMP.h"
#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define KERNELSIZE 3
#define KERNELSCALE_FOR_1 9
#define KERNELSCALE_FOR_5 7
#define KERNELSCALE_FOR_2 1
#define KERNELSCALE_FOR_3 4
#define KERNELSCALE_FOR_4 2
#define true 1
#define false 0
#include <stdio.h>
/*
 * initialize_pixel_sum - Initializes all fields of sum to 0
 */
/* dont need this function.
void initialize_pixel_sum(pixel_sum * sum) {
	sum->red = sum->green = sum->blue = 0;
	// sum->num = 0;
	return;
}

/*
 * assign_sum_to_pixel - Truncates pixel's new value to match the range [0,255]
 */
static void assign_sum_to_pixel(pixel *current_pixel, pixel_sum sum, int kernelScale) {

    // divide by kernel's weight
    sum.red = sum.red / kernelScale;
    sum.green = sum.green / kernelScale;
    sum.blue = sum.blue / kernelScale;

    // truncate each pixel's color values to match the range [0,255]
    current_pixel->red = (unsigned char) (MIN(MAX(sum.red, 0), 255));
    current_pixel->green = (unsigned char) (MIN(MAX(sum.green, 0), 255));
    current_pixel->blue = (unsigned char) (MIN(MAX(sum.blue, 0), 255));
    return;
}
/*
* sum_pixels_by_weight - Sums pixel values, scaled by given weight
*/ //try to remove it
/*
static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight) {
    sum->red += ((int) p.red) * weight;
    sum->green += ((int) p.green) * weight;
    sum->blue += ((int) p.blue) * weight;
    // sum->num++;
    return;
}
 */


/*
 *  Applies kernel for pixel at (i,j)
 */
static pixel applyKernel(int dim, int i, int j, pixel *src, int kernel[3][3], int kernelScale, bool filter) {
    pixel_sum sum;
    pixel current_pixel;
    int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
    int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
    int min_row, min_col, max_row, max_col;
    pixel loop_pixel;
    //initialize_pixel_sum(&sum);
    //do the initialize_pixale sum
    sum.red= sum.blue = sum.green = 0;
    //end of initailze pixale sum.
    // sum->num = 0;
    //end of the initalize pixale sum
    int outterloop=MIN(i+1, dim-1);
    int innerloop=MIN(j+1, dim-1);
    int maxInnerloop=j-1;
    int jj=maxInnerloop;
    int ii=i-1;
    for(; ii <= outterloop; ii++) {
        for(; jj <= innerloop; jj++) {

            int kRow, kCol;

            // compute row index in kernel
            if (ii < i) {
                kRow = 0;
            } else if (ii > i) {
                kRow = 2;
            } else {
                kRow = 1;
            }

            // compute column index in kernel
            if (jj < j) {
                kCol = 0;
            } else if (jj > j) {
                kCol = 2;
            } else {
                kCol = 1;
            }
            // apply kernel on pixel at [ii,jj]
            //the real code.
            //sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
            //the declaration
            //static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight)
            //try to implement sum_pixels_by_weight here
            //try to do it instead of call sum_pixels by weight

            int kernalvale=kernel[kRow][kCol];
            pixel p=src[(ii)*(dim)+(jj)];
            sum.red += ((int) p.red) * kernalvale;
            sum.green += ((int) p.green) * kernalvale;
            sum.blue += ((int) p.blue) * kernalvale;
            //end of "call sum pixels by weight.

        }
        jj=maxInnerloop;
    }

    if (filter) {
        // find min and max coordinates

        ii = i-1;
        jj = maxInnerloop;
        max_intensity=(int)src[(ii*dim)+jj].red+(int)src[(ii*dim)+jj].green+(int)src[(ii*dim)+jj].blue;
        min_intensity=(int)src[(ii*dim)+jj].red+(int)src[(ii*dim)+jj].green+(int)src[(ii*dim)+jj].blue;
        min_row=i-1;
        max_row=i-1;
        max_col=j-1;
        min_col=j-1;
        for(; ii <= outterloop; ii++) {
            int k=ii*dim;
            for(; jj <= innerloop; jj++) {
                // check if smaller than min or higher than max and update
                // loop_pixel = src[calcIndex(ii, jj, dim)];
                loop_pixel=src[k+jj];


                if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
                    max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
                    max_row = ii;
                    max_col = jj;
                } else  if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
                    min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
                    min_row = ii;
                    min_col = jj;
                }

            }
            jj = maxInnerloop;
        }
        // filter out min and max
        //using ii instad of declaring a new variable.
        // min_row*dim+min_col
//        sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
//        sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
        // sum_pixels_by_weight(&sum, src[ min_row*dim+min_col ], -1);
        pixel p=src[ min_row*dim+min_col ];
        sum.red += ((int) p.red) * -1;
        sum.green += ((int) p.green) * -1;
        sum.blue += ((int) p.blue) * -1;

        p=src[max_row*dim+max_col];
        sum.red += ((int) p.red) * -1;
        sum.green += ((int) p.green) * -1;
        sum.blue += ((int) p.blue) * -1;

    }
    // assign kernel's result to pixel at [i,j]
    //this is fassster when i call the function.
   // assign_sum_to_pixel(&current_pixel, sum, kernelScale);

    return current_pixel;
}


static pixel applyKernel2(int dim, int i, int j, pixel *src) {
    pixel_sum sum;
    pixel current_pixel;
   // int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
   // int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
    //int min_row, min_col, max_row, max_col;
   // pixel loop_pixel;
    //initialize_pixel_sum(&sum);
    //do the initialize_pixale sum
    sum.red= sum.blue = sum.green = 0;
    //end of initailze pixale sum.
    // sum->num = 0;
    //end of the initalize pixale sum
   // int outterloop=MIN(i+1, dim-1);
    //int innerloop=MIN(j+1, dim-1);
    int maxInnerloop=j-1;
    int jj=maxInnerloop;
    int ii=i-1;

/*
    for(; ii <= outterloop; ii++) {
        for(; jj <= innerloop; jj++) {

            int kRow, kCol;

            // compute row index in kernel
            if (ii < i) {
                kRow = 0;
            } else if (ii > i) {
                kRow = 2;
            } else {
                kRow = 1;
            }

            // compute column index in kernel
            if (jj < j) {
                kCol = 0;
            } else if (jj > j) {
                kCol = 2;
            } else {
                kCol = 1;
            }
            // apply kernel on pixel at [ii,jj]
            //the real code.
            //sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
            //the declaration
            //static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight)
            //try to implement sum_pixels_by_weight here
            //try to do it instead of call sum_pixels by weight

            int kernalvale=kernel[kRow][kCol];
            pixel p=src[(ii)*(dim)+(jj)];
            sum.red += ((int) p.red) * kernalvale;
            sum.green += ((int) p.green) * kernalvale;
            sum.blue += ((int) p.blue) * kernalvale;
            //end of "call sum pixels by weight.

        }
        jj=maxInnerloop;
    }
    */
/*
    pixel *p=&src[(ii)*(dim)+(jj)];
    sum.red+=(*(p)).red *-1;
    sum.green+=(*(p)).green *-1;
    sum.blue+=(*(p)).blue *-1;

    sum.red+=(*(p+1)).red *-1;
    sum.green+=(*(p+1)).green *-1;
    sum.blue+=(*(p+1)).blue *-1;

    sum.red+=(*(p+2)).red *-1;
    sum.green+=(*(p+2)).green *-1;
    sum.blue+=(*(p+2)).blue *-1;

    sum.red+=(*(p+ii)).red *-1;
    sum.green+=(*(p+ii)).green *-1;
    sum.blue+=(*(p+ii)).blue *-1;


    sum.red+=(*(p+ii+1)).red *9;
    sum.green+=(*(p+ii+1)).green *9;
    sum.blue+=(*(p+ii+1)).blue *9;

    sum.red+=(*(p+ii+2)).red *-1;
    sum.green+=(*(p+ii+2)).green *-1;
    sum.blue+=(*(p+ii+2)).blue *-1;

    sum.red+=(*(p+ii+ii)).red *-1;
    sum.green+=(*(p+ii+ii)).green *-1;
    sum.blue+=(*(p+ii+ii)).blue *-1;

    sum.red+=(*(p+ii+ii+1)).red *-1;
    sum.green+=(*(p+ii+ii+1)).green *-1;
    sum.blue+=(*(p+ii+ii+1)).blue *-1;

    sum.red+=(*(p+ii+ii+2)).red *-1;
    sum.green+=(*(p+ii+ii+2)).green *-1;
    sum.blue+=(*(p+ii+ii+2)).blue *-1;


*/



    pixel *p=&(src[((ii)*(dim))+(jj)]);
    sum.red -= (int) (*(p)).red ;
    sum.green -= (int) (*(p)).green ;
    sum.blue -= (int) (*(p)).blue ;

    sum.red -= (int) (*(p+1)).red;
    sum.green -= (int) (*(p+1)).green;
    sum.blue -= (int) (*(p+1)).blue ;

    sum.red -= (int) (*(p+2)).red ;
    sum.green -= (int) (*(p+2)).green;
    sum.blue -= (int) (*(p+2)).blue ;

    sum.red -= (int) (*(p+dim)).red ;
    sum.green -= (int)(* (p+dim)).green;
    sum.blue -= (int) (*(p+dim)).blue;

    sum.red += (int) ((*(p+1+dim)).red * 9);
    sum.green += (int) ((*(p+1+dim)).green * 9);
    sum.blue += (int) ((*(p+1+dim)).blue * 9);

    sum.red -= (int) (*(p+dim+2)).red ;
    sum.green -= (int) (*(p+dim+2)).green ;
    sum.blue -= (int) (*(p+dim+2)).blue ;

    sum.red -= (int) (*(p+dim+dim)).red ;
    sum.green -= (int) (*(p+dim+dim)).green ;
    sum.blue -= (int) (*(p+dim+dim)).blue ;

    sum.red -= (int) (*(p+dim+dim+1)).red ;
    sum.green -= (int) (*(p+dim+dim+1)).green ;
    sum.blue -= (int) (*(p+dim+dim+1)).blue ;

    sum.red -= (int) (*(p+dim+dim+2)).red ;
    sum.green -= (int) (*(p+dim+dim+2)).green ;
    sum.blue -= (int) (*(p+dim+dim+2)).blue;

    // assign kernel's result to pixel at [i,j]
    //this is fassster when i call the function.
    assign_sum_to_pixel(&current_pixel, sum, KERNELSCALE_FOR_2);






    return current_pixel;
}

static pixel applyKernel3(int dim, int i, int j, pixel *src) {
    pixel_sum sum;
    pixel current_pixel;
  //  int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
   // int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
    //int min_row, min_col, max_row, max_col;
   // pixel loop_pixel;
    //initialize_pixel_sum(&sum);
    //do the initialize_pixale sum
    sum.red= sum.blue = sum.green = 0;
    //end of initailze pixale sum.
    // sum->num = 0;
    //end of the initalize pixale sum
    // int outterloop=MIN(i+1, dim-1);
    //int innerloop=MIN(j+1, dim-1);
    int maxInnerloop=j-1;
    int jj=maxInnerloop;
    int ii=i-1;

/*
    for(; ii <= outterloop; ii++) {
        for(; jj <= innerloop; jj++) {

            int kRow, kCol;

            // compute row index in kernel
            if (ii < i) {
                kRow = 0;
            } else if (ii > i) {
                kRow = 2;
            } else {
                kRow = 1;
            }

            // compute column index in kernel
            if (jj < j) {
                kCol = 0;
            } else if (jj > j) {
                kCol = 2;
            } else {
                kCol = 1;
            }
            // apply kernel on pixel at [ii,jj]
            //the real code.
            //sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
            //the declaration
            //static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight)
            //try to implement sum_pixels_by_weight here
            //try to do it instead of call sum_pixels by weight

            int kernalvale=kernel[kRow][kCol];
            pixel p=src[(ii)*(dim)+(jj)];
            sum.red += ((int) p.red) * kernalvale;
            sum.green += ((int) p.green) * kernalvale;
            sum.blue += ((int) p.blue) * kernalvale;
            //end of "call sum pixels by weight.

        }
        jj=maxInnerloop;
    }
    */
/*
    pixel *p=&src[(ii)*(dim)+(jj)];
    sum.red+=(*(p)).red *-1;
    sum.green+=(*(p)).green *-1;
    sum.blue+=(*(p)).blue *-1;

    sum.red+=(*(p+1)).red *-1;
    sum.green+=(*(p+1)).green *-1;
    sum.blue+=(*(p+1)).blue *-1;

    sum.red+=(*(p+2)).red *-1;
    sum.green+=(*(p+2)).green *-1;
    sum.blue+=(*(p+2)).blue *-1;

    sum.red+=(*(p+ii)).red *-1;
    sum.green+=(*(p+ii)).green *-1;
    sum.blue+=(*(p+ii)).blue *-1;


    sum.red+=(*(p+ii+1)).red *9;
    sum.green+=(*(p+ii+1)).green *9;
    sum.blue+=(*(p+ii+1)).blue *9;

    sum.red+=(*(p+ii+2)).red *-1;
    sum.green+=(*(p+ii+2)).green *-1;
    sum.blue+=(*(p+ii+2)).blue *-1;

    sum.red+=(*(p+ii+ii)).red *-1;
    sum.green+=(*(p+ii+ii)).green *-1;
    sum.blue+=(*(p+ii+ii)).blue *-1;

    sum.red+=(*(p+ii+ii+1)).red *-1;
    sum.green+=(*(p+ii+ii+1)).green *-1;
    sum.blue+=(*(p+ii+ii+1)).blue *-1;

    sum.red+=(*(p+ii+ii+2)).red *-1;
    sum.green+=(*(p+ii+ii+2)).green *-1;
    sum.blue+=(*(p+ii+ii+2)).blue *-1;


*/



    pixel *p=&(src[((ii)*(dim))+(jj)]);


    sum.red += (int) (*(p+dim)).red ;
    sum.green += (int)(* (p+dim)).green;
    sum.blue += (int) (*(p+dim)).blue;

    sum.red += (int) ((*(p+1+dim)).red )+((*(p+1+dim)).red );
    sum.green += (int) ((*(p+1+dim)).green )+((*(p+1+dim)).green );
    sum.blue += (int) ((*(p+1+dim)).blue )+((*(p+1+dim)).blue );

    sum.red += (int) (*(p+dim+2)).red ;
    sum.green += (int) (*(p+dim+2)).green ;
    sum.blue += (int) (*(p+dim+2)).blue ;

















    // assign kernel's result to pixel at [i,j]
    //this is fassster when i call the function.
    assign_sum_to_pixel(&current_pixel, sum, KERNELSCALE_FOR_3);




    return current_pixel;
}

static pixel applyKernel4(int dim, int i, int j, pixel *src) {
    pixel_sum sum;
    pixel current_pixel;
    int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
    int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
    int min_row, min_col, max_row, max_col;
    pixel loop_pixel;
    //initialize_pixel_sum(&sum);
    //do the initialize_pixale sum
    sum.red= sum.blue = sum.green = 0;
    //end of initailze pixale sum.
    // sum->num = 0;
    //end of the initalize pixale sum
    // int outterloop=MIN(i+1, dim-1);
    //int innerloop=MIN(j+1, dim-1);
    int maxInnerloop=j-1;
    int jj=maxInnerloop;
    int ii=i-1;

/*
    for(; ii <= outterloop; ii++) {
        for(; jj <= innerloop; jj++) {

            int kRow, kCol;

            // compute row index in kernel
            if (ii < i) {
                kRow = 0;
            } else if (ii > i) {
                kRow = 2;
            } else {
                kRow = 1;
            }

            // compute column index in kernel
            if (jj < j) {
                kCol = 0;
            } else if (jj > j) {
                kCol = 2;
            } else {
                kCol = 1;
            }
            // apply kernel on pixel at [ii,jj]
            //the real code.
            //sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
            //the declaration
            //static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight)
            //try to implement sum_pixels_by_weight here
            //try to do it instead of call sum_pixels by weight

            int kernalvale=kernel[kRow][kCol];
            pixel p=src[(ii)*(dim)+(jj)];
            sum.red += ((int) p.red) * kernalvale;
            sum.green += ((int) p.green) * kernalvale;
            sum.blue += ((int) p.blue) * kernalvale;
            //end of "call sum pixels by weight.

        }
        jj=maxInnerloop;
    }
    */
/*
    pixel *p=&src[(ii)*(dim)+(jj)];
    sum.red+=(*(p)).red *-1;
    sum.green+=(*(p)).green *-1;
    sum.blue+=(*(p)).blue *-1;

    sum.red+=(*(p+1)).red *-1;
    sum.green+=(*(p+1)).green *-1;
    sum.blue+=(*(p+1)).blue *-1;

    sum.red+=(*(p+2)).red *-1;
    sum.green+=(*(p+2)).green *-1;
    sum.blue+=(*(p+2)).blue *-1;

    sum.red+=(*(p+ii)).red *-1;
    sum.green+=(*(p+ii)).green *-1;
    sum.blue+=(*(p+ii)).blue *-1;


    sum.red+=(*(p+ii+1)).red *9;
    sum.green+=(*(p+ii+1)).green *9;
    sum.blue+=(*(p+ii+1)).blue *9;

    sum.red+=(*(p+ii+2)).red *-1;
    sum.green+=(*(p+ii+2)).green *-1;
    sum.blue+=(*(p+ii+2)).blue *-1;

    sum.red+=(*(p+ii+ii)).red *-1;
    sum.green+=(*(p+ii+ii)).green *-1;
    sum.blue+=(*(p+ii+ii)).blue *-1;

    sum.red+=(*(p+ii+ii+1)).red *-1;
    sum.green+=(*(p+ii+ii+1)).green *-1;
    sum.blue+=(*(p+ii+ii+1)).blue *-1;

    sum.red+=(*(p+ii+ii+2)).red *-1;
    sum.green+=(*(p+ii+ii+2)).green *-1;
    sum.blue+=(*(p+ii+ii+2)).blue *-1;


*/



    pixel *p=&(src[((ii)*(dim))+(jj)]);


    sum.red -= (int) (*(p+dim)).red+(*(p+dim)).red ;
    sum.green -= (int)(* (p+dim)).green+(* (p+dim)).green;
    sum.blue -= (int) (*(p+dim)).blue+(*(p+dim)).blue;

    sum.red += (int) ((*(p+1+dim)).red )+((*(p+1+dim)).red )+((*(p+1+dim)).red )+((*(p+1+dim)).red )+((*(p+1+dim)).red )+((*(p+1+dim)).red );
    sum.green += (int) ((*(p+1+dim)).green )+((*(p+1+dim)).green )+(int) ((*(p+1+dim)).green )+((*(p+1+dim)).green )+(int) ((*(p+1+dim)).green )+((*(p+1+dim)).green );
    sum.blue += (int) ((*(p+1+dim)).blue )+((*(p+1+dim)).blue )+(int) ((*(p+1+dim)).blue )+((*(p+1+dim)).blue )+(int) ((*(p+1+dim)).blue )+((*(p+1+dim)).blue );

    sum.red -= (int) (*(p+dim+2)).red +(int) (*(p+dim+2)).red;
    sum.green -= (int) (*(p+dim+2)).green + (int) (*(p+dim+2)).green;
    sum.blue -= (int) (*(p+dim+2)).blue +(int) (*(p+dim+2)).blue ;

















    // assign kernel's result to pixel at [i,j]
    //this is fassster when i call the function.
    assign_sum_to_pixel(&current_pixel, sum, KERNELSCALE_FOR_4);




    return current_pixel;
}



static pixel applyKernel1(int dim, int i, int j, pixel *src) {
    pixel_sum sum;
    pixel current_pixel;
   // int min_intensity = 766; // arbitrary value that is higher than maximum possible intensity, which is 255*3=765
    //int max_intensity = -1; // arbitrary value that is lower than minimum possible intensity, which is 0
   // int min_row, min_col, max_row, max_col;
   // pixel loop_pixel;
    //initialize_pixel_sum(&sum);
    //do the initialize_pixale sum
    sum.red= sum.blue = sum.green = 0;
    //end of initailze pixale sum.
    // sum->num = 0;
    //end of the initalize pixale sum
    int outterloop=MIN(i+1, dim-1);
    int innerloop=MIN(j+1, dim-1);
    int maxInnerloop=j-1;
    int jj=maxInnerloop;
    int ii=i-1;
    int cal;

    //not need any calcualte here because not using this
    //int kernalvale=1;
    for(; ii <= outterloop; ii++) {
        cal=ii*dim;
        for(; jj <= innerloop; jj++) {
            /*

            int kRow, kCol;

            // compute row index in kernel
            if (ii < i) {
                kRow = 0;
            } else if (ii > i) {
                kRow = 2;
            } else {
                kRow = 1;
            }

            // compute column index in kernel
            if (jj < j) {
                kCol = 0;
            } else if (jj > j) {
                kCol = 2;
            } else {
                kCol = 1;
            }
             */
            // apply kernel on pixel at [ii,jj]
            //the real code.
            //sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
            //the declaration
            //static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight)
            //try to implement sum_pixels_by_weight here
            //try to do it instead of call sum_pixels by weight


            pixel p=src[cal+(jj)];
            sum.red += ((int) p.red);
            sum.green += ((int) p.green) ;
            sum.blue += ((int) p.blue) ;
            //end of "call sum pixels by weight.

        }
        jj=maxInnerloop;
    }

    //not excuate because not going inside the if
//dont need it because its false anyway.
//
//    if (false) {
//
//
//        ii = i-1;
//        jj = maxInnerloop;
//        for(; ii <= outterloop; ii++) {
//            int k=ii*dim;
//            for(; jj <= innerloop; jj++) {
//                // check if smaller than min or higher than max and update
//                // loop_pixel = src[calcIndex(ii, jj, dim)];
//                loop_pixel=src[k+jj];
//                if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
//                    min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
//                    min_row = ii;
//                    min_col = jj;
//                }
//                if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
//                    max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
//                    max_row = ii;
//                    max_col = jj;
//                }
//            }
//            jj = maxInnerloop;
//        }
//        // filter out min and max
//        //using ii instad of declaring a new variable.
//        // min_row*dim+min_col
////        sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
////        sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
//        // sum_pixels_by_weight(&sum, src[ min_row*dim+min_col ], -1);
//        pixel p=src[ min_row*dim+min_col ];
//        sum.red += ((int) p.red) * -1;
//        sum.green += ((int) p.green) * -1;
//        sum.blue += ((int) p.blue) * -1;
//
//        p=src[max_row*dim+max_col];
//        sum.red += ((int) p.red) * -1;
//        sum.green += ((int) p.green) * -1;
//        sum.blue += ((int) p.blue) * -1;
//
//    }
//
//    // assign kernel's result to pixel at [i,j]
//    //this is fassster when i call the function.
    assign_sum_to_pixel(&current_pixel, sum, KERNELSCALE_FOR_1);




    return current_pixel;
}


static pixel applyKernel5(int dim, int i, int j, pixel *src) {
    pixel_sum sum;
    pixel current_pixel;
    int min_intensity ;// arbitrary value that is higher than maximum possible intensity, which is 255*3=765
    int max_intensity; // arbitrary value that is lower than minimum possible intensity, which is 0
    int min_row, min_col, max_row, max_col;
    pixel loop_pixel;
    //initialize_pixel_sum(&sum);
    //do the initialize_pixale sum
    sum.red= sum.blue = sum.green = 0;
    //end of initailze pixale sum.
    // sum->num = 0;
    //end of the initalize pixale sum
    int outterloop=i+1;
    int innerloop=j+1;
    int maxInnerloop=j-1;
    int jj=maxInnerloop;
    int ii=i-1;
    for(; ii <= outterloop; ii++) {
        for(; jj <= innerloop; jj++) {

            // apply kernel on pixel at [ii,jj]
            //the real code.
            //sum_pixels_by_weight(&sum, src[calcIndex(ii, jj, dim)], kernel[kRow][kCol]);
            //the declaration
            //static void sum_pixels_by_weight(pixel_sum *sum, pixel p, int weight)
            //try to implement sum_pixels_by_weight here
            //try to do it instead of call sum_pixels by weight


            pixel p=src[(ii)*(dim)+(jj)];
            sum.red += ((int) p.red);
            sum.green += ((int) p.green) ;
            sum.blue += ((int) p.blue);
            //end of "call sum pixels by weight.

        }
        jj=maxInnerloop;
    }


    // find min and max coordinates
    int k;
    ii = i-1;
    jj = maxInnerloop;
    int place =(ii*dim)+jj;
    max_intensity=(int)src[place].red+(int)src[place].green+(int)src[place].blue;
    min_intensity=(int)src[place].red+(int)src[place].green+(int)src[place].blue;
    min_row=i-1;
    max_row=i-1;
    max_col=j-1;
    min_col=j-1;
    for(; ii <= outterloop; ii++) {
      k=ii*dim;
        for(; jj <= innerloop; jj++) {
            // check if smaller than min or higher than max and update
            // loop_pixel = src[calcIndex(ii, jj, dim)];
            loop_pixel=src[k+jj];


            if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) > max_intensity) {
                max_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
                max_row = ii;
                max_col = jj;
            } else  if ((((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue)) <= min_intensity) {
                min_intensity = (((int) loop_pixel.red) + ((int) loop_pixel.green) + ((int) loop_pixel.blue));
                min_row = ii;
                min_col = jj;
            }

        }
        jj = maxInnerloop;
    }
    // filter out min and max
    //using ii instad of declaring a new variable.
    // min_row*dim+min_col
//        sum_pixels_by_weight(&sum, src[calcIndex(min_row, min_col, dim)], -1);
//        sum_pixels_by_weight(&sum, src[calcIndex(max_row, max_col, dim)], -1);
    // sum_pixels_by_weight(&sum, src[ min_row*dim+min_col ], -1);
    pixel p=src[ min_row*dim+min_col ];
    sum.red -= ((int) p.red) ;
    sum.green -= ((int) p.green);
    sum.blue -= ((int) p.blue);

    p=src[max_row*dim+max_col];
    sum.red -= ((int) p.red);
    sum.green -= ((int) p.green) ;
    sum.blue-= ((int) p.blue) ;


    // assign kernel's result to pixel at [i,j]
    //this is fassster when i call the function.
    assign_sum_to_pixel(&current_pixel, sum, KERNELSCALE_FOR_5);




    return current_pixel;
}






/*
* Apply the kernel over each pixel.
* Ignore pixels where the kernel exceeds bounds. These are pixels with row index smaller than kernelSize/2 and/or
* column index smaller than kernelSize/2
*/
void smooth(int KernalChoise,int dim, pixel *src, pixel *dst, int kernel[3][3], int kernelScale, bool filter) {

    int i, j;
    int calculate=dim - 1;
    int HalfKenrnalsize=1;
    j =i=  HalfKenrnalsize;
    int calc;


    switch (KernalChoise) {
        case 1:  for (; i < calculate; i++) {
                calc = (i) * (dim);
                for (; j < calculate; j++) {
//do this line
                    //dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize, kernel, kernelScale, filter);

                    dst[(calc) + j] = applyKernel1(dim, i, j, src);

                }
                j = HalfKenrnalsize;
            }
            break;
        case 2:
            for (; i < calculate; i++) {
                 calc = (i) * (dim);
                for (; j < calculate; j++) {
//do this line
                    //dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize, kernel, kernelScale, filter);

                    dst[(calc) + j] = applyKernel2(dim, i, j, src);

                }
                j = HalfKenrnalsize;
            }
            break;
        case 3:

            for (; i < calculate; i++) {
               calc = (i) * (dim);
                for (; j < calculate; j++) {
//do this line
                    //dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize, kernel, kernelScale, filter);

                    dst[(calc) + j] = applyKernel3(dim, i, j, src);

                }
                j = HalfKenrnalsize;
            }
            break;
        case 4:

            for (; i < calculate; i++) {
                 calc = (i) * (dim);
                for (; j < calculate; j++) {
//do this line
                    //dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize, kernel, kernelScale, filter);

                    dst[(calc) + j] = applyKernel4(dim, i, j, src);

                }
                j = HalfKenrnalsize;
            }
            break;
        default:
            for (; i < calculate; i++) {
                calc = (i) * (dim);
                for (; j < calculate; j++) {
//do this line
                    //dst[calcIndex(i, j, dim)] = applyKernel(dim, i, j, src, kernelSize, kernel, kernelScale, filter);

                    dst[(calc) + j] = applyKernel5(dim, i, j, src);

                }
                j = HalfKenrnalsize;
            }
            break;



        
    }

}

void charsToPixels(Image *charsImg, pixel* pixels) {

    int row=0, col=0;
    int k;
    for ( ; row < m ; row++) {
        int rowCol=row*n;
        int    Threerowcol=3*rowCol;
        for ( ; col < n ; col++) {
             k=col+col+col;
            pixels[rowCol + col].red = image->data[Threerowcol + k];
            pixels[rowCol+ col].green = image->data[Threerowcol+ k + 1];
            pixels[rowCol + col].blue = image->data[Threerowcol + k + 2];
        }
        col = 0;
    }
}



void pixelsToChars(pixel* pixels, Image *charsImg) {

    int row=0, col=0;
    int  rowCol;
    int Threerowcol;
    int y;
    for (; row < m ; row++) {
        rowCol=row*n;
         Threerowcol=3*rowCol;
        for ( ; col < n-2 ; col+=3){

y=rowCol + col;
            image->data[Threerowcol + 3*col] =pixels[y].red;
            image->data[Threerowcol+ 3*col + 1] =pixels[y].green;
            image->data[Threerowcol + 3*col + 2] =pixels[y].blue;

y=rowCol + col+1;
            image->data[Threerowcol + 3*(col+1)] = pixels[y].red;
            image->data[Threerowcol+ 3*(col+1) + 1] = pixels[y].green;
            image->data[Threerowcol + 3*(col+1) + 2] = pixels[y].blue;
            y=rowCol + col+2;
            image->data[Threerowcol + 3*(col+2)] = pixels[y].red;
            image->data[Threerowcol+ 3*(col+2) + 1] = pixels[y].green;
            image->data[Threerowcol + 3*(col+2) + 2] = pixels[y].blue;




        }
        for (; col <n ; col++) {

            image->data[Threerowcol + 3*col] = pixels[rowCol + col].red;
            image->data[Threerowcol+ 3*col + 1] = pixels[rowCol+ col].green;
            image->data[Threerowcol + 3*col + 2] = pixels[rowCol + col].blue;

        }


        col = 0;
    }
}
/*
void pixelsToChars(pixel* pixels, Image *charsImg) {

    int row, col;
    for (row = 0 ; row < m ; row++) {
        for (col = 0 ; col < n ; col++) {

            image->data[3*row*n + 3*col] = pixels[row*n + col].red;
            image->data[3*row*n + 3*col + 1] = pixels[row*n + col].green;
            image->data[3*row*n + 3*col + 2] = pixels[row*n + col].blue;
        }
    }
}
*/

void copyPixels(pixel* src, pixel* dst) {

    int row, col;
    int rowCol;
    int y;
    row = 0;
    col = 0;
    for ( ; row < m ; row++) {
        rowCol=row*n;
        for ( ; col < n-2 ; col+=3) {

y=rowCol+col;
            dst[y].red = src[y].red;
            dst[y].green = src[y].green;
            dst[y].blue = src[y].blue;
++y;
            dst[y].red = src[y].red;
            dst[y].green = src[y].green;
            dst[y].blue = src[y].blue;

            ++y;
            dst[y].red = src[y].red;
            dst[y].green = src[y].green;
            dst[y].blue = src[y].blue;
        }
        for (; col <n ; col++) {
            dst[rowCol+col].red = src[rowCol+col].red;
            dst[rowCol+col].green = src[rowCol+col].green;
            dst[rowCol+col].blue = src[rowCol+col].blue;
        }
        col = 0;
    }
}

void doConvolution(int KernalChoise,Image *image,  int kernel[3][3], int kernelScale, bool filter) {
    pixel* pixelsImg = malloc(m*n*sizeof(pixel));
    pixel* backupOrg = malloc(m*n*sizeof(pixel));

    charsToPixels(image, pixelsImg);
    copyPixels(pixelsImg, backupOrg);

    smooth(KernalChoise,m, backupOrg, pixelsImg, kernel, kernelScale, filter);

    pixelsToChars(pixelsImg, image);

    free(pixelsImg);
    free(backupOrg);
}

