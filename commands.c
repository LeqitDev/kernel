#include "include/console.h"
#include "include/commands.h"


void parseLine(const char * line, char * args[80]) {
    int i = 0;
    int argspos[40] = {0};
    char argsinstring[80] = "XXX";
    int arg = 0;
    while (*line) {
        if (*line != 0) {
            if (*line == ' ') {
                arg++;
                argspos[arg] = i + 1;
                argsinstring[i] = '\0';
            } else {
                argsinstring[i] = *line;
            }
        }
        line++;
        i++;
    }
    for (i = 0; i <= arg; i++) {
        args[i] = &argsinstring[argspos[i]];
    }
}

void proccedCommand(char * line) {
    putc('\n');
    char const * args[80] = {"XXXX"};
    parseLine(line, args);
    if (contains(args[0], "echo")) {
        if (args[1] != 0) {
            for (int i = 1; i < 80; i++) {
                if (args[i] != 0) {
                    puts(args[i]);
                    putc(' ');
                }
            }
        } else {
            puts("Nutze ");
            setTextColor(CONSOLE_COLOR_GREEN);
            puts("echo [message]");
        }
    } else if (contains(args[0], "help")) {
        println("---------------------------------[Help]---------------------------------");
        println("- echo [message]                      | Schreibe etwas in die Konsole  -");
        println("---------------------------------[End]----------------------------------");
    } else {
        println("%s ist kein gueltiger Command.", args[0]);
        puts("Probiere ");
        setTextColor(CONSOLE_COLOR_GREEN);
        puts("help");
    }
}

