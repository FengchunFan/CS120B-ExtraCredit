#include <avr/io.h>
#include "io.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

//create three states
enum ThreeLEDsSM {start_three, led_0, led_1, led_2} ThreeLEDsstate;
unsigned char threeLEDs = 0x00;

enum BlinkingLEDSM {start_blinking, blink_on, blink_off} BlinkingLEDstate;
unsigned char blinkingLED = 0x00;

enum CombineLEDsSM {start_combine, Init} CombineLEDstate;

volatile unsigned char TimerFlag = 0;
unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){
TCCR1B = 0x0B;
OCR1A = 125;
TIMSK1 = 0x02;
TCNT1 = 0;
_avr_timer_cntcurr = _avr_timer_M;
SREG |= 0x80;
}


void TimerOff(){
TCCR1B = 0x00;
}

void TimerISR(){
TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
_avr_timer_cntcurr--;
if(_avr_timer_cntcurr == 0){
        TimerISR();
        _avr_timer_cntcurr = _avr_timer_M;
}
}

void TimerSet(unsigned long M){
_avr_timer_M = M;
_avr_timer_cntcurr = _avr_timer_M;
}

void Tick_Three(){
        switch(ThreeLEDsstate){
                case start_three:
                        ThreeLEDsstate = led_0;
                        break;
                case led_0:
                        ThreeLEDsstate = led_1;
                        break;
                case led_1:
                        ThreeLEDsstate = led_2;
                        break;
                case led_2:
                        ThreeLEDsstate = led_0;
                        break;
                default:
                        ThreeLEDsstate = start_three;
                        break;
        }

        switch(ThreeLEDsstate){
                case start_three:
                        break;
                case led_0:
                        threeLEDs = 0x01;
                        break;
                case led_1:
                        threeLEDs = 0x02;
                        break;
                case led_2:
                        threeLEDs = 0x04;
                        break;
                default:
                        break;
        }
}

void Tick_Blink(){
        switch(BlinkingLEDstate){
                case start_blinking:
                        BlinkingLEDstate = blink_on;
                        break;
                case blink_on:
                        BlinkingLEDstate = blink_off;
                        break;
                case blink_off:
                        BlinkingLEDstate = blink_on;
                        break;
                default:
                        BlinkingLEDstate = start_blinking;
                        break;
        }

        switch(BlinkingLEDstate){
                case start_blinking:
                        break;
                case blink_on:
                        blinkingLED = 0x08;
                        break;
                case blink_off:
                        blinkingLED = 0x00;
                        break;
                default:
                        break;
        }
}

void Tick_Combine(){
        switch(CombineLEDstate){
                case start_combine:
                        CombineLEDstate = Init;
                        break;
                case Init:
                        CombineLEDstate = Init;
                        break;
                default:
                        break;
        }

        switch(CombineLEDstate){
                case start_combine:
                        break;
                case Init:
                        PORTB = ((threeLEDs)|(blinkingLED));
                        break;
                default:
                        break;
        }
}

int main(void){
        DDRB = 0xFF; PORTB = 0x00;
        unsigned long Three_Period = 0;
        unsigned long Blink_Period = 0;
        TimerSet(10);
        TimerOn();

        while(1){
        //call all three tick functions
        if(Three_Period >= 50){
        Tick_Three();
        Three_Period = 0;
        }

        if(Blink_Period >= 160){
        Tick_Blink();
        Blink_Period = 0;
        }
        Tick_Combine();

        while(!TimerFlag);
        TimerFlag = 0;
        Three_Period = Three_Period + 10;
        Blink_Period = Blink_Period + 10;
        }
        return 1;
}
