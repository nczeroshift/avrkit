
#define F_CPU           12000000    // CPU FREQUENCY
#define USART_BAUD      9600        // Number of symbols per second

#include "cli_parser.h"
#include "usart_serial.h"
#include "i2c_master.h"
#include "adc.h"
#include "pwm_osc1.h"
#include "music.h"

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h> 

struct CLI_Parser_Context cliParser;

uint16_t battery_value = 0xFFFF;
uint8_t enable_flags[8] = {0,0,0,0, 0,0,0,0};

// ATmega8
// TIMER_0 - TIME/STATE
// TIMER_1 - PWM MOTOR DRIVER TWO CHANNEL
// TIMER_2 - PWM BUZZER MUSIC

void rxCallback(uint8_t ch)
{
    if(ch == '\r')
    {
        char type = cliParser.args[0][0];
        // m -> motor control
        // m [l,r] [u,d] [pwm], e.g: m l u 100
        // b -> fetch battery level
        // e -> enable
        // e [0..7] [0,1]
        if(cliParser.currentArg == 3)
        {
            if(type == 'm')
            {
                char motorId = cliParser.args[1][0];
                char direction = cliParser.args[2][0];    
                uint16_t pwmValue = atoi(cliParser.args[3]);

                if(motorId == 'l'){
                    OCR1A = pwmValue;
                    if(direction == 'u')
                        PORTD |= (1<<PD7);
                    else if(direction == 'd')
                        PORTD &= ~(1<<PD7);
                          
                }
                else if(motorId == 'r'){
                    OCR1B = pwmValue;
                    if(direction == 'u')
                        PORTD |= (1<<PD6);
                    else if(direction == 'd')
                        PORTD &= ~(1<<PD6);
                }
            }
            else if(type == 'e')
            {
                MUSIC_Clear();
                uint8_t channel = atoi(cliParser.args[1]);
                uint8_t state = cliParser.args[2][0] == '1';
                if(channel < 8)
                    enable_flags[channel] = state;
            }
        }
        else if(cliParser.currentArg == 1)
        {
            if(type == 'b'){
                battery_value = 0xFFFF;
                ADC_Start(0);
            }
        }
      

        CLI_Parser_Clear(&cliParser);
    }
    else if(ch != '\n'){
        CLI_Parse_Character(&cliParser,(char)ch);   
    }
}

void adcCallback(uint8_t ch, uint16_t value){
    battery_value = value;
}

volatile uint64_t timeCounter = 0;

void TIMER0_Init(void){
    TCNT0 = 0;
    TCCR0 = (1 << CS01 ) | (1 << CS00);
    TIMSK = (1 << TOIE0);
}

ISR(TIMER0_OVF_vect){
	TCNT0 = 130;
    timeCounter++;
}

void christmas_init(void){
    DDRC |= (1<<PC5);
    DDRC |= (1<<PC4);
    DDRC |= (1<<PC3);
}

void christmas_blink(void){
    static uint8_t pattern;

    if((pattern & 0x03) == 0x00)
        PORTC |= (1<<PC5);
    else
        PORTC &= ~(1<<PC5);

    if((pattern & 0x03) == 0x01)
        PORTC |= (1<<PC4);
    else
        PORTC &= ~(1<<PC4);

    if((pattern & 0x03) == 0x02)
        PORTC |= (1<<PC3);
    else
        PORTC &= ~(1<<PC3);

    pattern++;
}

int32_t intro_length    = 5;
int32_t intro_tempo     = 250;
char    intro_notes[]   = "cedf ";
uint8_t intro_beats[]   = {1,2,1,2,8};


// CHRISTMAS
int christmas_length = 31;
char christmas_notes[] = "dggagseceaabagsdsbbCbageddeasg ";
uint8_t christmas_beats[] = {4,4,2,2,2,2,4,4,4,4,2,2,2,2,4,4,4,4,2,2,2,2,4,4,2,2,4,4,4,8,8};
int christmas_tempo = 250;


int main(void)
{
    ADC_Set(ADC_AVCC,ADC_PRESCALE_DEFAULT);
    ADC_SetCallback(adcCallback);

    USART_SetCallback(rxCallback);
    stdout = USART_GetOutputStream();
    USART_Enable(USART_BAUD,F_CPU);

    DDRB |= (1<<PB0); // State Led
    
    DDRB |= (1<<PB3); // OSC2

    DDRB |= (1<<PB1); // PWM 1
    DDRB |= (1<<PB2); // PWM 2

    DDRD |= (1<<PD6); // Direction 2
    DDRD |= (1<<PD7); // Direction 1

    PORTB &= ~(1 << PB1); // LOW 1
    PORTB &= ~(1 << PB2); // LOW 1

    PWM_OSC1_Set(0,0,256);
    PWM_OSC1_Enable();

    christmas_init();

    TIMER0_Init();
    //TIMER2_Init();

    MUSIC_InitTimer(F_CPU);

    sei();
    
    printf("bot, build %s at %s\n\r",__DATE__,__TIME__);

    uint64_t lastCounter = 0;

 
    uint8_t boot_song = 1;

    while(1){

        if(boot_song && !MUSIC_Play(intro_notes,intro_beats,intro_length,intro_tempo,timeCounter)){
            boot_song = 0;
            PORTB &= ~(1<<PB3);
            TCCR2 = 0;
        }

        if(battery_value != 0xFFFF){
            printf("b %d\n\r",battery_value);
            battery_value = 0xFFFF;
        }

        if(enable_flags[1]){
            MUSIC_Play(christmas_notes,christmas_beats,christmas_length,christmas_tempo,timeCounter);
        }
        else if(!boot_song){
            PORTB &= ~(1<<PB3);
            TCCR2 = 0;
        }  

        if(timeCounter - lastCounter > 1000){
            lastCounter = timeCounter;
            if(PORTB & (1<<PB0))
                PORTB &= ~(1 << PB0);
            else
                PORTB |= (1 << PB0);
            
            if(enable_flags[0])
                christmas_blink();
        }

    }

    return 0;
}
