#define ST_BASE 0xFFFFFD00
#define TIMER_DURATION 1<<15 // Value between 0 and 15

struct ST{
 	unsigned int CR;
 	unsigned int PIMR;
 	unsigned int UNUSED[2];
 	unsigned int SR;
 	unsigned int INTERRUPTED_ENABLE;
};

static volatile struct ST * const st = (struct ST *)ST_BASE; 

void init_time_interrupt(void) {
    st->PIMR = TIMER_DURATION;
    st->INTERRUPTED_ENABLE = 1 << 0;
}

void init_system_timer(void){
    init_time_interrupt();
}

int is_timer_done(void){
    return st->SR & 1 << 0;
}

void st_reset_timer(void){
    st->PIMR = TIMER_DURATION;
}
