#include "include/console.h"
#include "include/commands.h"


void proccedCommand(char * line) {
    putc('\n');
    if (contains(line, "echo")) {
        int msgpos = charinstring(line, ' ');
        if (msgpos) {
            for (int i = 0; i < msgpos; i++) { line++;}
            puts(line);
        } else {
            puts("Try ");
            setTextColor(CONSOLE_COLOR_GREEN);
            puts("help");
        }
    } else if (contains(line, "help")) {
        puts("---------------------------------[Help]---------------------------------\n");
        puts("- echo [message]                                                       -\n");
        puts("---------------------------------[End]----------------------------------\n");
    } else {
        int msgpos = charinstring(line, ' ');
        if (msgpos) {
            for (int i = 0; i < msgpos; i++) { putc(line[i]); }
        } else {
            puts(line);
        }
        puts("isn't a command!\n");
        puts("Please try ");
        setTextColor(CONSOLE_COLOR_GREEN);
        puts("help");
    }
    return;
}

