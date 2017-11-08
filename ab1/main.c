#define DEBUG_REG 0xFFFFF200
#define MAX_STRING_LENGTH 256

#define MODE_DEFAULT 0
#define MODE_SPECIAL 1
#define ASCII_NULL_CHAR 48
#define INT_BYTE_COUNT 4
#include <stdarg.h>


static inline void write_u32 (unsigned int addr, unsigned int    val  ) {
    *(volatile unsigned int   *)addr = val ;
}

static inline void print_c_pointer(const char *c){
    //write_u32(DEBUG_REG,c); // oder komplizierter ?

    volatile unsigned int * const UART0DR = (unsigned int *) DEBUG_REG;
    *UART0DR = (unsigned int)(*c); /* Transmit char */
}

static inline void print_c(char c){
    //write_u32(DEBUG_REG,c); // oder komplizierter ?
    // TODO

    volatile unsigned int * const UART0DR = (unsigned int *)0x101f1000;
    *UART0DR = (unsigned int)(c); /* Transmit char */
}


static inline void print_s(const char *s){
    while(*s != '\0') { /* Loop until end of string */
        print_c(*s);
        s++; /* Next char */
    }
}

static inline void print_x_help(char b){
    char c = '?'; // default

    if(0<= b && b<10){ c = '0' +b;} 	// Zahl [0-9] -> [0-9]
    if(10<= b && b<16){ c = 'A' +b -10 ;}	// Buchstabe [10-15] -> [A-F]
    print_c(c);

}

static inline void print_x(unsigned int x){
    int i; // komischer c standart
    int bendian[] = {0,1,2,3};
    int lendian[] = {3,2,1,0};
    int mendian[] = {2,1,4,3};

    int *endianOrder = bendian; // unterschiedliche Anordnung der Bytes

    char* pre= "0x"; // fancy
    print_s(pre);

    char* p = (char*) &x; // int = 4 bytes .... byte array

    char b= 0;

    for(i=0; i<INT_BYTE_COUNT; i++){
        b = (p[endianOrder[i]]) & 0xF0; 	// nimm forderen 4 Bits
        b = b>>4;		                    // schiebe sie and den anfang(rechts)
        print_x_help(b);
        b = (p[endianOrder[i]]) & 0x0F;	// nimm hintere 4 Bits
        print_x_help(b);
    }


}

static inline void print_p(void* p){
    unsigned int x =(unsigned int) p;
    print_x(x);
}

static inline int count_params_from_format_string(const char* s){
    int count =0;
    while(*s != '\0') {
        if(*s == '%'){count++;}
        s++;
    }
    return count;
}


void my_print_f(const char *s,...) {
    int mode = MODE_DEFAULT ;
    // initialisieren der dyn Argument liste
    va_list args;
    int num_args = count_params_from_format_string(s); // N der Argumente notwendig
    va_start(args,s); // nun kann va_arg benutzt werden

    // Format String durchlaufen und gegebenfalls Argumente einbinden
    while(*s != '\0') { // Null terminierte Strings
        if(*s == '%'){
            s++; // falls dann muss nun ein Zeichen fÃ¼r das Format kommen
            switch((*s)){
                // Nutze entsprechende Print funktion
                case  'c': 	print_c((char)va_arg(args,int)); 			break;
                case  's':	print_s(va_arg(args,const char*)); 	break;
                case  'x':	print_x(va_arg(args,unsigned int)); 	break;
                case  'p':	print_p(va_arg(args,void*)); 			break;
                case '\0':	//print_c(&'%');
                    continue; // Error im naechsten schritt wird Print beendet
                default:
                    // war einfach nur ein Prozent Zeichen
                    //print_c('%');
                    print_c(*s);
            }
        }else{
            print_c(*s);
        }
        s++;// pointer auf das naechste Zeichen setzen
    }


}

void c_entry() {
    // test

    //my_print_f("Hallo %s, das ist ein %s \nZahl: %x","Welt","Test!",1023);
    char x = 'c';
    unsigned int i ;
    for(i=0;i<32;i++){
        my_print_f("Zahl: %x\n",i);

    }
    my_print_f("Pointer: %p\n",&x);
    my_print_f("Char %c, String %s,Hex %x, Pointer %p\n",x,"Hello",0x101,&x);
}


