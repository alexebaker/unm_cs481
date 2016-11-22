/*----------------------------------------------------*
 * Author: Alex Baker                                 *
 * UNM ID: 101372834                                  *
 * UNM Email: alexebaker@unm.edu                      *
 *----------------------------------------------------*
 * CS 481                                             *
 * Programing Assignment 4                            *
 *----------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>


int main(int argc, char *argv[])
{
    int frames = 0;
    char *usage = "Usage: ./page_replace <frames (1-7)>";

    if (argc == 1)
    {
        frames = 3;
    }
    else if (argc == 2)
    {
        frames = atoi(argv[1]);
        if (frames < 1 || frames > 7)
        {
            fprintf(stderr, "Frames parameter must be betwee 1 and 7!\n%s", usage);
            exit(EXIT_FAILURE);
        }
    }
    else
    {
        fprintf(stderr, "%s", usage);
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}
