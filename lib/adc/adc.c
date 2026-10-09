#include "stm32f103xb.h"
#include "adc.h"

void adc_init(){
    RCC->APB2ENR|=RCC_APB2ENR_ADC1EN|RCC_APB2ENR_IOPAEN|RCC_APB2ENR_IOPBEN;//paso 1 habilitar los clocks
    ADC1->CR2|=ADC_CR2_ADON;//paso 4 encender el ADC
    for(int i=0;i<1000;i++);//esperar
    ADC1->CR2|=ADC_CR2_RSTCAL;//paso 4 reiniciar los registros para la calibracion
    while(ADC1-> CR2 & ADC_CR2_RSTCAL);//paso 4 esperar a que se reinicien 
    ADC1->CR2|=ADC_CR2_CAL;//paso 4 inicia la calibracion    
    while(ADC1 -> CR2 & ADC_CR2_CAL);//esperamos a que cambie a 0 para que inidique que ya termino la calibracion
    ADC1->CR2|=ADC_CR2_EXTSEL;//paso 6 definir el disparo de la conversion
    ADC1->CR2|=ADC_CR2_EXTTRIG;//paso 6 definir el disparo de la conversion

    
} 

int adc_read(unsigned int canal){
    if(canal<=7){//paso 3 configurar pines como entradas analogicas
        GPIOA->CRL&=~(0xF<<canal*4);//para puerto a si es el canal es menor igual a 7
    }
    else if(canal<=9){
        GPIOB->CRL&=~(0xF<<(canal%2)*4);//para puerto b si es el canal es menor igual a 9
    }
    ADC1->SQR3|=canal;//paso 5 seleccionar el canal
    ADC1->SMPR2=(0B111<<canal*3);//paso 6 elegir el tiempo de muestreo
    ADC1->CR2|=ADC_CR2_SWSTART;//paso 7 inicia la conversion
    while(!(ADC1->SR&ADC_SR_EOC));//paso 8 esperar la EOC
    return ADC1->DR;//paso 9 devolver el DR
}