#define DEBUG_REG 0xFFFFF200
#define REMAP_REG 0xFFFFFF00 
#define WRITE_OFF 28
#define STATUS_OFF 20
#define RECEIVE_OFF 24
#define INT_BYTE_COUNT 4

#define IVT_RESET 	0x00000000 // RESET
#define IVT_UNDEFINED   0x00000004 // UNDEFINED INSTRUCTION
#define IVT_SOFTWARE	0x00000008 // SOFTWARE INTERRUPT
#define IVT_PREFETCH 	0x0000000C // PREFETCH ABORT
#define IVT_DATA	0x00000010 // DATA ABORT
#define IVT_IRQ		0x00000018 // IRQ
#define IVT_FIQ 	0x0000001C // FIQ evtl direkt behandlen danach ist

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



void c_handler(){
	my_print_f("An Exception happend: %s","???");	
}

void c_entry() {
	/*
    for (;;) {
        if (*STATUS_ADDR & 0x01) {                           // pruefe ob es eine neue Eingabe gab
            my_print_f("-> You pressed: %c \n", RECEIV_ADDR[0]);
        }
    }
	*/

	// Speicher Remap S 125
	// write 1 (oder 0?) nach 0xFFF FFF F00
	// for memory remap
	*REMAP_REG = 1;

	

	// IVT eintragen
	// sprungbefehle (b/bl) aus Assmbler in bytes desasmblieren..


	*IVT_RESET  = ... 	// RESET
     	*IVT_UNDEFINED =	// UNDEFINED INSTRUCTION
	*IVT_SOFTWARE = 	// SOFTWARE INTERRUPT
	*IVT_PREFETCH = 	// PREFETCH ABORT
	*IVT_DATA = 	 	// DATA ABORT
	*IVT_IRQ = 	 	// IRQ
	*IVT_FIQ = 	 	// FIQ evtl direkt behandlen danach ist speicher frei

	// An den Ziel Operationen muss assmbler trampolin erstellt werden bevor
	// normaler C code ausgeführt werden kann

}














