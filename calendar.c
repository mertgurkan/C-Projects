//
//  Student name: Mert Gurkan
//  Student ID: 260716883
//
//  comp206 assignment1 question 2
//
//  Created by Mert on 2018-02-09.
//  Copyright © 2018 Mert Gurkan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//defining months
char *months[]= {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};

int monthSize[]= {7, 8, 5, 5, 3, 4, 4, 6, 9, 7, 8, 8};

void calendar(int DAYSIZE, int FIRSTDAY)
{
    int begin = FIRSTDAY;
    
    for(int month = 0; month <= 11; month++)
    {
        printf("|");
        int lines= (DAYSIZE+3)*7 - 1;
        for(int i=0; i< lines; i++)
        {
            printf("-");
        }
        printf("|\n");
        
        printf("|");
        for(int space=0; space < ((lines - monthSize[month]) / 2); space++)
        {
            printf(" ");
        }
        printf("%s", months[month]);
        for(int space = ((lines - monthSize[month]) / 2) + monthSize[month]; space < lines; space++)
        {
            printf(" ");
        }
        printf("|\n");
        
        char days[7][10] = {{'S','u','n','d','a','y',' ', ' ', ' '},
            {'M','o','n','d','a','y',' ', ' ', ' '},
            {'T','u', 'e','s','d','a','y',' ', ' '},
            {'W','e','d','n','e','s','d','a','y'},
            {'T','h','u','r','s','d','a','y', ' ',},
            {'F','r','i','d','a','y', ' ', ' ', ' '},
            {'S','a','t','u','r','d','a','y', ' '}};
        
        printf("|");
        
        for(int i=0; i< lines; i++)
        {
            printf("-");
        }
        printf("|\n");
        
        
        char dayPrinted[DAYSIZE + 1];
        
        printf("|");
        
        int Space = 0;
        
        if(DAYSIZE > 9)
        {
            Space = DAYSIZE - 9;
        }
        for(int x=1; x < 8; x++)
        {
            printf(" ");
            int i = 0;
            for(i = 1; i <= DAYSIZE; i++)
            {
                char daySymbol = days[x][i];
                dayPrinted[i]= daySymbol;
            }
            dayPrinted[i] = '\0';
            printf("%s", dayPrinted);
            for(int i=1; i<=Space; i++)
            {
                printf(" ");
            }
            printf(" ");
            printf("|");
        }
        printf("\n");
        printf("|");
        
        for(int i=1; i <= lines; i++)
        {
            printf("-");
        }
        printf("|\n");
        printf("|");
        
        if(begin<7)
        {
            for(int i = 1;  i <= begin%7-1;  i++)
            {
                for(int i=0 ; i< DAYSIZE+2; i++)
                {
                    printf(" ");
                }
                printf("|");
            }
        }
        if(begin==7)
        {
            for(int i=1; i < 7; i++ )
            {
                for( int i=0 ; i< DAYSIZE+2; i++)
                {
                    printf(" ");
                }
                printf("|");
            }
        }
        for(int i= 1; i <= 30; i++)
        {
            if(i<10)
            {
                printf(" ");
                printf("%d", i);
                for(int x=1; x < DAYSIZE+1; x++)
                {
                    printf(" ");
                }
                printf("|");
                if((i + begin -1 ) % 7 == 0)
                {
                    printf(" \n");
                    printf("|");
                }
            }
            if(i>9)
            {
                printf(" ");
                printf("%d", i);
                for(int x=1; x < DAYSIZE; x++)
                {
                    printf(" ");
                }
                printf("|");
                if((i + begin -1 ) % 7 == 0)
                {
                    if(i != 30)
                    {
                        printf(" \n");
                        printf("|");
                    }
                }
            }
            
        }
        if(begin == 7)
        {
            for(int i=0; i < 6; i++)
            {
                for(int i=0 ; i< DAYSIZE+2; i++)
                {
                    printf(" ");
                }
                printf("|");
            }
        }
        if(begin != 7)
        {
            for(int i=1; i<(7 - begin); i++)
            {
                for(int i=0 ; i < DAYSIZE+2; i++)
                {
                    printf(" ");
                }
                printf("|");
            }
        }
        printf("\n");
        
        begin = begin +2;
        if(begin > 7)
        {
            begin = begin%7;
        }
    }
    
    printf("|");
    int lines = (DAYSIZE+3)*7-1;
    for(int i=0; i< lines; i++)
    {
        printf("-");
    }
    printf("|\n");
}
int main(int argc, char* argv[])
{
    
    int DAYSIZE = atof(argv[1]);
    
    int FIRSTDAY = atof(argv[2]);
    
    if(FIRSTDAY<1)
    {
        printf("Error: The first day of the week must be between 1 and 7.\n");
    }if(FIRSTDAY>7)
    {
        printf("Error: The first day of the week must be between 1 and 7.\n");
    }if(DAYSIZE<2)
    {
        printf("Error: Cannot print the days smaller than size 2.\n");
    }
    if((FIRSTDAY < 8) && (FIRSTDAY > 1) && (DAYSIZE > 1)){
        calendar(DAYSIZE, FIRSTDAY);
    }
}

