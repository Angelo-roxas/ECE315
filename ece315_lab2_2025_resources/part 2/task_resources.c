void vLoginTask(void *pvParameters)
{
    LoginData loginData;
    vTaskDelay(pdMS_TO_TICKS(30000));
    while (!loggedIn) {
    	getParameter("username", loginData.username);
		getParameter("password", loginData.password);
        /* Send the login data for hashing and verification */
        xQueueSend(xLoginQueue, &loginData, 0);
		vTaskDelay(pdMS_TO_TICKS(100000));
    }
	vTaskDelete(NULL);
}

void vHashingTask(void *pvParameters)
{
    LoginData loginData;
    char userString[128]; // Buffer to hold "username::password"
    BYTE hash[HASH_LENGTH];
    char computedHashStr[HASH_STR_SIZE];
    bool loginSuccess;

    while (1) {
        if (xQueueReceive(xLoginQueue, &loginData, 0) == pdPASS) {
            concatenateStrings(loginData.username, loginData.password, userString, sizeof(userString));
            sha256String(userString, hash);
            hashToString(hash, computedHashStr);
            loginSuccess = false;

            for (int i = 0; i < registeredUserCount; i++) {
                if (strcmp(computedHashStr, registeredUsers[i].hashString) == 0) {
                    loginSuccess = true;
                    break;
                }
            }
			
            if (loginSuccess) {
                xil_printf("\nLogin successful!\n");
                loggedIn = true;  // Signal successful login
				
                xTaskCreate( vUartCommandTask,
							 "UART command Task",
							 configMINIMAL_STACK_SIZE + 200,
							 NULL,
							 tskIDLE_PRIORITY + 1,
							 NULL);
							 
            } else {
                xil_printf("\nLogin failed! Invalid credentials.\n");
            }
            vTaskDelay(pdMS_TO_TICKS(100000));
        }
		vTaskDelay(xPollPeriod);
    }
}


void vUartCommandTask(void *pvParameters)
{
    char buffer[128];
    char cmdStr[3];
    char action;
    Message msg;
    TimerHandle_t xLogoutTimer;

    xLogoutTimer = xTimerCreate( "LogoutTimer",
    							 pdMS_TO_TICKS(30000),
								 pdFALSE,
                                 (void *) xTaskGetCurrentTaskHandle(),
								 vLogoutTimerCallback);

    if (xLogoutTimer == NULL) {
        xil_printf("Failed to create logout timer!\n");
        vTaskDelete(NULL);
    }
    // Start the timer.
    xTimerStart(xLogoutTimer, 0);

    while (1) {
        xil_printf("Enter data (or type 'logout' to logout, or '<command> <action>'): ");

        receiveInput(buffer, sizeof(buffer));
        xTimerReset(xLogoutTimer, 0);

        if (strcmp(buffer, "logout") == 0) {
            xil_printf("\nLogging out...\n");
            loggedIn = false;
			
            xTaskCreate( vLoginTask,
            			 "Login Task",
						 configMINIMAL_STACK_SIZE + 200,
						 NULL, tskIDLE_PRIORITY + 1,
						 NULL);
            
			xTimerStop(xLogoutTimer, 0);
            vTaskDelete(NULL);
        } else if (sscanf(buffer, "%2s %c", cmdStr, &action) == 2) {
            if (strcmp(cmdStr, "E7") == 0) {
                msg.type = 't';
            } else if (strcmp(cmdStr, "A5") == 0) {
                msg.type = 'a';
            } else {
                xil_printf("\nUnrecognized command: %s\n", cmdStr);
                continue;
            }

            msg.action = action;

            if (msg.type == 't' || msg.type == 'c' || msg.type == 'f' ||
                msg.type == 'Q' || msg.type == 'p') {
                if (xQueueSend(xRGBQueue, &msg, 0) == pdPASS){
                	xil_printf("\nRGB Command '%s' with action '%c' sent.\n", cmdStr, action);
                }else {
                	xil_printf("\nError sending RGB command.\n");
                }
            }else if (msg.type == 'a' || msg.type == 's' || msg.type == 'r' || msg.type == 'b') {
                if (xQueueSend(xLedQueue, &msg, 0) == pdPASS){
                	xil_printf("\nLED Command '%s' with action '%c' sent.\n", cmdStr, action);
                }else {
                	xil_printf("\nError sending LED command.\n");
                }
            }
        } else {
            xil_printf("\nEcho: %s\n", buffer);
        }
    }
}


void vLogoutTimerCallback(TimerHandle_t xTimer)
{
    TaskHandle_t xUartCommandTask = (TaskHandle_t) pvTimerGetTimerID(xTimer);
    xil_printf("\nInactivity timeout: Logging out...\n");

    loggedIn = false;  // Clear the login flag

    xTaskCreate( vLoginTask,
				 "Login Task",
				 configMINIMAL_STACK_SIZE + 200,
                 NULL,
				 tskIDLE_PRIORITY + 1,
				 NULL);

    vTaskDelete(vUartCommandTask);
}
