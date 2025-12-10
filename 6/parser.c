/****************************************
 * C-ploration 9 for CS 271
 *
 * [NAME] Brent Grossman
 * [TERM] FALL 2025
 *
 ****************************************/
#include "parser.h"
#include "symtable.h"
#include "error.h"

#include <stdbool.h>
#include <string.h>

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
int parse(FILE * file, instruction *instructions) {
    add_predefined_symbols();
    instruction instr;

    unsigned int rom_address = 0;
    unsigned int line_num = 0;
    unsigned int instr_num = 0;
	char line[MAX_LINE_LENGTH] = {0};
	char tmp_line[MAX_LINE_LENGTH];

	while(fgets(line, sizeof(line), file)) {
	    line_num++;

	    if(instr_num >= MAX_INSTRUCTIONS) {
            exit_program(EXIT_TOO_MANY_INSTRUCTIONS, MAX_INSTRUCTIONS);
        }

        strip(line);

        if(!*line) {
            continue;
        }
        // char inst_type;
        if(is_Atype(line)) {
            // inst_type = 'A';
            if(!parse_A_instruction(line, &instr.a)) {
                exit_program(EXIT_INVALID_A_INSTR, line_num, line);
            }
            instr.type = A_type_instruction;
            if (instr.a.is_addr) {
                printf("A: %d\n", instr.a.address);
            } else {
                printf("A: %s\n", instr.a.label);
            }
            rom_address++;
        }
        else if(is_label(line)) {
            // inst_type = 'L';

            char label[MAX_LABEL_LENGTH];

            extract_label(line, label);

            if(!isalpha(label[0])) {
                exit_program(EXIT_INVALID_LABEL, line_num, label);
            }
            if(symtable_find(label) != NULL) {
                exit_program(EXIT_SYMBOL_ALREADY_EXISTS, line_num, label);
            }

            symtable_insert(label, rom_address);
            continue;
        }
        else if(is_Ctype(line)) {
            strcpy(tmp_line, line);
            parse_C_instruction(tmp_line, &instr.c);
            if (instr.c.dest == DEST_INVALID) {
                exit_program(EXIT_INVALID_C_DEST, line_num, line);
            }
            if (instr.c.comp == COMP_INVALID) {
                exit_program(EXIT_INVALID_C_COMP, line_num, line);
            }
            if (instr.c.jump == JMP_INVALID) {
                exit_program(EXIT_INVALID_C_JUMP, line_num, line);
            }
            instr.type = C_type_instruction;
            printf("C: d=%d, c=%d, j=%d\n", instr.c.dest, instr.c.comp, instr.c.jump);
            rom_address++;
            // inst_type = 'C';
        }

        // printf("%u: %c  %s\n", instr_num, inst_type, line);
        instructions[instr_num++] = instr;
	}
	return instr_num;
}

/*
    Function: is_Atype
    ------------------
    args: const char *line is the name of the input arg, and this will be a string of one line from the asm file.
    returns: a true or false boolean value.
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
    returns: a true or false boolean value.
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
    args: const char *line is the name of the input arg, and this will be a string of one line from the asm file.
    returns: a true or false boolean value.
*/

bool is_Ctype(const char *line) {
    if(!is_Atype(line) && !is_label(line)) {
        return true;
    }
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
    int len = strlen(line) - 2;      // Length of line minus 2 to remove the end paren and NULL terminator
    strncpy(label, line + 1, len);   // string copy Destination: label, source: line starting at index 1, Max Num of char's: len
    label[len] = '\0';               // Add null terminator
    return label;
}


void add_predefined_symbols() {
    for(int i = 0; i < NUM_PREDEFINED_SYMBOLS; i++) {
        predefined_symbol s = predefined_symbols[i];
        symtable_insert(s.name, s.address);
    }
}

bool parse_A_instruction(const char *line, a_instruction *instr) {
    char *s = malloc(strlen(line + 1) + 1);
    if(s == NULL) {
        return false;
    }
    strcpy(s, line + 1);

    char *s_end = NULL;
    long result = strtol(s, &s_end, 10);

    if(s == s_end) {
        instr->is_addr = false;
        instr->label = malloc(strlen(s) + 1);
        if(instr->label == NULL) {
            free(s);
            return false;
        }
        strcpy(instr->label, s);
        free(s);
        return true;
    }
    else if(*s_end != '\0') {
        free(s);
        return false;
    }
    else {
        instr->is_addr = true;
        instr->address = (int16_t) result;
    }
    free(s);
    return true;
}

void parse_C_instruction(char *line, c_instruction *instr) {
    char *temp = NULL;
    char *jump = NULL;
    char *dest = NULL;
    char *comp = NULL;
    int a;

    temp = strtok(line, ";");
    jump = strtok(NULL, ";");
    dest = strtok(temp, "=");
    comp = strtok(NULL, "=");

    if (comp == NULL) {
        comp = dest;
        dest = NULL;
    }

    instr->jump = str_to_jumpid(jump);
    instr->dest = str_to_destid(dest);
    instr->comp = str_to_compid(comp, &a);
    instr->a = a;

}


opcode instruction_to_opcode(c_instruction instr) {
    opcode op = 0;
    op |= (7 << 13);           /* 111 prefix for C-instructions */
    op |= (instr.a << 12);     /* a bit */
    op |= (instr.comp << 6);   /* comp bits (7 bits) */
    op |= (instr.dest << 3);   /* dest bits (3 bits) */
    op |= instr.jump;          /* jump bits (3 bits) */
    return op;
}

void assemble(const char * file_name, instruction* instructions, int num_instructions) {
    char output_filename[256];
    strcpy(output_filename, file_name);
    strcat(output_filename, ".hack");

    FILE *fout = fopen(output_filename, "w");
    if (fout == NULL) {
        exit_program(EXIT_CANNOT_OPEN_FILE, output_filename);
    }
    int next_var_address = 16;

    for (int i = 0; i < num_instructions; i++) {
        opcode op = 0;
        instruction instr = instructions[i];

        if (instr.type == A_type_instruction) {
            if (instr.a.is_addr) {
                op = instr.a.address;
            } else {

                Symbol *sym = symtable_find(instr.a.label);

                if (sym != NULL) {

                    op = sym->addr;
                } else {

                    symtable_insert(instr.a.label, next_var_address);
                    op = next_var_address;
                    next_var_address++;
                }

                free(instr.a.label);
            }
        } else if (instr.type == C_type_instruction) {
            op = instruction_to_opcode(instr.c);
        }

        fprintf(fout, "%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",
                OPCODE_TO_BINARY(op));
    }

    fclose(fout);
}

