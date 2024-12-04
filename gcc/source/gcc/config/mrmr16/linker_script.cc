#include "mrmr16.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char linker_args_l[MAX_OUTPUT] = "";
static char object_files_l[MAX_OUTPUT];
static char directory_l[MAX_OUTPUT] = "/home/orsned01/MiniRISC_port/binutils/install/bin/";

static char first[MAX_OUTPUT] = "/home/orsned01/MiniRISC_port/binutils/install/bin/minirisc-linux-ld ";
static char thrid[MAX_OUTPUT] = " -o /home/orsned01/MiniRISC_port/gcc/install_mrmr_FULL/bin/exec.a";

void get_object_files_from_script(char *directory, char *output_buffer, size_t buffer_size)
{
    char command[256];
    FILE *fp;

    // Prepare the command to run the script
    snprintf(command, sizeof(command), "/home/orsned01/MiniRISC_port/gcc/source/gcc/config/mrmr16/list_objects.sh %s", directory);

    // Open a pipe to run the command
    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        return;
    }

    // Read the output of the script
    char line[128];
    output_buffer[0] = '\0'; // Initialize the output buffer
    while (fgets(line, sizeof(line), fp) != NULL) {
        // Concatenate the line to the output buffer
        // if (strlen(output_buffer) + strlen(line) + 1 < buffer_size) {
        //     strncat(output_buffer, line, buffer_size - strlen(output_buffer) - 1);
        // } else {
        //     fprintf(stderr, "Buffer overflow\n");
        //     break;
        // }
        strcat(output_buffer, line);
    }

    // Close the pipe
    pclose(fp);

    // Remove trailing newline characters
    char *newline_n = strchr(output_buffer, '\n');
    char *newline_r = strchr(output_buffer, '\r');
    while (newline_n != NULL || newline_r != NULL) {
        if(newline_r != NULL)
        {
            *newline_r = ' ';
            newline_r = strchr(output_buffer, '\r');
        }
        if(newline_n != NULL)
        {
            *newline_n = ' ';
            newline_n = strchr(output_buffer, '\n');
        }
    }
}

char* linker_function()
{
   // Get the .o files from the specified directory
   get_object_files_from_script(directory_l, object_files_l, sizeof(object_files_l));

   strcat(linker_args_l, first);
   strcat(linker_args_l, object_files_l);
   strcat(linker_args_l, thrid);
   return linker_args_l;
}