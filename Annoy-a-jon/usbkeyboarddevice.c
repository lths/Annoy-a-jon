/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== usbkeyboarddevice.c ========
 */

#include <stdbool.h>

/* XDCtools Header files */
#include <xdc/std.h>
#include <xdc/runtime/Error.h>
#include <xdc/runtime/System.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>

/* TI-RTOS Header files */
#include <ti/drivers/GPIO.h>

/* Example/Board Header files */
#include "Board.h"

/* USB Reference Module Header file */
#include "USBKBD.h"

#define TASKSTACKSIZE   768

const unsigned char text[] = "It Begins..... \n";
const unsigned char text1[] = "Hello Jon \n";
const unsigned char text2[] = "It's me. \n";
const unsigned char text3[] = "I need your help. \n";
const unsigned char text4[] = "Help ME! \n";

char ranchar = "k";
int randotime = 0;

int counter = 0;
int counter2 = 0;
int counter3 = 0;

int enloop = 0;

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];
/*
 *  ======== taskFxn ========
 */
Void taskFxn (UArg arg0, UArg arg1)
{
    USBKBD_State state;
    unsigned int currButton;
    unsigned int prevButton;
    int sent;

    while (true) {

        /* Block while the device is NOT connected to the USB */
        USBKBD_waitForConnect(BIOS_WAIT_FOREVER);

        /* Determine the status of the keyboard */
        USBKBD_getState(&state);

        /* Update LED outputs */
        GPIO_write(Board_LED0, state.capsLED ? Board_LED_ON : Board_LED_OFF);
        GPIO_write(Board_LED1, state.scrollLED ? Board_LED_ON : Board_LED_OFF);

        /*
         * When Board_BUTTON0 transitions from HIGH to LOW, print string to the
         * USB host
         */
        currButton = GPIO_read(Board_BUTTON0);
        GPIO_write(Board_LED0, Board_LED_ON);
        if((currButton == 0) && (prevButton != 0))
        {
            sent = USBKBD_putString((String)text, sizeof(text), BIOS_WAIT_FOREVER);
            System_printf("Sent %d bytes\n", sent);
            GPIO_write(Board_LED0, Board_LED_OFF);
            /*
            sent = USBKBD_putString((String)text2, sizeof(text2), BIOS_WAIT_FOREVER);
            sent = USBKBD_putString((String)text3, sizeof(text3), BIOS_WAIT_FOREVER);
            sent = USBKBD_putString((String)text4, sizeof(text4), BIOS_WAIT_FOREVER);
            sent = USBKBD_putString("543 \n", sizeof(text4), BIOS_WAIT_FOREVER);
            ranchar = ((rand()+32) % 127 + 32);
            sent = USBKBD_putString(ranchar, sizeof(ranchar), BIOS_WAIT_FOREVER);
            */
            while (1){


                //GPIO_write(Board_LED0, Board_LED_ON);

                //print previous lines after so many messages
                if(counter == 15){
                    sent = USBKBD_putString((String)text1, sizeof(text1), BIOS_WAIT_FOREVER);
                }
                if(counter == 20){
                    sent = USBKBD_putString((String)text2, sizeof(text1), BIOS_WAIT_FOREVER);
                }
                if(counter == 25){
                    sent = USBKBD_putString((String)text3, sizeof(text1), BIOS_WAIT_FOREVER);
                }
                if(counter == 30){
                    sent = USBKBD_putString((String)text4, sizeof(text1), BIOS_WAIT_FOREVER);
                }

                if(counter > 40){
                    counter = 0;
                }

                //generate random char and print
                ranchar = (rand() % 127 + 32);
                sent = USBKBD_putString(ranchar, sizeof(ranchar), BIOS_WAIT_FOREVER);
                counter++;

                //wait random amount of minutes, 1-10
                randotime = (rand() % 1800000 + 60000);
                Task_sleep(randotime);
                Task_sleep(100);

                }



        }
        prevButton = currButton;

        /* When connected, poll the pin once every 100ms */
        Task_sleep(100);

        //ranchar = ((rand()+32) % 127 + 32);
       // sent = USBKBD_putString(ranchar, sizeof(ranchar), BIOS_WAIT_FOREVER);
    }
}

/*
 *  ======== main ========
 */
int main(void)
 {
    /* Construct BIOS objects */
    Task_Params taskParams;

    /* Call board init functions */
    Board_initGeneral();
    Board_initGPIO();
    Board_initUSB(Board_USBDEVICE);

    /* Construct keyboard Task thread */
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.stack = &task0Stack;
    taskParams.priority = 2;
    Task_construct(&task0Struct, (Task_FuncPtr)taskFxn, &taskParams, NULL);

    /* Turn on user LED */
    GPIO_write(Board_LED0, Board_LED_ON);

    System_printf("Starting the USB Keyboard Device example\nSystem provider is "
                  "set to SysMin. Halt the target to view any SysMin contents"
                  " in ROV.\n");
    /* SysMin will only print to the console when you call flush or exit */
    System_flush();

    USBKBD_init();

    /* Start BIOS */
    BIOS_start();



    return (0);
}
