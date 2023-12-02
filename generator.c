/**
 * @file generator.c
 * @brief generating project
 * @author Simona Valkovská <xvalko12@vutbr.cz>
 *
 * Project: IFJ compiler
 */
#include "generator.h"


#include <stdio.h>



// Function definitions
void define_function(char *name, char *param_val[]) {
    LABEL_INST("JUMP","end");
    LABEL_INST("LABEL", name);
    ZERO_INST("PUSHFRAME");
    ONE_INST("DEFVAR", "LF", "retval");
    for (int i = 0; param_val[i] != NULL; i++) {
        ONE_INST("DEFVAR", "LF", ("param%d", i + 1));
        TWO_INST("MOVE", "LF", ("param%d", i + 1), "LF", ("%s", param_val[i])); // TODO retype string and float
    }
    frame = "LF";
}

void return_function(char *type, char *value) {
    TWO_INST("MOVE", "LF", "retval", type, value); // TODO retype string and float
    ZERO_INST("POPFRAME");
    ZERO_INST("RETURN");
    LABEL_INST("LABEL","end")
    frame = "TF";
}

void call_function(char *name, char *param_val[]) {
    ZERO_INST("CREATEFRAME");
    for (int i = 0; param_val[i] != NULL; i++) {
        TWO_INST("DEFVAR", "TF", ("param%d", i + 1));
        TWO_INST("MOVE", "LF", ("param%d", i + 1), "TF", ("%s", param_val[i])); // TODO retype string, float, func_call
    }
    LABEL_INST("CALL", name);
    frame = "TF";
}

void assign_func_call(char *frame_var, char *name) {
    TWO_INST("MOVE", frame_var, name, "TF", "retval");
}

void def_var(char *name, char *val, char *val_type) {
    TWO_INST("DEFVAR", frame, name);
    if (val) {
        THREE_INST("MOVE", frame, name, val_type, val);
    }
}

void pushs(char *type, char *val) {
    TWO_INST("PUSHS", type, val);
}

void pops(char *frame_var, char *name_var) {
    TWO_INST("POPS", frame_var, name_var);
}

void assign(char *frame_var_to, char *name_var_to, char *frame_var_from, char *name_var_from) {
    THREE_INST("MOVE", frame_var_to, name_var_to, frame_var_from, name_var_from);
}

int main() {
    file = tmpfile();
    if (file == NULL) {
        fprintf(stderr, "Error creating file.\n");
        return INTERNAL_ERR;
    }

    fclose(file);
    return NO_ERR;
}
