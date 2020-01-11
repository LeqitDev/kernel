#include "include/console.h"
#include "include/InterruptDescriptorTable.h"
#include <stdarg.h>

struct __attribute__((packed)) VideoChar { char c; char color; }; //Videochar struct um text einfacher zum *wirklichen* Videochar hinzuzufügen

struct VideoChar * video = (struct VideoChar*) 0xb8000; //Der video speicher
int backgroundColor = CONSOLE_COLOR_BLACK; //setzten der (generellen) Hintergrundfarbe
int textColor = CONSOLE_COLOR_WHITE; //setzten der (generellen) Textfarbe
int x = 0; //x-position
int y = 0; //y-position

typedef struct
{
    char * text;
    size_t len;
} StringPutBuffer;

static void stringPutc(void * ctx, char c)
{
    StringPutBuffer * buf = (StringPutBuffer *)ctx;
    buf->text[buf->len] = c;
    buf->len += 1;
    buf->text[buf->len] = 0;
}

int getColor() {
    return backgroundColor * 16 + textColor; //Die Hintergrundfarbe und Textfarbe in einen colorcode umrechnen
}

void setBgColor(int hexColorCode) {
    backgroundColor = hexColorCode;
}

void setTextColor(int hexColorCode) {
    textColor = hexColorCode;
}

void clearScreen(void)
{
    //Den Bildschirm mit null überschreiben
	struct VideoChar vchar;
	vchar.c = 0;
	vchar.color = 0x00;
	for(int i = 0; i <= 2000; i++)
	{
		video[i] = vchar;
	}
}

void displaycursor(int col, int row)
{
    int tmp;
    tmp = row * 80 + col;
    outb(0x3D4,14);
    outb(0x3D5,tmp >> 8);
    outb(0x3D4,15);
    outb(0x3D5,tmp);
}

void puts(const char * string)
{
    //String schreiben
	for (int i = 0; string[i] != 0; i++)
	{
	    //Abfragen nach Backslash-Codes
	    if (string[i] == '\n') {
            i++;
            newLine();
	    } else if (string[i] == '\b') {
	        //TODO: zum nächsten text springen
	        i++;
            if (x == 0 && y > 0) {
                x = 79;
                y--;
            } else if (x > 0) {
                x--;
            }
	        while (video[x + 80 * y].c == 0 && y >= 0) {
                if (x == 0 && y > 0) {
                    x = 79;
                    y--;
                } else if (x > 0) {
                    x--;
                }
	        }
	        struct VideoChar vchar;
	        vchar.c = 0;
	        vchar.color = 0x00;

	        video[x + 80 * y] = vchar;
	    } else if (string[i] == '\t') {
            struct VideoChar vchar;
            vchar.c = ' ';
            vchar.color = 0x00;
            for (int i = 0; i < 5 && x < 80; ++i) {
                video[x + 80 * y] = vchar;
                x++;
            }
	    } else {
	        //sonst normale Ausgabe
            struct VideoChar vchar;
            vchar.c = string[i]; //Char setzten
            vchar.color = getColor(); //Farbe des char hintergrundes und des chars an sich setzten

            video[x + 80 * y] = vchar; //25 Zeilen 80 Spalten. y++ cursor +80 = neue Zeile

            x++; //X erhöhen
            if (x >= 80) newLine(); //Wenn x größer als die max. Spalten dann neue Zeile
	    }
	}
	displaycursor(x, y);
	if (y == 24 || x / 80 >= 24) //Wenn ganz unten angekommen muss nach oben gescrollt werden.
		scroll();
}

void putc(char c)
{
    //Char schreiben & direkt nach Backslash-Codes abfragen da nicht mehrere chars vorhanden sind
    if (c == '\n') {
        y++;
        x = 0;
    } else if (c == '\b') {
        if (x == 0 && y > 0) {
            x = 79;
            y--;
        } else if (x > 0) {
            x--;
        }
        while (video[x + 80 * y].c == 0 && y >= 0) {
            if (x == 0 && y > 0) {
                x = 79;
                y--;
            } else if (x > 0) {
                x--;
            }
        }
        struct VideoChar vchar;
        vchar.c = 0;
        vchar.color = 0x00;

        video[x + 80 * y] = vchar;
    } else if (c == '\t') {
        struct VideoChar vchar;
        vchar.c = ' ';
        vchar.color = 0x00;
        for (int i = 0; i < 5 && x < 80; ++i) {
            video[x + 80 * y] = vchar;
            x++;
        }
    } else {
        //ansonsten einen char ausgeben
        struct VideoChar vchar;
        vchar.c = c;
        vchar.color = getColor();

        video[x + 80 * y] = vchar; //Char in den Videospeicher abspeichern

        x++; //X erhöhen für den nächsten char
    }
    displaycursor(x, y);
    if (y == 24 || x / 80 >= 24) //Wenn ganz unten angekommen muss nach oben gescrollt werden.
        scroll();
}

