#include <stdio.h>
#include "compiler.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>


/**
 * The function `send_instructions` reads instructions from standard input, tokenizes them, and handles
 * them based on whether tags have been saved or not.
 * 
 * @param tags_saved The `tags_saved` parameter is a boolean value that indicates whether the tags and
 * positions should be saved or not. If `tags_saved` is `true`, the function will translate each
 * instruction to binary and handle it accordingly. If `tags_saved` is `false`, the function will save
 * the tags
 * 
 * @return The function `send_instructions` is returning the number of lines read from the input.
 */
int send_instructions(bool tags_saved){
    char line[1024];  
    int token_counter =0; //count parts of instruction to identify labels
    int lines = 0; //keep count of instrutions read to save al tags
    while (fgets(line, sizeof(line), stdin)) {
        
        if(is_line(line)){
            
            
            
            char str[1024];
            strcpy(str, line);
            char *parts[4]; 

            // tokenize 'line' by commas
            char *token = strtok(str, ",");
            int count = 0;

            while (token != NULL) {
                parts[count++] = token;
                token = strtok(NULL, ",");
                token_counter++;
            }
            char* check_line = strtok(parts[token_counter-1], ";");
            //read file and save tags and positions
            if(tags_saved){
                if (token_counter!=1)
                {
                    //translate each instruction to binary
                    handle_instruction(parts, token_counter);
                }
                else{
                    lines--;
                }
            }
            else{
                //read file and save tags and positions
                if (token_counter==1)
                {
                    save_label_address(parts[0], lines);
                    lines--;
                }
                
            }
            //reset parameters
            token_counter=0;
            for (int i = 0; i < 4; i++) {
                parts[i] = ""; // An empty string
            }
            lines++;
        }else{
            fprintf(stderr, "Patrón no reconocido %s en linea %d\n",line, lines);
            exit(1);
        }

        
    }
    clearerr(stdin);
    return lines;
}

int main() {
    
    
    bool tags_saved = false; // save tag names
    int lines1 = send_instructions(tags_saved);
    fseek(stdin, 0, SEEK_SET);// set to read the stdin from beggining
    tags_saved = true; // translate instructions with tags saved
    int lines2 = send_instructions(tags_saved);
    return 0;
}

//gcc main.c compiler.c -o main
//./main < input_assembly.txt > output.txt