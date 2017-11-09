#define DEBUG_REG 0xFFFFF200
#define WRITE_OFF 28
#define STATUS_OFF 20
#define RECEIVE_OFF 24
#define INT_BYTE_COUNT 4

#include <stdarg.h>


volatile char *const TRANS_ADDR = (char *) DEBUG_REG + WRITE_OFF;
volatile char *const STATUS_ADDR = (char *) DEBUG_REG + STATUS_OFF;
volatile char *const RECEIV_ADDR = (char *) DEBUG_REG + RECEIVE_OFF;

static inline void print_c(char c) {
    *TRANS_ADDR = c;                        // schreibe char
}


static inline void print_s(const char *s) {
    while (*s != '\0') {                    // Iteriere durch den String
        print_c(*s);
        s++;                                // naechster Buchstabe
    }
}

static inline void print_x_help(char b) {
    char c = '?';                           // default

    if (b < 10) {
        c = (char) ('0' + b);                        // Zahl [0-9] -> [0-9]
    }
    if (10 <= b && b < 16) {
        c = (char) ('A' + b - 10);                   // Buchstabe [10-15] -> [A-F]
    }
    print_c(c);
}

static inline void print_x(unsigned int x) {
    int i;
    int bendian[] = {0, 1, 2, 3};

    int *endianOrder = bendian;             // unterschiedliche Anordnung der Bytes
    char *pre = "0x";
    print_s(pre);

    char *p = (char *) &x;                  // int = 4 bytes .... byte array
    char b = 0;

    for (i = 0; i < INT_BYTE_COUNT; i++) {
        b = (char) ((p[endianOrder[i]]) & 0xF0);        // nimm forderen 4 Bits
        b = b >> 4;                            // schiebe sie and den anfang(rechts)
        print_x_help(b);
        b = (char) ((p[endianOrder[i]]) & 0x0F);        // nimm hintere 4 Bits
        print_x_help(b);
    }
}

static inline void print_p(void *p) {
    unsigned int x = (unsigned int) p;
    print_x(x);
}

void my_print_f(const char *format_string, ...) {
    va_list args;
    va_start(args, format_string);                      // nun kann va_arg benutzt werden
                                                        // Format String durchlaufen und gegebenfalls Argumente einbinden
    while (*format_string != '\0') {                    // Null terminierte Strings
        if (*format_string == '%') {
            format_string++;                            // falls dann muss nun ein Zeichen fÃ¼r das Format kommen
            switch ((*format_string)) {                 // Nutze entsprechende Print funktion
                case 'c':
                    print_c((char) va_arg(args, int));
                    break;
                case 's':
                    print_s(va_arg(args, const char*));
                    break;
                case 'x':
                    print_x(va_arg(args, unsigned int));
                    break;
                case 'p':
                    print_p(va_arg(args, void*));
                    break;
                case '\0':
                    continue;                               // Error im naechsten schritt wird Print beendet
                default:
                    print_c(*format_string);                // war einfach nur ein Prozent Zeichen
            }
        } else {
            print_c(*format_string);
        }
        format_string++;                                    //pointer auf das naechste Zeichen setzen
    }
}

void c_entry() {
    my_print_f("Read Status Address : %p", )
    for (;;) {
        if (*STATUS_ADDR & 0x01) {                           // pruefe ob es eine neue Eingabe gab
            my_print_f("-> You pressed: %c \n", RECEIV_ADDR[0]);
        }
    }
}

