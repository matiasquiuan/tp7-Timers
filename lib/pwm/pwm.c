#include "stm32f103xb.h"
#include "pwm.h"

void pwm_init(uint8_t canal, uint32_t frec){
    RCC->APB1ENR|=RCC_APB1ENR_TIM3EN; //habilitamos el clock para el periferico tim3 en el registro apb1
   
    if(canal==1){
        RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;//habilitamos el puerto para los pines 
        GPIOA->CRL&=~(0xF<<(6)*4);//configuramos los pines para los canales correspondientes
        GPIOA->CRL|=(0xB<<(6)*4);
        TIM3->CCMR1&=~(7<<4);//configuramos para el canal 1
        TIM3->CCMR1|=(6<<4);
        TIM3->CCER |=TIM_CCER_CC1E;//habilitamos la salida en el canal 1
    } 
    if(canal==2){
        RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;//habilitamos el puerto para los pines
        GPIOA->CRL&=~(0xF<<(7)*4);//configuramos los pines para los canales correspondientes
        GPIOA->CRL|=(0xB<<(7)*4);
        TIM3->CCMR1&=~(7<<12);//configuramos para el canal 2
        TIM3->CCMR1|=(6<<12);
        TIM3->CCER |=TIM_CCER_CC2E;//habilitamos la salida en el canal 2
    } 
    if(canal==3){
        RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;//habilitamos el puerto para los pines
        GPIOB->CRL&=~(0xF<<(0)*4);//configuramos los pines para los canales correspondientes
        GPIOB->CRL|=(0xB<<(0)*4);
        TIM3->CCMR2&=~(7<<4);//configuramos para el canal 3
        TIM3->CCMR2|=(6<<4);
        TIM3->CCER |=TIM_CCER_CC3E;//habilitamos la salida en el canal 3
    }
    if(canal==4){
        RCC->APB2ENR|=RCC_APB2ENR_IOPBEN;//habilitamos el puerto para los pines
        GPIOB->CRL&=~(0xF<<(1)*4);//configuramos los pines para los canales correspondientes
        GPIOB->CRL|=(0xB<<(1)*4);
        TIM3->CCMR2 &=~(7<<12);//configuramos para el canal 4
        TIM3->CCMR2|=(6<<12);
        TIM3->CCER |=TIM_CCER_CC4E;//habilitamos la salida en el canal 4
    }
    TIM3->PSC=7;//configuramos el prescaler a 7 para 1MHZ 8MHZ/(7+1)=1MHZ
    TIM3->ARR=((1000000/frec)-1);//calculo para el arr (1Mhz/frec)-1 lo cual dara la cantidad exacta de "vueltas" del contador
    TIM3->EGR|=TIM_EGR_UG; //aplicar el evento
    TIM3->CR1|=TIM_CR1_CEN; //incio del contador
}

void pwm(uint8_t canal , uint8_t duty){
    if(duty>100) duty=100; //si se pasa del 100
    int crr=((TIM3->ARR+1)*duty)/100;//calculo de crr la (cuentas_cont*duty)/100 lo cual dara la cantidad de cuentas que tiene que hacer el timer para que este prendido (duty)
    if(canal==1) TIM3->CCR1=crr;//le asignamos a el canal correspondiente el crr 
    if(canal==2) TIM3->CCR2=crr;//pasamos del porcentaje del duty a las cuentas del contador en las que estara prendido hasta que llegue a la arr 
    if(canal==3) TIM3->CCR3=crr;
    if(canal==4) TIM3->CCR4=crr;
}