/*
 * initialization.c
 *
 *  Created on: Feb 24, 2023
 *  Author: Shyama Gandhi
 *  Modified by	: Shyama M. Gandhi, Winter 2023 (This file functions have been adopted from Xilinx driver files)
 *
 *  This file has been created using inbuilt driver functions for UART and SPI.
 *
 */

#include "initialization.h"


void printMenu( void )
{
	xil_printf("***************** Welcome to Lab_3 of ECE-315 *****************\r\n");
	xil_printf("Select from the Command Menu to perform the desired operation.\r\n");
	xil_printf("Press <menu command number><ENTER>\r\n\n");
	xil_printf("\n*** UART Loop-back OFF ***\r\n");
	xil_printf("\n*** SPI Loop-back OFF ***\r\n");
	xil_printf("\n\t1: Toggle UART Loop-back Mode\r\n");
	xil_printf("\n\t2: Toggle SPI Loop-back Mode\r\n");

}


int intializeUART(u16 deviceId)
{
	int status;
	Config = XUartPs_LookupConfig(deviceId);
	if (Config == NULL) {
		return XST_FAILURE;
	}

	status = XUartPs_CfgInitialize(&Uart, Config, Config->BaseAddress);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XUartPs_SetOperMode(&Uart, XUARTPS_OPER_MODE_NORMAL);

	return XST_SUCCESS;
}


int initializeSPI( u16 spiMaster, u16 spiSlave )
{
	int status;
	XSpiPs_Config *spiConfigMaster;
	XSpiPs_Config *spiConfigSlave;

	/*
	 * Initialize the SPI driver for SPI 0 so that it's ready to use
	 */
	spiConfigMaster = XSpiPs_LookupConfig(spiMaster);
	if (spiConfigMaster == NULL ) {
		return XST_FAILURE;
	}

	status = XSpiPs_CfgInitialize( &spiInstMaster
								 , spiConfigMaster
								 , spiConfigMaster->BaseAddress
								 );

	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Initialize the SPI driver for SPI 1 so that it's ready to use
	 */

	spiConfigSlave = XSpiPs_LookupConfig(spiSlave);
	if (NULL == spiConfigSlave) {
		return XST_FAILURE;
	}

	status = XSpiPs_CfgInitialize( &spiInstSlave
								 , spiConfigSlave
								 , spiConfigSlave->BaseAddress
								 );
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	status = XSpiPs_SetOptions( &spiInstMaster, (XSPIPS_CR_CPHA_MASK) | (XSPIPS_MASTER_OPTION ) | (XSPIPS_CR_CPOL_MASK));
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	status = XSpiPs_SetOptions( &spiInstSlave, (XSPIPS_CR_CPHA_MASK) | (XSPIPS_CR_CPOL_MASK));
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;

}
