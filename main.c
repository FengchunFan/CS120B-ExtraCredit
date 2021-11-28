#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif

unsigned char i = 0x00;
unsigned char time = 0x06;
enum States {Start, Init, WaterCheck, MakeCoffee, Done} state;
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

void Tick(){
  switch(state){
        case Start:
                state = Init;
                break;
        case Init:
                if((~PINA & 0x0F) == 0x01){
                        state = WaterCheck;
                }else{
                        state = Init;
                }
                break;
        case WaterCheck:
                i = i + 1;
                if(((i%2) == 0) && ((~PINA & 0x0F) == 0x02)){
                        i = 0;
                        state = MakeCoffee;
                }
                if(i>20 && ((~PINA & 0x0F)!= 0x02)){
                        i = 0;
                        state = Init;
                }
                break;
        case MakeCoffee:
                i = i + 1;
                if(((~PINA & 0x0F) == 0x04) && (time < 0x06)){
                        time = time + 1;
                }else if (((~PINA & 0x0F) == 0x08) && (time > 0x02)){
                        time = time - 1;
                }
                if(i >= 60){
                        time = time - 1;
                }
                if(time == 0){
                        state = Done;
                }
                break;

        case Done:
                i = i + 1;
                if(i > 20){
                        state = Init;
                }
                break;
        default:
                state = Start;
                break;
  }
        
  switch(state){
        case Start:
                break;
        case Init:
                PORTB = 0x00;
                break;
        case WaterCheck:
                PORTB = 0x01;
                break;
        case MakeCoffee:
                break;
        case Done:
                PORTB = 0x02;
        default:
                break;
  }
}

int main(void) {
    DDRA = 0x00; PORTA = 0xFF;
    DDRB = 0xFF; PORTB = 0x00;
    TimerSet(500);
    TimerOn();
    while (1) {
        Tick();
        while(!TimerFlag);
        TimerFlag = 0;
    }
}
