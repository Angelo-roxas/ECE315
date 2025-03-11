/*
 * spi_Driver.h
 *
 *  Created on	: Mar 5, 2025
 *  Modified by	: Avik Mandal, Winter 2025 (This file functions have been adopted from Xilinx driver files)
 *.
 *  The file has all functions related to SPI
 *
 */

#ifndef SRC_SPI_DRIVER_H_
#define SRC_SPI_DRIVER_H_


#include "xuartps.h"	/* UART device driver */
#include "xspips.h"		/* SPI device driver */
#include "initialization.h"

extern void spiMasterWrite( u8 *sendbuffer, int byteCount );
extern void spiSlaveRead( int byteCount );
extern void spiSlaveWrite( u8 *sendbuffer, int byteCount );
extern void spiMasterRead( int byteCount );


/***************** Macros (Inline Functions) Definitions *********************/

#define SpiPs_RecvByte(BaseAddress) \
		(u8)XSpiPs_In32((BaseAddress) + XSPIPS_RXD_OFFSET)

#define SpiPs_SendByte(BaseAddress, Data) \
		XSpiPs_Out32((BaseAddress) + XSPIPS_TXD_OFFSET, (Data))

/*****************************************************************************/

u8 RxBuffer_Slave[1];
u8 RxBuffer_Master[1];


#endif /* SRC_SPI_DRIVER_H_ */
