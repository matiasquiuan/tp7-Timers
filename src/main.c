#include "stm32f103xb.h"
#include "adc.h"
#include "pwm.h"
int pote=0;
int conv=0;
int duty=0;
int main(){
    RCC->APB2ENR|=RCC_APB2ENR_IOPAEN;
    adc_init();//iniciamos el adc
    pwm_init(1, 1000);//inciamos el pwm en el canal 1 con un f=1khz
    while(1){
        conv=adc_read(pote);//pasamos a digital la tension analogica del pote
        duty=(conv*100)/4095;//calculamos el duty
        pwm(1,duty);
    }

}