/*
 * main.c
 *
 *  Main file for Mobile-Speed-Display-Mk2 program
 *  Authors: Henry Bertolozzi, Jan Ang, Denis Chsherbachuk, Rishikesh Biswas
 */
#include "tm4c123gh6pm.h"

//define constants

// Use UART0 RXD and TXD connected to Port A Pins 0 and 1 respectively
#define U0RX BIT0
#define U0TX BIT1
// set baud rate to 9600 baud with 16MHz system clock
#define IBRD_VAL 104
#define FBRD_VAL 11

int main(void)
{
//initialize clock

//initialize serial communication
    //16,000,000 / (9600 * 16) = 104.1667 IBRD = 104
    //FBRD? 0.1666666 * 64 = 10.6666624 -> 11
    //Actual Divisor 104 + 11/64 = 104.1719 16,000,000 / 104.1719 / 16 = 9,599.5177
    //% error = 100 * (9600 - 9599.5177) / 9600 = 0.005 %
    //Typically 2% timing error is allowable. For 9600, the allowable range would be 9,408 to 9,792

    int data; // receive/transmit data
    // activate clock for GPIO Ports A
    SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOA;
    // activate clock for UART Module 0
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0;
    // Enable RXD/TXD pins for UART0 on PORT A pins 0 and 1
    GPIO_PORTA_AFSEL_R |= U0RX | U0TX; // select alternate function
    GPIO_PORTA_DEN_R |= U0RX | U0TX; // digital enable
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R
            & ~(GPIO_PCTL_PA0_M | GPIO_PCTL_PA1_M)) |
    GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX; // UART mode
    // setup UART 0 to transmit at 9600 baud N81
    UART0_CTL_R &= ~UART_CTL_UARTEN; // disable UART
    UART0_IBRD_R = IBRD_VAL; // set baud rate divisor
    UART0_FBRD_R = FBRD_VAL; // set fractional part of divisor
    UART0_LCRH_R = UART_LCRH_WLEN_8; // 8-bit word length, no parity, one stop bit
    UART0_CTL_R |= UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE; // enable UART (TX & RX)
//display initialization

//constant operation
    while (1)
    {
//get values from radar and compute speed

//activate port pins to control LED board to show speed value

//get values from PC
//send values to PC
        while (UART0_FR_R & UART_FR_RXFE)
        {
        } // wait for receive byte
        data = UART0_DR_R; // get received byte from FIFO
        while (UART0_FR_R & UART_FR_TXFF)
        {
        } // wait for space in transmit FIFO
        UART0_DR_R = data + 1; // put byte to transmit in FIFO
    }
}

