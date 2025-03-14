/*
 * initialization.h
 *
 *  Created on	: Mar 14, 2021
 *  Modified by	: Shyama M. Gandhi, Winter 2023 (This file functions have been adopted from Xilinx driver files)
 *
 *  This file has been created using inbuilt driver functions for UART and SPI. 
 *  The file also has a print_command_menu() function for displaying the menu on console.
 *  SPI 0 as MASTER, SPI 1 as SLAVE
 *  	  
 */

#ifndef SRC_INITIALIZATION_H_
#define SRC_INITIALIZATION_H_

#include "xuartps.h"	/* UART device driver */
#include "xspips.h"		/* SPI device driver */

/******************* Other defined functions for UART and SPI ****************/
extern int intializeUART( u16 deviceId );
extern int initializeSPI( u16 spiMaster, u16 spiSlave );
extern void printMenu( void );

/************************** Variable Definitions *****************************/

XUartPs Uart;		/* Instance of the UART Device */
XUartPs_Config *Config;	/* The instance of the UART-PS Config */

/*****************************************************************************/
XSpiPs spiInstMaster;
XSpiPs spiInstSlave;

#endif /* SRC_INITIALIZATION_H_ */

