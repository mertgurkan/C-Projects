//
//  Student name: Mert Gurkan
//  Student ID: 260716883
//
//  main.c
//  comp206a2
//
//  Created by Mert on 2018-03-15.
//  Copyright © 2018 Mert Gurkan. All rights reserved.
//

/* FILE: A2_bmp_helpers.c is where you will code your answers for Assignment 2.
 *
 * Each of the functions below can be considered a start for you.
 *
 * You should leave all of the code as is, except for what's surrounded
 * in comments like "REPLACE EVERTHING FROM HERE... TO HERE.
 *
 * The assignment document and the header A2_bmp_headers.h should help
 * to find out how to complete and test the functions. Good luck!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

int bmp_open( char* bmp_filename,        unsigned int *width,
             unsigned int *padding,      unsigned int *data_size,
             unsigned int *height,     unsigned int *bits_per_pixel,
             unsigned int *data_offset, unsigned char** img_data )
{
    
    // reading the file
    FILE* img = fopen(bmp_filename, "rb");
    
    // reading the 54 byte header
    unsigned char header[54];
    fread(header, sizeof(unsigned char), 54, img);
    
    char X = *(header);
    char Y = *(header+1);
    
    // wrong file type 19788 == XY
    if (X != 'X' || Y != 'Y')
    {
        printf( "Not right file format" );
        fclose(img);
        return 1;
    }
    
    data_size =     *(int)&header[2];
    data_offset=    *(int)&header[10];
    width =         *(int)&header[18];
    height =        *(int)&header[22];
    bits_per_pixel= *(int)&header[28];
    *padding=0; while ((*width*3+*padding) % 4!=0) *padding++;
    
    fseek(img, *data_offset, SEEK_SET);
    
    img_data = (unsigned char)malloc(*data_size);
    
    //reading the 54 byte header
    fread(*img_data, 1 , *data_size , img);
    
    //closing file
    fclose(img);
    return 0;
    
}

// We've implemented bmp_close for you. No need to modify this function
void bmp_close( unsigned char **img_data ){
    
    if( *img_data != NULL ){
        free( *img_data );
        *img_data = NULL;
    }
    
}

int bmp_mask( char* input_bmp_filename, char* output_bmp_filename,
             unsigned int a_min, unsigned int b_min, unsigned int a_max, unsigned int b_max,
             unsigned char red, unsigned char green, unsigned char blue )
{
    unsigned int img_height;
    unsigned int img_width;
    unsigned int data_size;
    unsigned int bits_per_pixel;
    unsigned int padding;
    unsigned int data_offset;
    unsigned char* img_data    = NULL;
    
    int open_return_code = bmp_open( input_bmp_filename, &img_width, &img_height, &bits_per_pixel, &padding, &data_size, &data_offset, &img_data );
    
    if( open_return_code ){ printf( "bmp_open failed. Returning from bmp_mask without attempting changes.\n" ); return -1; }
    
    if((a_min >= a_max) || (b_min >= b_max)){
        printf("Minimum co-ordinates need to be less than maximum co-ordinates. \n");
        return -1;
    }
    
    // helper variable
    unsigned int num_colors = bits_per_pixel/8;
    
    unsigned char *output_data = (char*)malloc(data_size);
    memcpy(output_data, img_data, data_size);
    
    // For every row in the image...
    for(int i = 0; i < img_height; i++){
        // Loop through every pixel in that row
        for(int j = 0; j < (img_width + padding); j++){
            // If the pixel you're currently on is within the specified range - change the color.
            if(j>=a_min && j<=a_max && i>=b_min && i<=b_max){
                output_data[ data_offset  + i*(img_width*num_colors+padding) + j*num_colors ] = blue;
                output_data[ data_offset  + i*(img_width*num_colors+padding) + j*num_colors + 1] = green;
                output_data[ data_offset  + i*(img_width*num_colors+padding) + j*num_colors + 2] = red;
            }
        }
    }
    
    FILE* out_file = fopen(output_bmp_filename, "wb");
    fwrite(output_data, 1, data_size, out_file);
    fclose(out_file);
    
    bmp_close( &img_data );
    
    return 0;
}

int bmp_collage( char* bmp_input1, char* bmp_input2, char* bmp_result, int x_offset, int y_offset ){
    
    unsigned int img_height1;
    unsigned int img_width1;
    unsigned int data_size1;
    unsigned int bits_per_pixel1;
    unsigned int padding1;
    unsigned int data_offset1;
    unsigned char* img_data1    = NULL;
    
    int open_return_code = bmp_open( bmp_input1, &img_width1, &img_height1, &bits_per_pixel1, &padding1, &data_size1, &data_offset1, &img_data1 );
    
    if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input1 ); return -1; }
    
    unsigned int img_height2;
    unsigned int img_width2;
    unsigned int data_size2;
    unsigned int bits_per_pixel2;
    unsigned int padding2;
    unsigned int data_offset2;
    unsigned char* img_data2    = NULL;
    
    open_return_code = bmp_open( bmp_input2, &img_width2, &img_height2, &bits_per_pixel2, &padding2, &data_size2, &data_offset2, &img_data2 );
    
    if( open_return_code ){ printf( "bmp_open failed for %s. Returning from bmp_collage without attempting changes.\n", bmp_input2 ); return -1; }
    
    // Calculate extra pixels needed 'before' and 'after'
    // the first image as well as start and end co-ordinates for overlayed image.
    unsigned int x_before = 0;
    unsigned int x_after = 0;
    unsigned int y_before = 0;
    unsigned int y_after = 0;
    
    unsigned int img_x_start = 0;
    unsigned int img_x_end = 0;
    unsigned int img_y_start = 0;
    unsigned int img_y_end = 0;
    
    if(x_offset <= 0){
        x_before = x_offset * -1;
        img_x_start = 0;
    }
    else{
        img_x_start = x_offset;
    }
    
    if(x_offset + img_width2 > img_width1){
        x_after = x_offset + img_width2 - img_width1;
        img_x_end = img_x_start + img_width2;
    }
    else{
        img_x_end = img_x_start + img_width2;
    }
    
    if(y_offset <= 0){
        y_before = y_offset * -1;
        img_y_start = 0;
    }
    else{
        img_y_start = y_offset;
    }
    
    if(y_offset + img_height2 > img_height1){
        y_after = y_offset + img_height2 - img_height1;
        img_y_end = img_y_start + img_height2;
    }
    else{
        img_y_end = img_y_start + img_height2;
    }
    
    // Calculating the new width, height and padding
    unsigned int new_img_width = x_before + img_width1 + x_after;
    unsigned int new_padding = (4 - ((new_img_width*bits_per_pixel1)/8)%4)%4;
    unsigned int new_img_height = y_before + img_height1 + y_after;
    
    printf("New Image WidthL %d \n", new_img_width);
    
    unsigned int max_data_offset = 0;
    if(data_offset1>data_offset2){
        max_data_offset = data_offset1;
    }
    else{
        max_data_offset = data_offset2;
    }
    
    // Calculating new data size
    unsigned int new_data_size = (new_img_width * (bits_per_pixel1/8) + new_padding)*new_img_height + max_data_offset;
    unsigned int num_colors = bits_per_pixel2 / 8;
    
    char* img_output = (char*)malloc(new_data_size);
    
    memcpy(img_output, img_data1, max_data_offset);
    
    // We need to use memcpy to copy multiple bytes onto img_output.
    memcpy(img_output + 2, &new_data_size, 4);
    memcpy(img_output + 18, &new_img_width, 4);
    memcpy(img_output + 22, &new_img_height, 4);
    
    // Apply for every row in the image
    for(int i = 0; i < new_img_height; i++){
        // Loop through every pixel in that row
        for(int j = 0; j < new_img_width; j++){
            // If the pixel is within the specified range, then set to background.
            if(j>=x_before && j<=(new_img_width-x_after) && i>=y_before && i<=(new_img_height-y_after)){
                img_output[ max_data_offset  + i*(new_img_width*num_colors+new_padding) + j*num_colors ] = img_data1[ data_offset1  + (i-y_before)*(img_width1*num_colors+padding1) + (j-x_before)*num_colors];
                img_output[ max_data_offset  + i*(new_img_width*num_colors+new_padding) + j*num_colors + 1] = img_data1[ data_offset1  + (i-y_before)*(img_width1*num_colors+padding1) + (j-x_before)*num_colors + 1];
                img_output[ max_data_offset  + i*(new_img_width*num_colors+new_padding) + j*num_colors + 2] = img_data1[ data_offset1  + (i-y_before)*(img_width1*num_colors+padding1) + (j-x_before)*num_colors + 2];
            }
            else{
                img_output[ max_data_offset  + i*(new_img_width*num_colors+new_padding) + j*num_colors ] = 0;
                img_output[ max_data_offset  + i*(new_img_width*num_colors+new_padding) + j*num_colors + 1] = 0;
                img_output[ max_data_offset  + i*(new_img_width*num_colors+new_padding) + j*num_colors + 2] = 0;
            }
        }
    }
    
    // Apply for every row in the image
    for(int i = 0; i < new_img_height; i++){
        // Loop through every pixel in that row
        for(int j = 0; j < new_img_width; j++){
            // If the pixel is within the specified range, then set to overlay.
            if(j>=img_x_start && j<img_x_end && i>=img_y_start && i<img_y_end){
                img_output[ max_data_offset  + i*(new_img_width*num_colors+new_padding) + j*num_colors ] = img_data2[ data_offset2  + (i-img_y_start)*(img_width2*num_colors+padding2) + (j-img_x_start)*num_colors];
                img_output[ max_data_offset  + i*(new_img_width*num_colors+new_padding) + j*num_colors + 1] = img_data2[ data_offset2  + (i-img_y_start)*(img_width2*num_colors+padding2) + (j-img_x_start)*num_colors + 1];
                img_output[ max_data_offset  + i*(new_img_width*num_colors+new_padding) + j*num_colors + 2] = img_data2[ data_offset2  + (i-img_y_start)*(img_width2*num_colors+padding2) + (j-img_x_start)*num_colors + 2];
            }
        }
    }
    
    
    FILE* out_file = fopen(bmp_result, "wb");
    fwrite(img_output, 1, new_data_size, out_file);
    fclose(out_file);
    
    bmp_close( &img_data1 );
    bmp_close( &img_data2 );
    
    return 0;
}

int max(int number1, int number2){
    if(number1>number2){
        return number1;
    }
    else{
        return number2;
    }
    
}
