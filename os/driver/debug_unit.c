#define DBGU_BASE 0xFFFFF200

struct dbgu{
    unsigned int cr;
    unsigned int mr;
    unsigned int ier;
    unsigned int idr;
    unsigned int imr;
    unsigned int sr;
    unsigned int rhr;
    unsigned int thr;
};

static volatile struct dbgu * const dbgu = (struct dbgu *)DBGU_BASE;

enum dbgu_status {
 	RXRDY = 1 << 0,
 	TXRDY = 1 << 1,
};

void dbgu_putc(char c)
{
  	do {} while (!(dbgu->sr & TXRDY)); 
 	dbgu->thr = c;
}
 
char dbgu_getc(void)
{
  	do {} while (!(dbgu->sr & RXRDY)); 
 	return dbgu->rhr; 
}

int has_received_char(){
   return dbgu->sr & RXRDY;
}

void dbgu_enable_interrupts(){
    dbgu->ier = 1;
}

void init_dbgu(void){
}
