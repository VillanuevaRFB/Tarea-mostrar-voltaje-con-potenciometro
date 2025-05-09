#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

volatile unsigned char canal=2;

ISR(INT0_vect){
    if (canal==2)
        canal=1;
    else
        canal=2;
    ADMUX=(1<<REFS0)|(1<<ADLAR)|canal;
}

int main(void){
    DDRD=0xF0;//displays
    DDRB=0x03;//activar/desactivar
    DDRD&=~0x04;
    PORTD|=0x04;

    ADMUX=(1<<REFS0)|(1<<ADLAR)|(canal);
    ADCSRA=(1<<ADEN)|(1<<ADPS2);
    
    EIMSK|=(1<<INT0);
    EICRA|=(1<<ISC01);
    sei();
    while(1){
        ADCSRA|=(1<<ADSC);
        while(!(ADCSRA&(1<<ADIF)));
        ADCSRA|=(1<<ADIF);

        unsigned char ahigh=ADCH;
        double voltaje=(ahigh*5.0)/255.0;
        unsigned char unidad=(unsigned char)voltaje;
        unsigned char decimal=(unsigned char)((voltaje-unidad)*10);
        PORTB=0x01;
        PORTD&=0x0F;
        PORTD|=(unidad<<4);
        _delay_ms(5);
        PORTB=0x02;
        PORTD&=0x0F;
        PORTD|=(decimal<<4);
        _delay_ms(5);
    }
}