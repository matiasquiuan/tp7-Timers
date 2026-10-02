#include "stm32f103xb.h"
#include "timer.h"

int timer_overflow;

void timer_init(){
    RCC->APB1ENR|=RCC_APB1ENR_TIM2EN; // paso 1 habilitamos clock del timer 
    TIM2->CR1&=~TIM_CR1_CEN; //paso 2 detenemos el contador
    TIM2->PSC=7; //PASO 3 PSC PARA 1MHZ 8MHZ/(7+1)=1MHZ
    TIM2->ARR=0xFFFF;//paso 4 configuramos arr con el valor maximo de 16 bits
    TIM2->CNT=0; //PASO 5 REINICIO CNT 
    TIM2->DIER|=TIM_DIER_UIE; //PASO HABILITAMOS INTERRUPCION
    TIM2->EGR|=TIM_EGR_UG;//PASO 7 GENERAMOS UG (ACTUALIZACION)
    TIM2->SR&=~TIM_SR_UIF; //PASO 7 LIMPIAMOS LAS FLAGS
    NVIC_EnableIRQ(TIM2_IRQn);//paso 8 habilitar irq en nvic
    TIM2->CR1|=TIM_CR1_CEN; //paso 9 arranca el contador
}

void delay_init(){
    RCC->APB1ENR|=RCC_APB1ENR_TIM2EN; // paso 1 habilitamos clock del timer 
    TIM2->CR1&=~TIM_CR1_CEN; //paso 2 detenemos el contador
    TIM2->PSC=7; //PASO 3 PSC PARA 1MHZ 8MHZ/(7+1)=1MHZ
    TIM2->ARR=0xFFFF;//paso 4 configuramos arr con el valor maximo de 16 bits
    TIM2->CNT=0; //PASO 5 REINICIO CNT 
    TIM2->EGR|=TIM_EGR_UG;//PASO 7 GENERAMOS UG (ACTUALIZACION)
    TIM2->SR&=~TIM_SR_UIF; //PASO 7 LIMPIAMOS LAS FLAGS
    TIM2->CR1|=TIM_CR1_CEN; //paso 9 arranca el contador
}

void delay_us(uint32_t us){
    TIM2->CR1|=TIM_CR1_CEN;//prendemos el contador
    TIM2->CNT=0; //lo ponemos en 0
    while(TIM2->CNT>us){ //si el tiempo pedido ya paso
        TIM2->CNT=0;//lo ponemos en 0
        TIM2->CR1&=~TIM_CR1_CEN;//lo apagamos
    }
}

void delay_ms(uint32_t ms){
    for(int i=0;i<ms;i++){//repetimos ms veces 1000 us o 1ms
        delay_us(1000);
    }
}

uint32_t timer_millis(){
    (timer_overflow + TIM2->CNT)/1000;//sumamos los overflows con el tiempo de cnt actual
    return;
}

void TIM2_IRQHandler(){
    if(TIM2->SR&=~TIM_SR_UIF){//preguntamos si se produjo un overflow
        timer_overflow=+0xFFFF; //si la hubo sumamos los 16bits 
    }
}

void pwm_init(uint8_t canal, uint32_t frec){
    switch(canal){
        case 1:
            RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
            RCC->APB1ENR|=RCC_APB1ENR_TIM3EN;
            GPIOA->CRL&=~(0xF<<(TIM3_CH1)*4);
        break;
    }
}