
#include "settings.h"

#include <stdlib.h>
#include <string.h>

#include "usart_serial.h"

#include <avr/io.h>
#include <avr/interrupt.h>

#define USART_RX_MASK ( USART_RX_SIZE - 1 )
#define USART_TX_MASK ( USART_TX_SIZE - 1 )

static uint8_t USART_RxBuf  [USART_RX_SIZE];
static uint8_t USART_TxBuf  [USART_TX_SIZE];

static USART_Stream_Write(char ch, FILE * stream);

static FILE USART_Output_Stream = FDEV_SETUP_STREAM(USART_Stream_Write, NULL, _FDEV_SETUP_WRITE);

static volatile uint8_t USART_RxHead;
static volatile uint8_t USART_RxTail;
static volatile uint8_t USART_TxHead;
static volatile uint8_t USART_TxTail;

void (*USART_RxCallback)(uint8_t ch) = NULL;

void USART_Enable(uint64_t baudrate){
    uint64_t baud_setting = (F_CPU / 8 / baudrate - 1) / 2;

    memset(USART_RxBuf,0,USART_RX_SIZE);
    memset(USART_TxBuf,0,USART_TX_SIZE);

    UBRRH = (baud_setting >> 8);
    UBRRL = baud_setting;
    
    UCSRA &= ~(1 << U2X);                           // Don't use U2X
    UCSRB =  (1<<TXEN) | (1<<RXEN) | (1<<RXCIE);    // Enable TX, RX, Interrupt
    UCSRC = (1<<URSEL) | (3<<UCSZ0);                // Asyncronous mode

    USART_RxTail = 0;
    USART_RxHead = 0;
    USART_TxTail = 0;
    USART_TxHead = 0;
}

void USART_Tx(uint8_t data){
    uint8_t tmphead;
    tmphead = ( USART_TxHead + 1 ) & USART_TX_MASK;
    // Wait while full
    while ( tmphead == USART_TxTail ){};
    USART_TxBuf[tmphead] = data;
    USART_TxHead = tmphead;
    // If the UART ain't busy sending bytes, send the first byte.
    if((UCSRB & (1<<TXCIE)) != (1<<TXCIE)){
        // Enable Tx interrupt for the following bytes
        UCSRB |= (1<<TXCIE); 
         // Emulate the TX interruption for the first character
        uint8_t tmptail = ( USART_TxTail + 1 ) & USART_TX_MASK;
        USART_TxTail = tmptail;
        UDR = USART_TxBuf[tmptail];
    }
}

static int USART_Stream_Write(char ch, FILE *stream){
    USART_Tx(ch);
    return 0;
}

uint8_t USART_Rx(void){
    uint8_t tmptail;
    while ( USART_RxHead == USART_RxTail ){};
    tmptail = ( USART_RxTail + 1 ) & USART_RX_MASK;
    USART_RxTail = tmptail;
    return USART_RxBuf[tmptail]; 
}

ISR(USART_RXC_vect){
    uint8_t tmphead;
    uint8_t data = UDR;
    tmphead = ( USART_RxHead + 1 ) & USART_RX_MASK;
    USART_RxHead = tmphead;
    if ( tmphead != USART_RxTail ) 
        USART_RxBuf[tmphead] = data;
    if( USART_RxCallback != NULL )
        USART_RxCallback(data);
}

ISR(USART_TXC_vect){
    uint8_t tmptail;
    if ( USART_TxHead != USART_TxTail ){
        tmptail = ( USART_TxTail + 1 ) & USART_TX_MASK;
        USART_TxTail = tmptail;
        UDR = USART_TxBuf[tmptail];
    }
    else{
        // Empty queue, disable interrupt.
        UCSRB &= ~(1<<TXCIE);    
    }
}

void USART_SetCallback(USART_Callback callback){
    USART_RxCallback = callback;
}

FILE * USART_GetOutputStream(void){
    return &USART_Output_Stream;
}

