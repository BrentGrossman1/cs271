/****************************************
 * C-ploration 4 for CS 271
 *
 * [NAME] Brent Grossman
 * [TERM] FALL 2025
 *
 ****************************************/
#include "parser.h"
#include <stdbool.h>

/* Function: strip
 * -------------
 * remove whitespace and comments from a line
 *
 * s: the char* string to strip
 *
 * returns: the stripped char* string
 */
char *strip(char *s){

	char s_new[strlen(s) + 1];
	int i = 0;

	for(char *s2 = s; *s2; s2++) {
        if(*s2 == '/' && *(s2+1) == '/') {
            break;
            }
        else if(!isspace(*s2)) {
            s_new[i++] = *s2;
            }
        }
    s_new[i] = '\0';
	strcpy(s, s_new);

    return s;
}

/* Function: parse
 * -------------
 * iterate each line in the file and strip whitespace and comments.
 *
 * file: pointer to FILE to parse
 *
 * returns: nothing
 */
void parse(FILE * file){

	char line[MAX_LINE_LENGTH] = {0};

	while(fgets(line, sizeof(line), file)) {
        strip(line);
        if(!*line) {
            continue;
        }
        char inst_type;
        if(is_Atype(line)) {
            inst_type = 'A';
        }
        else if(is_label(line)) {
            inst_type = 'L';
        }
        else if(is_Ctype(line)) {
            inst_type = 'C';
        }
        printf("%c  %s\n", inst_type, line);
	}
}


/*
    Function: is_Atype
    ------------------
    args: const char *line is the name of the input arg, and this will be a string of one line from the asm file.
*/

bool is_Atype(const char *line) {
    if(line[0] == '@') {
        return true;
    }
        return false;
}


/*
    Function: is_label
    ------------------
    args: const char *line is the name of the input arg, and this will be a string of one line from the asm file.
*/

bool is_label(const char *line) {
    int last = strlen(line) - 1;
    if(line[0] == '(' && line[last] == ')') {
        return true;
    }
    return false;
}


/*
    Function: is_Ctype
    ------------------
    args:
*/

bool is_Ctype(const char *line) {
    if(!is_Atype(line) && !is_label(line)) {
        return true;
    }
    return false;
}



