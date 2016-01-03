/**
* AVRKIT
* https://github.com/nczeroshift/avrkit/
*/

#include "music.h"
#include "utils.h"
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>

static const char     notes_letters[]    ={'c' ,'d','e','f','s','g','a','v','b','C','D','E'};
static const uint16_t notes_frequencies[]={ 261,294,329,349,369,392,440,466,493,523,587,659}; // Hz

static uint8_t        notes_timer_start[12]    ={0};
static uint8_t        notes_timer_prescaler[12]={0};

static uint8_t  player_n = 0;      // current note
static uint64_t player_last_t = 0; // time since current note started playing

static uint8_t timer2_start = 0;

#define TIMER2_CONFIG (1<<WGM20) | (1<<COM21)

void setTone(char tone){
    if(tone == ' '){
        TCCR2 = 0;
        return;
    }

    for(uint8_t i = 0;i < 12;i++)
        if(notes_letters[i] == tone){
            timer2_start = notes_timer_start[i];
            OCR2 = timer2_start >> 1;
            TCCR2 = TIMER2_CONFIG | notes_timer_prescaler[i];
            break;
        }
}

int64_t computeTimerValue(int64_t fcpu, int64_t div, int64_t freq){
    return 2 * (256 - fcpu / (2 * div * freq));
}

void MUSIC_InitTimer(uint64_t fcpu){
    uint16_t pre_scalers[] = {
        256,
        128,
        64,
        32
    };
    uint8_t pre_scalers_config[] = 
    {
        (1<<CS22) | (1<<CS21),
        (1<<CS22) | (1<<CS20),
        (1<<CS22), 
        (1<<CS21) | (1<<CS20)
    };

    for(uint8_t i = 0;i < 12;i++)
    {    
        notes_timer_start[i] = 0;
        notes_timer_prescaler[i] = 0;

        for(uint8_t j = 0;j < 4;j++)
        {
            int64_t v = computeTimerValue(fcpu,pre_scalers[j],notes_frequencies[i]);
            if(v <= 255)
            {
                notes_timer_start[i] = v;
                notes_timer_prescaler[i] = pre_scalers_config[j];
                break;
            }
        }
    }

    TCNT2 = 0;
    TIMSK |= (1 << TOIE2);
    TCCR2 |= TIMER2_CONFIG | pre_scalers_config[0];
}

void MUSIC_Clear(void){
    player_n = 0;
    player_last_t = 0;
}

uint8_t MUSIC_Play(char notes[], uint8_t beats[], uint8_t length, int32_t tempo, uint64_t time){  
    char note = notes[player_n];
    int beat = beats[player_n];

    uint64_t off = time - player_last_t;
    int duration = beat * tempo;

    if(off > duration){
        if(off - duration < 30){
            setTone(' ');
            return 1;
        }
        player_last_t = time;
        player_n++;
    }

    setTone(note);

    if( player_n >= length){
        player_n = 0;
        return 0;
    }

    return 1;
}

ISR(TIMER2_OVF_vect){
    TCNT2 = timer2_start;
}

