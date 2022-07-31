/*
 * nrf24l_bme280_nano.c
 *
 * Created: 31.7.2022 17.07.16
 * Author : Kariantti
 */ 
#define  F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

// Set up UART
#ifndef BAUD
#define BAUD 9600
#endif
#include "includes/STDIO_UART.h"

#include "includes/nrf24l01.h"
#include "includes/nrf24l01-mnemonics.h"
#include "includes/spi.h"

void print_config(void)
{
	uint8_t data;
	printf("Startup successful\n\n nRF24L01+ configured as:\n");
	printf("-------------------------------------------\n");
	nrf24_read(CONFIG,&data,1);
	printf("CONFIG		0x%x\n",data);
	nrf24_read(EN_AA,&data,1);
	printf("EN_AA			0x%x\n",data);
	nrf24_read(EN_RXADDR,&data,1);
	printf("EN_RXADDR		0x%x\n",data);
	nrf24_read(SETUP_RETR,&data,1);
	printf("SETUP_RETR		0x%x\n",data);
	nrf24_read(RF_CH,&data,1);
	printf("RF_CH			0x%x\n",data);
	nrf24_read(RF_SETUP,&data,1);
	printf("RF_SETUP		0x%x\n",data);
	nrf24_read(STATUS,&data,1);
	printf("STATUS		0x%x\n",data);
	nrf24_read(FEATURE,&data,1);
	printf("FEATURE		0x%x\n",data);
	printf("-------------------------------------------\n\n");
}

// Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;


int main(void)
{
    /* Replace with your application code */
	char tx_message[32];
	strcpy(tx_message, "Hello from Nano");
	
	// Initialize UART
	uart_init();
	
	// Initialize NRF24L01 and print config
	nrf24_init();
	print_config();
	
	// Start listening incoming messages
	nrf24_start_listening();
	
	
    while (1) 
    {
		if(message_received)
		{
			// Message received, print it to console
			message_received = false;
			printf("Received message: %s\n", nrf24_read_message());
			// Send message as response
			_delay_ms(500);
			status = nrf24_send_message(tx_message);
			if(status == true) printf("Message sent successfully");
		}
    }
}

ISR(INT0_vect)
{
	message_received = true;
}
