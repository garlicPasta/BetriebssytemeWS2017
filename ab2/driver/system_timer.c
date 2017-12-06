#define ST_BASE 0xFFFFFD00

struct ST{
 	unsigned int CR;
 	unsigned int PIMR;
 	unsigned int UNUSED[3];
 	unsigned int INTERRUPTED_ENABLE;
};

static volatile struct ST * const st = (struct ST *)ST_BASE; 

void init_time_interrupt(void) {
    st->PIMR = 10;
    st->INTERRUPTED_ENABLE = 1 << 0;
}

void init_system_timer(void){
    init_time_interrupt();
}
