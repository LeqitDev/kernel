#include "include/console.h"
#include "include/commands.h"


void proccedCommand(char * line) {
    if (contains(line, "echo")) {
        if (charinstring(line, '"')) {
            puts(line);
//            putc(charinstring(line, '"'));
//            for (int i = 0; i < charinstring(line, '"'); i++) { line++; putc(*line); }
//            puts(line);
        } else {
            puts("Try help");
        }
    }
}

