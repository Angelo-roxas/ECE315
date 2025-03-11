#include "initialization.h"
#include "spi_driver.h"


void spiMasterRead(int byteCount)
{
	int count=0;

	for(count = 0; count < byteCount; count++){
		RxBuffer_Master[count] = SpiPs_RecvByte(spiInstMaster.Config.BaseAddress);
	}
}

void spiMasterWrite(u8 *Sendbuffer, int byteCount)
{
	u32 statusReg;
	int transCount = 0;

	/*
	 * Fill the TXFIFO with as many bytes as it will take (or as
	 * many as we have to send).
	 */

	while (byteCount > 0 && transCount < XSPIPS_FIFO_DEPTH) {
		SpiPs_SendByte(spiInstMaster.Config.BaseAddress, *Sendbuffer);
		Sendbuffer++;
		transCount++;
		byteCount--;
	}

	/*
	 * Wait for the transfer to finish by polling Tx fifo status.
	 */
	do {
		statusReg = XSpiPs_ReadReg(spiInstMaster.Config.BaseAddress, XSPIPS_SR_OFFSET);
	} while ((statusReg & XSPIPS_IXR_TXOW_MASK) == 0);
}

void spiSlaveRead(int byteCount)
{
	int count;
	u32 statusReg;

	/*
	 * Polling the Rx Buffer for Data
	 */
	do{
		statusReg = XSpiPs_ReadReg(spiInstSlave.Config.BaseAddress, XSPIPS_SR_OFFSET);
	}while(!(statusReg & XSPIPS_IXR_RXNEMPTY_MASK));

	/*
	 * Reading the Rx Buffer
	 */
	for(count = 0; count < byteCount; count++){
		RxBuffer_Slave[count] = SpiPs_RecvByte(spiInstSlave.Config.BaseAddress);
	}

}

void spiSlaveWrite(u8 *Sendbuffer, int byteCount)
{
	int transCount = 0;
	u32 statusReg;

	while ((byteCount > 0) && (transCount < XSPIPS_FIFO_DEPTH)) {
		SpiPs_SendByte( spiInstSlave.Config.BaseAddress, *Sendbuffer);
		Sendbuffer++;
		transCount++;
		byteCount--;
	}

	/*
	 * Wait for the transfer to finish by polling Tx fifo status.
	 */
	do {
		statusReg = XSpiPs_ReadReg( spiInstSlave.Config.BaseAddress, XSPIPS_SR_OFFSET);
	} while ((statusReg & XSPIPS_IXR_TXOW_MASK) == 0);
}