void newLine()
{
    y++; //Y erhöhen für nächste zeile
    x = 0; //X auf 0 setzten damit man an dem linien anfang anfängt zu schreiben
    displaycursor(x, y);
}

void scroll(void)
{
	for (int y = 0; y < 24; y++)
	{
		for (int x = 0; x < 80; x++)
		{
			video[x + 80 * y] = video[x + 80 * (y + 1)]; // alles eine zeile höher schieben
		}
	}
	x = 0; //X wieder auf zeilen anfang setzten
	y--; //Y von 26 (außerhalb des Bildschirms) auf 25 setzten (neue zeile da alle zeilen auf eine darüber gesetzt wurden)
	displaycursor(x, y);
}

void getLine(char * line, int row) {
    int col = 0;
    for (col = 0; col < 80; col++) {
        line[col] = video[col + 80 * row].c;
    }
    line[col] = '\0';
}

int strlen(char * string) {
    int length = 0;
    while (*string) {
        string++;
        length++;
    }
    return length;
}

int contains(char * string, char * searchkey) {
    int character = 0;
    while (*string) {
        if (*string != 0 && character < strlen(searchkey)) {
            if (*string == searchkey[character]) {
                character++;
            }
            string++;
        }
    }
    if (character == strlen(searchkey)) return character;
    else return 0;
}

int charinstring(char * string, char character) {
    int position = 0;
    while (*string) {
        if (*string != 0) {
            if (*string == character) {
                return position;
            }
            string++;
            position++;
        }
    }
    return 0;
}

int unsignedToString(char * buffer, size_t length, unsigned long long num, int base)
{
    //unsigned funktion: Minuswerte nicht mitinbegriffen: nur positive Werte
    if (base < 2 && base > 16)
    {
        //wenn base kleiner als binär oder größer als hexadecimal ist wird die funktion direkt abgebrochen
        return -1;
    }

    int len = 0;
    const char * Digits = "0123456789ABCDEF"; //von int zu einem char z.B. Digits[4] = '4';

    unsigned long long helper = num;

    for (; helper > 0; len++)
    {
        //lenge des long long herausfinden
        helper /= base;
    }

    if (len+1 > (int) length)
    {
        //*echte* länge mit der angegebenen Länge vergleichen wenn *echte* länge größer funktion abbrechen
        return -1;
    }

    if(num == 0)
    {
        //wenn nummer == 0
        buffer[0] = Digits[0]; //(int) 0 zu (char) 0 konvertieren
        len++; //lenge erhöhen da eine zahl ja auch die länge 1 hat
        buffer[len] = '\0'; //ein string hört immer mit dem char '\0' auf!
        return len; //länge zurückgeben und aus dieser funktion herausgehen
    }

    for (len = 0; num > 0; len++)
    {
        // eigentliche long long to string wandlung
        buffer[len] = Digits[num % base];
        num /= base;
    }

    //reverse
    for (int x = 0; x < len/2; x++)
    {
        //Den string drehen bisher z.B. 004 danach sollte dann 400 herauskommen
        char temp = buffer[x]; //temporärer speicher für die drehung
        buffer[x] = buffer[len-x-1]; //zahl tauschen
        buffer[len-x-1] = temp; //getauschte zahl mit der aus dem temp. speicher tauschen
    }

    buffer[len] = '\0'; //endchar setzten
    return len; //länge zurückgeben und aus der funktion herausgehen
}

int signedToString(char * buffer, size_t length, signed long long num, int base)
{
    //signed werte: Minuswerte mitinbegriffen: negative Werte und positive
    int neg = 0;
    if (num < 0) {buffer[0] = '-'; num = -num; buffer++; neg++;} //abfragen ob positiv wenn ja nummer positivieren und ein minus zeichen vorne dranhängen
    int len = unsignedToString(buffer, length, num, base); //nur den buffer nach dem minus zeichen übergeben: buffer++;
    len += neg; //wenn es negativ war += 1 ansonsten += 0
    return len; //länge zurückgeben und aus der funktion herausgehen
}

