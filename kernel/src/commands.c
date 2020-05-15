#include "console.h"
#include "commands.h"

void parseLine(const char * line, char * args[]) {
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

int argsLen(char * args[], int maxlength) {
    int len = 0;
    for (int i = 0; i < maxlength; i++) {
        if (args[i] != 0) len++;
    }
    return len;
}

void proceedCommand(char * line) {
    putc('\n');
    char * args[80] = {"XXXX"};
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
            print("Nutze ");
            setTextColor(CONSOLE_COLOR_GREEN);
            print("echo [message]");
        }
        putc('\n');
    } else if (contains(args[0], "help")) {
        println("---------------------------------[Help]---------------------------------");
        println("- echo [message]                      | Schreibe etwas in die Konsole  -");
        println("---------------------------------[End]----------------------------------");
    } else if (contains(args[0], "add")) {
        int x = strtoint(args[1]);
        int y = strtoint(args[2]);
        if (args[3] != 0) println("%s %i", args[3], x + y);
        else println("%i", x + y);
    } else if (contains(args[0], "sub")) {
        int x = strtoint(args[1]);
        int y = strtoint(args[2]);
        if (args[3] != 0) println("%s %i", args[3], x - y);
        else println("%i", x - y);
    } else if (contains(args[0], "mul")) {
        int x = strtoint(args[1]);
        int y = strtoint(args[2]);
        if (args[3] != 0) println("%s %i", args[3], x * y);
        else println("%i", x * y);
    } else if (contains(args[0], "div")) {
        int x = strtoint(args[1]);
        int y = strtoint(args[2]);
        if (args[3] != 0) println("%s %i", args[3], x / y);
        else println("%i", x / y);
    } else if (contains(args[0], "if")) {
        if (contains(args[2], "equals")) {
            int i = 3;
            if (contains(args[1], args[3])) {
                while (!contains(args[i++], "else") && args[i++] != 0) {
                    print(args[i]);
                }
            } else {
                while (contains(args[i++], "else") && args[i++] != 0) ;
                i++;
                while (args[++i] != 0) {
                    println("%i", i);
                    print(args[i]);
                }
            }
        }
    } else {
        println("%s ist kein gueltiger Command.", args[0]);
        puts("Probiere ");
        setTextColor(CONSOLE_COLOR_GREEN);
        puts("help");
    }
}

