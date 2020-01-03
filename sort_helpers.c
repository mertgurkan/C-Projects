//
// Student name: Mert Gurkan
// Student id: 260716883
//
//  main.c
//  assignment4
//
//  Created by Mert on 2018-04-12.
//  Copyright © 2018 Mert Gurkan. All rights reserved.
//

#include "A4_sort_helpers.h"

// Function: read_all()
// Provided to read an entire file, line by line.
// No need to change this one.
void read_all( char *filename ){
    
    FILE *fp = fopen( filename, "r" );
    int line = 0;
    
    while( line < MAX_NUMBER_LINES &&
          fgets( text_array[line], MAX_LINE_LENGTH, fp ) )
    {
        line++;
    }
    
    text_array[line][0] = '\0';
    fclose(fp);
}

// Function: read_all()
// Provided to read only the lines of a file staring with first_letter.
// No need to change this one.
void read_by_letter( char *filename, char first_letter ){
    
    FILE *fp = fopen( filename, "r" );
    int line = 0;
    text_array[line][0] = '\0';
    
    while( fgets( text_array[line], MAX_LINE_LENGTH, fp ) ){
        if( text_array[line][0] == first_letter ){
            line++;
        }
        
        if( line == MAX_NUMBER_LINES ){
            sprintf( buf, "ERROR: Attempted to read too many lines from file.\n" );
            write( 1, buf, strlen(buf) );
            break;
        }
    }
    
    text_array[line][0] = '\0';
    fclose(fp);
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q1.

void sort_words()
{
    char temp[200];
    
    int row = sizeof(text_array)/sizeof(text_array[0]);
    
    //the number to put in 
    for (int x=0; x<MAX_NUMBER_LINES;x++)
    {
        if(strcmp(text_array[x],"\0") == 0){
            break;
        }
        for (int y=x+1; y<MAX_NUMBER_LINES ;y++)
        {
            if(strcmp(text_array[y],"\0") == 0){
                break;
            }
            if (strcmp(text_array[x], text_array[y]) > 0)
            {
                strcpy(temp, text_array[x]);
                strcpy(text_array[x], text_array[y]);
                strcpy(text_array[y], temp);
            }
        }
        
    }
}
void append(char* s, char c){
    int len = strlen(s);
    s[len] = c;
    s[len+1] = '\0';
}

sem_t alphabetical_sem[26];
sem_t main_sem;


// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2.
int initialize( ){
    
    //sprintf(buf, "Initializing.\n";
    //write(1, buf, strlen(buf));
    
    //creating a file with static name
    FILE *fp = fopen("sem_data", "w");
    
    //close the file
    fclose(fp);
    
    //creating all named semaphore
    for(int y = 0; y < 26; y++){
        char sem_name[80];
        strcpy(sem_name, "SEM_");
        append(sem_name, 'a'+y);
        
        //printf("%s\n", sem_name);
        
        sem_unlink(sem_name);
        if(y == 0){
            alphabetical_sem[y] = sem_open( sem_name, O_CREAT, 0666, 1 );
        }else{
            alphabetical_sem[y] = sem_open( sem_name, O_CREAT, 0666, 0 );
        }
    }
    
    //creating the main semaphore 
    sem_unlink("main_sem");
    main_sem = sem_open( "main_sem", O_CREAT, 0666, 0 );
    
    return 0;
}

// YOU MUST COMPLETE THIS FUNCTION FOR Q2 and Q3.
int process_by_letter( char* input_filename, char first_letter ){
    // For Q2, keep the following 2 lines in your solution (maybe not at the start).
    // Add lines above or below to ensure the "This process will sort..." lines
    // are printed in the right order (alphabetical).
    
    char sem_name[80];
    strcpy(sem_name, "SEM_");
    append(sem_name, first_letter);
    int id = first_letter - 'a';
    
    sem_wait(alphabetical_sem[id]);
    
    read_by_letter(input_filename,first_letter);
    
    //sorting the text array
    sort_words();
    
    //write to database
    FILE *fp = fopen("sem_data", "a");
    
    int line = 0;
    while(text_array[line][0] != '\0'){
        fprintf(fp, text_array[line]);
        line++;
    }
    
    fclose(fp);
    sprintf( buf, "This process will sort the letter %c.\n",  first_letter );
    
    if(id != 25){
        sem_post(alphabetical_sem[id+1]);
    }else{
        sem_post(main_sem);
    }
    
    
    // For Q3, uncomment the following 2 lines and integrate them with your overall solution.
    
    return 0;
}

// YOU COMPLETE THIS ENTIRE FUNCTION FOR Q2 and Q3.
int finalize( ){
    // For Q2, keep the following 2 lines in your solution (maybe not at the start).
    // Add lines above or below to ensure the "Sorting complete!" line
    // is printed at the very end, after all letter lines.
    
    sem_wait(main_sem); //until everybody is done
    
    //opening the file with static name
    //reading each line in order and print to the screen
    FILE* pf= fopen("sem_data", "r");
    if(pf){
        while (fgets ( buf, sizeof buf, pf ) != NULL){
            //printf("%s\n",buf);
            write(1,buf,strlen(buf));
        }
    }
    //closing the file
    fclose(pf);
    
    //deleting the file
    remove("sem_data");
    sprintf( buf, "Sorting complete!\n" );
    write( 1, buf, strlen(buf) );
    
    
    // For Q3, come up with a way to accumulate the sorted results from each
    // letter process and print the overal sorted values to standard out.
    // You are not allowed to read from the input file, or call sort_words
    // directly from this function.
    
    return 0;
}


