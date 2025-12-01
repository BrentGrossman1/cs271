/****************************************
 * C-ploration 8 for CS 271
 *
 * [NAME] Brent Grossman
 * [TERM] FALL 2025
 *
 ****************************************/
#include "parser.h"
#include "symtable.h"
#include "error.h"

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
    unsigned int rom_address = 0;
    unsigned int line_num = 0;
    unsigned int instr_num = 0;
	char line[MAX_LINE_LENGTH] = {0};

	while(fgets(line, sizeof(line), file)) {
	    line_num++;
	    if(instr_num > MAX_INSTRUCTIONS) {
            exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS + 1);
        }
        strip(line);
        if(!*line) {
            continue;
        }
        char inst_type;
        if(is_Atype(line)) {
            inst_type = 'A';
            rom_address++;
        }
        else if(is_label(line)) {
            inst_type = 'L';
            char label[MAX_LABEL_LENGTH];
            extract_label(line, label);

            if(!isalpha(label[0])) {

                exit_program(EXIT_INVALID_LABEL, line_num, line);
            }
            if(symtable_find(label) != NULL) {

                exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, line);
            }

            symtable_insert(label, instr_num);
            continue;
        }
        else if(is_Ctype(line)) {
            rom_address++;
            inst_type = 'C';
        }
        printf("%u: %c  %s\n", instr_num, inst_type, line);
        instr_num++;
	}
}

/*
    Function: is_Atype
    ------------------
    args: const char *line is the name of the input arg, and this will be a string of one line from the asm file.
    returns: a true or false boolean value.
*/

bool is_Atype(const char *line) {
    if(line[0] == '@') return true;
    return false;
}

/*
    Function: is_label
    ------------------
    args: const char *line is the name of the input arg, and this will be a string of one line from the asm file.
    returns: a true or false boolean value.
*/
bool is_label(const char *line) {
    int last = strlen(line) - 1;
    if(line[0] == '(' && line[last] == ')') return true;
    return false;
}

/*
    Function: is_Ctype
    ------------------
    args: const char *line is the name of the input arg, and this will be a string of one line from the asm file.
    returns: a true or false boolean value.
*/
bool is_Ctype(const char *line) {
    if(!is_Atype(line) && !is_label(line)) return true;
    return false;
}

/*
    Function: extract_label
    -----------------------
    takes a line, and extracts the label name from the parenthesis.

    args: *line is the label passed to the function
    returns: the variable "label" containing the string name of the label.
*/
char *extract_label(const char *line, char* label) {
    int len = strlen(line) - 2;
    strncpy(label, line + 1, len);
    label[len] = '\0';
    return label;
}
