/*
 * Lab 1, Part 2 - RGB LED Control with FreeRTOS
 *
 * ECE-315 WINTER 2025 - COMPUTER INTERFACING
 * Created on: January 9, 2025
 * Author(s):  Antonio Andara Lara
 *
 * Summary:
 * 1) Update BSP to 1000 ticks/sec.
 * 2) Toggle RGB LED at varying frequencies to find persistence of vision threshold.
 * 3) Implement a PWM-like task to smoothly transition LED brightness.
 *
 * Deliverables:
 * - Show flicker elimination frequency and period.
 * - Demonstrate smooth brightness transitions.
 */


// Include FreeRTOS Libraries
#include "FreeRTOS.h"
#include "task.h"

// Include Xilinx Libraries
#include "xgpio.h"

// RGB LED Colors
#define RGB_OFF     0b000
#define RGB_RED     0b100
#define RGB_GREEN   0b010
#define RGB_BLUE    0b001
#define RGB_YELLOW  0b110 // Red + Green
#define RGB_CYAN    0b011 // Green + Blue
#define RGB_MAGENTA 0b101 // Red + Blue
#define RGB_WHITE   0b111

// RGB LED Device ID
#define RGB_LED_ID XPAR_AXI_LEDS_DEVICE_ID
#define RGB_CHANNEL 2

/*************************** Enter your code here ****************************/
    // TODO: Declare RGB LED peripheral
XGpio	RGBLedInst;

/*****************************************************************************/

/*************************** Enter your code here ****************************/
    // TODO: Task prototype
static void rgb_led_task(void *pvParameters);

/*****************************************************************************/

int main(void)
{
    int status;
/*************************** Enter your code here ****************************/
	// TODO:
	// 1) Configure the RGB LED pins as output.
	// 2) Create the FreeRTOS task for the RGB LED.
	// 3) Start the scheduler.

    status = XGpio_Initialize(&RGBLedInst,RGB_LED_ID);
	if(status != XST_SUCCESS){
		xil_printf("GPIO initialization for RGBLed failed.\r\n");
		return XST_FAILURE;
	}
	XGpio_SetDataDirection(&RGBLedInst,RGB_CHANNEL,0x00);

	xil_printf("Initialization Complete, System Ready!\n");

	xTaskCreate(rgb_led_task,					/* The function that implements the task. */
				"main task", 				/* Text name for the task, provided to assist debugging only. */
				configMINIMAL_STACK_SIZE, 	/* The stack allocated to the task. */
				NULL, 						/* The task parameter is not used, so set to NULL. */
				tskIDLE_PRIORITY,			/* The task runs at the idle priority. */
				NULL);
	vTaskStartScheduler();


/*****************************************************************************/


    while (1);
    return 0;
}


static void rgb_led_task_old(void *pvParameters)
{
/*************************** Enter your code here ****************************/
    // TODO: Declare a variable of type TickType_t named 'xDelay'.
    TickType_t xDelay = 100;
	uint32_t color = RGB_CYAN;

/*****************************************************************************/
    while (1){
/*************************** Enter your code here ****************************/
    // TODO: Implement a loop that increments xDelay by 1 tick in each iteration,
    //       Allow the loop to run for 3 seconds for each xDelay value.
	//       Use xil_printf to display xDelay and its associated period and frequency
	//       Select a color for the RGB LED.
	 TickType_t start = xTaskGetTickCount();

    	while ((xTaskGetTickCount() - start < 3000 )){
			XGpio_DiscreteWrite(&RGBLedInst, RGB_CHANNEL, RGB_BLUE);
			vTaskDelay(xDelay);
			xil_printf("xDelay: %u\n", xDelay);
			xil_printf("Period: %u ms\n", xDelay);
			xil_printf("Frequency: %u\n", 1000/xDelay);
			if (xTaskGetTickCount() - start > 3000 )
				break;
			XGpio_DiscreteWrite(&RGBLedInst, RGB_CHANNEL, 0);

			vTaskDelay(xDelay);
			xDelay++;

/*****************************************************************************/
    }
}

/*************************** Enter your code here ****************************/
// TODO: Write the second task to control the duty cycle of the RGB LED signal.
static void rgb_led_task(void *pvParameters){
    TickType_t period = 3000;//change period as needed for timing
    TickType_t onDelay;
    TickType_t offDelay;
    int duty_cycle = 0;
    int increasing = 1;

    while(1){
        //adjust duty cycle
        // Calculate on and off delays based on duty cycle

        onDelay = (period * duty_cycle)/100 ;
        offDelay = period - onDelay;

        // Simulate PWM by toggling the LED
        XGpio_DiscreteWrite(&RGBLedInst, RGB_CHANNEL, RGB_GREEN); // Turn on LED
        vTaskDelay(onDelay);

        XGpio_DiscreteWrite(&RGBLedInst, RGB_CHANNEL, 0); // Turn off LED
        vTaskDelay(offDelay);

        if (increasing){
                duty_cycle+=1;
                if (duty_cycle >=100) increasing =0; //start decreasing
            }
        else{
            duty_cycle-= 1;
            if (duty_cycle <= 0)
                increasing = 1; //start increasing
        }
        
        }
    }






}
/*****************************************************************************/
