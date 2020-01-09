#include "include/InterruptDescriptorTable.h"
#include "include/KBC.h"
#include "include/commands.h"
#include "include/console.h"
#include <stdint.h>

//TODO: Kommentare vervollständigen

static void send_command(uint8_t command);
void init_keyboard(void) {
    while (inb(0x64) & 0x1) {
        inb(0x60);
    }

    send_command(0xF4);
}

static void send_command(uint8_t command) {
    while ((inb(0x64) & 0x2)) {}
    outb(0x60, command);
}



int normal_kc_array[] = {
        0, 0, //Scancode 0-1: 0, Escape
        49, 50, 51, 52, 53, 54, 55, 56, 57, 48, 45, 43, //Scancode 2-13: 1[!], 2[", ²], 3[§, ³], 4[$], 5[%], 6[&], 7[/, {], 8[(, [], 9[), ]], 0[=, }], -[_], +[*, ~]
        0, 0, //Scancode 14-15: Backspace, Cpaslock
        113, 119, 101, 114, 116, 121, 117, 105, 111, 112, 225, //Scancode 16-26: q[Q, @], w[W], e[E, €], r[R], t[T], y[Y], u[U], i[I], o[O], p[P], ß[?, \]
        0, 0, 0, //Scancode 27-29: 0, Enter, Strg
        97, 115 ,100, 102, 103, 104, 106, 107, 108, 129, 132, 148, //Scancode 30-41: a[A], s[S], d[D], f[F], g[G], h[H], j[J], k[K], l[l], ü[Ü], ä[Ä], ö[Ö]
        0, //Scancode 42: Shift
        94, 122, 120, 99, 118, 98, 110, 109, 44, 46, 35 //Scancode 43-53: ^[°], z[Z], x[X], c[C], v[V], b[B], n[N], m[M, µ], ,[;], .[:], #[']
};

int shift_kc_array[] = {
        0, 0, //Scancode 0-1: 0, Escape
        33, 34, 245, 36, 37, 38, 47, 40, 41, 61, 95, 42, //Scancode 2-13: 1[!], 2[", ²], 3[§, ³], 4[$], 5[%], 6[&], 7[/, {], 8[(, [], 9[), ]], 0[=, }], -[_], +[*, ~]
        0, 0, //Scancode 14-15: Backspace, Cpaslock
        81, 87, 69, 82, 84, 89, 85, 73, 79, 80, 63, //Scancode 16-26: q[Q, @], w[W], e[E, €], r[R], t[T], y[Y], u[U], i[I], o[O], p[P], ß[?, \]
        0, 0, 0, //Scancode 27-29: 0, Enter, Strg
        65, 83, 68, 70, 71, 72, 74, 75, 76, 154, 142, 153, //Scancode 30-41: a[A], s[S], d[D], f[F], g[G], h[H], j[J], k[K], l[l], ü[Ü], ä[Ä], ö[Ö]
        0, //Scancode 42: Shift
        248, 90, 88, 67, 86, 66, 78, 77, 59, 58, 39 //Scancode 43-53: ^[°], z[Z], x[X], c[C], v[V], b[B], n[N], m[M, µ], ,[;], .[:], #[']
};

int altgr_kc_array[] = {
        0, 0, //Scancode 0-1: 0, Escape
        0, 253, 252, 0, 0, 0, 123, 91, 93, 125, 0, 126, //Scancode 2-13: 1[!], 2[", ²], 3[§, ³], 4[$], 5[%], 6[&], 7[/, {], 8[(, [], 9[), ]], 0[=, }], -[_], +[*, ~]
        0, 0, //Scancode 14-15: Backspace, Cpaslock
        64, 0, 128, 0, 0, 0, 0, 0, 0, 0, 92, //Scancode 16-26: q[Q, @], w[W], e[E, €], r[R], t[T], y[Y], u[U], i[I], o[O], p[P], ß[?, \]
        0, 0, 0, //Scancode 27-29: 0, Enter, Strg
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, //Scancode 30-41: a[A], s[S], d[D], f[F], g[G], h[H], j[J], k[K], l[l], ü[Ü], ä[Ä], ö[Ö]
        0, //Scancode 42: Shift
        0, 0, 0, 0, 0, 0, 0, 230, 0, 0, 0 //Scancode 43-53: ^[°], z[Z], x[X], c[C], v[V], b[B], n[N], m[M, µ], ,[;], .[:], #[']
};

static int * sc_to_kc = normal_kc_array;

uint8_t translate_scancode(int set, uint16_t scancode, int breakcode) {

    if (set == 0) {
        //Kein e0/e1 scancode
        if (scancode == 14) {
            //Backspace
            return 8;
        } else if (scancode == 15) {
            //Tab
            return 9;
        } else if (scancode == 28) {
            //Enter
            return 13;
        } else if (scancode == 42 || scancode == 54){
            //Left-/Rightshift
            if (sc_to_kc == normal_kc_array) sc_to_kc = shift_kc_array;
            else sc_to_kc = normal_kc_array;
            return 0;
        } else if (scancode == 57) {
            //Space
            return 32;
        } else if (scancode == 58) {
            //CapsLock
            if (sc_to_kc == normal_kc_array && !breakcode) sc_to_kc = shift_kc_array;
            else if (sc_to_kc == shift_kc_array && !breakcode) sc_to_kc = normal_kc_array;
            return 0;
        } else if ((scancode >= 0x2 && scancode <= 0xD) ||
                   (scancode >= 0x10 && scancode <= 0x1A) ||
                   (scancode >= 0x14 && scancode <= 0x29) ||
                   (scancode >= 0x2B && scancode <= 0x35)) {
            //wenn es normale buchstaben sind
            return sc_to_kc[scancode];
        } else {
            return scancode;
        }
    } else if (set == 1) {
        //e0 scancode
        return scancode;
    } else if (set == 2) {
        //e1 scancode
        return scancode;
    }
    return -1;
}

char handle_keys(int keycode) {
    char character = 0;
    if (keycode >= 32 && keycode <= 255) character = keycode;
    else if (keycode == KEY_CODE_TABULATOR) character = '\t';
    else if (keycode == KEY_CODE_ENTER) {
        proccedCommand(getLine(y));
        character = '\n';
    }
    else if (keycode == KEY_CODE_BACKSPACE) character = '\b';

    return character;
}