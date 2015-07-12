
#ifndef _AVRKIT_SETTINGS_H_
#define _AVRKIT_SETTINGS_H_

#define F_CPU           16000000    // CPU FREQUENCY

// USART SERIAL
#define USART_BAUD      9600        // Number of symbols per second
#define USART_RX_SIZE   64          // Max receive queue size
#define USART_TX_SIZE   64          // Max transmit queue size


// CLI PARSER
#define CLI_MAX_ARGS    8           // Max arguments per line
#define CLI_MAX_LENGTH  8           // Max characters per argument

// STATE MACHINE
#define STATE_MACHINE_MAX_STATES 16 // Max possible states

#endif 
