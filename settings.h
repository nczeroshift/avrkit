/**
* AVRKIT
* https://github.com/nczeroshift/avrkit/
*/

#ifndef _AVRKIT_SETTINGS_H_
#define _AVRKIT_SETTINGS_H_

// Note, for devices type defines
// http://www.atmel.com/webdoc/AVRLibcReferenceManual/using_tools_1using_avr_gcc_mach_opt.html

// USART SERIAL
#ifdef __AVR_ATmega8__
#define USART_RX_SIZE   64          
#define USART_TX_SIZE   64          
#elif defined(__AVR_ATtiny4313__)
#define USART_RX_SIZE   4          
#define USART_TX_SIZE   4          
#endif


// CLI PARSER
#define CLI_MAX_ARGS    8           // Max arguments per line
#define CLI_MAX_LENGTH  8           // Max characters per argument

// STATE MACHINE
#define STATE_MACHINE_MAX_STATES 6 // Max possible states

#endif 
