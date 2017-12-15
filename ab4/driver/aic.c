#define AIC_BASE 0xFFFFF000

struct AIC{
 	unsigned int SMR[32];
 	void (*SVR[32])(void);
 	unsigned int IVR;
 	unsigned int UNUSED[7];
 	unsigned int IECR;
 	unsigned int IDCR;
 	unsigned int ICCR;
 	unsigned int ISCR;
 	unsigned int EOICR;
};

static volatile struct AIC * const aic = (struct AIC *)AIC_BASE;

void init_aic(void){
    aic->IDCR = 1 << 1;
    aic->IECR = 1 << 1;
    //aic->ISCR = 7;

    // aic->SVR[1] = handle_timer;
    //aic->SMR[1] = 7;
    aic->ICCR = 1 << 1;
}

void acknowledge_interrupt(void){
    aic->IVR;
}

void aic_clear_interrupt(int n){
    //aic->ICCR = 1 << n;
    aic->EOICR = 1;
    aic->ICCR = 1 << n;
    //aic->ISCR = 1 << n;
}
