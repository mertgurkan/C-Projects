//
//  Student name: Mert Gurkan
//  Student ID: 26076883
//
//  comp206 assignment 1 question 1
//
//  Created by Mert on 2018-02-09.
//  Copyright © 2018 Mert Gurkan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    //command line arguments
    char *file_path = argv[1];
    float x = 0;
    float y = 0;
    //converts a and b to floats if true - gives an error if false
    
    if(atof(argv[2])){
        x = atof(argv[2]);
    }
    else{
        printf("Error: bad float arg");
        return -1;
    }
    if(atof(argv[3])){
        y = atof(argv[3]);
    }
    else{
        printf("Error: bad float arg");
        return -1;
    }
    
    //for opening gnuplot script
    FILE *fp = fopen(file_path, "r");
    
    //checking whether the file is open or closed
    if(fp == NULL)
    {
        printf("Failed to open file; %s\n", file_path);
        return 0;
    }
    
    //creating a buffer and tags to update them
    char line[2000];
    int tagX = 0;
    int tagY = 0;
    
    //while loop goes through all of the lines in the text
    while(fgets(line, sizeof(line), fp))
    {
        char *pointer;
        //if the line does not include any tags - print the line
        if((strstr(line, "#X#") == NULL) && (strstr(line, "#Y#") == NULL) )
        {
            printf("%s", line);
        }
        else if ((strstr(line, "#Y#") == NULL)) {
            pointer = strstr(line, "#X#");
            char *p =line;
            
            while(p != pointer)
            {
                printf("%c", *p);
                p++;
            }
            //changing tagA
            printf("%.6f", x);
            pointer = pointer +3;
            printf("%s", pointer);
            tagX = 1;
        }
        else {
            pointer = strstr(line, "#Y#");
            char *p =line;
            while(p != pointer)
            {
                printf("%c", *p);
                p++;
            }
            //changing tagB
            printf("%.6f", y);
            pointer = pointer +3;
            printf("%s", pointer);
            tagY = 1;
        }
    }
    //if the tags #X# and #Y# are not included in the file
    if (tagX == 0 || tagY == 0)
    {
        printf("Error: bad file; %s\n", file_path);
        return -1;
    }
    //closing file
    fclose(fp);
    return 0;
}