int genprintf(void (*put)(void * ctx, char c), void * ctx, char const * fmt, va_list ArgList)
{
    //generelle Formatierungsfunktion
    int len = 0;
    while (*fmt)
    {
        //für alle chars im char pointer
        len++; //länge erhöhen
        if (*fmt == '%')
        {
            //wenn es ein %-Zeichen (Aktivierungscode) gibt
            fmt++; //%-Zeichen nicht ausgeben deswegen fmt++
            if (*fmt == 'i')
            {
                //wenn ein %i (int) gefordert ist
                fmt++; //'i'-Zeichen überspringen
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"; //char array füllen für ausgabe
                int length;
                if (*fmt == 'u')
                {
                    //wenn ein %iu (unsigned int) gefordert ist
                    fmt++; //'u' überspringen
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, unsigned int), 10); //unsigned int zu einem string umwandeln, länge speichern
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, int), 10); //int zu string umwandeln, länge speichern
                }
                len += length - 1; //länge des generierten strings zu der eigentlichen länge hinzurechnen
                for (int i = 0; i < length; i++) {
                    //den generierten string ausgeben
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'l')
            {
                //wenn ein %l (long) gefordert ist (siehe %i)
                fmt++;
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
                int length;
                if (*fmt == 'u')
                {
                    //wenn ein %lu (unsigned long) gefordert ist
                    fmt++;
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, long), 10);
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, long), 10);
                }
                len += length - 1;
                for (int i = 0; i < length; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'L')
            {
                //wenn ein %L (long long) gefordert ist (siehe %i)
                fmt++;
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
                int length;
                if (*fmt == 'u')
                {
                    //wenn ein %Lu (unsigned long long) gefordert ist
                    fmt++;
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, unsigned long long), 10);
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, long long), 10);
                }
                len += length - 1;
                for (int i = 0; i < length; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'b')
            {
                //wenn ein %b (int to binary) gefordert ist (siehe %i)
                fmt++;
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
                int length;
                if (*fmt == 'u')
                {
                    //wenn ein %bu (unsigned int to binary) gefordert ist
                    fmt++;
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, unsigned int), 2);
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, int), 2);
                }
                len += length - 1;
                for (int i = 0; i < length; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'o')
            {
                //wenn ein %o (int to octal) gefordert ist (siehe %i)
                fmt++;
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
                int length;
                if (*fmt == 'u')
                {
                    //wenn ein %oi (unsigned int to octal) gefordert ist
                    fmt++;
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, unsigned int), 8);
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, int), 8);
                }
                len += length - 1;
                for (int i = 0; i < length; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'h')
            {
                //wenn ein %h (int to hexadecimal) gefordert ist (siehe %i)
                fmt++;
                char Buff[64] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";
                int length;
                if (*fmt == 'u')
                {
                    //wenn ein %hu (unsigned int to hexadecimal) gefordert ist
                    fmt++;
                    length = unsignedToString(Buff, sizeof(Buff), va_arg(ArgList, unsigned int), 16);
                }
                else
                {
                    length = signedToString(Buff, sizeof(Buff), va_arg(ArgList, int), 16);
                }
                len += length - 1;
                if (put != NULL) put(ctx, '0');
                if (put != NULL) put(ctx, 'x');
                for (int i = 0; i < length; i++) {
                    if (put != NULL) put(ctx, Buff[i]);
                }
            }
            else if (*fmt == 'c')
            {
                //wenn ein %c (char) gefordert ist (siehe %s)
                fmt++;
                char c = va_arg(ArgList, int);
                if (put != NULL) put(ctx, c);
                len++;
                put(ctx, '\0');
            }
            else if (*fmt == 's')
            {
                //wenn ein %s (string) gefordert ist
                fmt++; //'s' überspringen
                int i = 0;
                char const * Buff = va_arg(ArgList, char const *); //den neuen angegebenen string speichern
                for (; Buff[i] != 0; i++) {
                    //neuen string ausgeben
                    if (put != NULL) put(ctx, Buff[i]);
                }
                len += i - 1; //länge des neuen strings zu normalen länge hinzufügen
                put(ctx, '\0'); //string-end-code setzten
            }
        }
        else
        {
            //den eigentlichen string ausgeben
            if (put != NULL) put(ctx, *fmt);
            fmt++;
        }
    }
    return len; //die länge des gesamten string zurückgeben und aus der funktion herausgehen
}

void kprintf(void (*put)(void * ctx, char c), void * ctx, char const * fmt, ...)
{
    //kaprintf
    va_list vaList;
    va_start(vaList, fmt);
    genprintf(put, ctx, fmt, vaList);
    va_end(vaList);
}

void printf(char * buffer, char const * fmt, ...) {
    //kprintf ohne void (*put)
    va_list vaList;
    va_start(vaList, fmt);
    StringPutBuffer writer = { buffer, 0 };
    genprintf(stringPutc, &writer, fmt, vaList);
    va_end(vaList);
}
