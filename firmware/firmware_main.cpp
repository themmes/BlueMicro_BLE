/*
Copyright 2018 <Pierre Constantineau, Julian Komaromy>

3-Clause BSD License

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR 
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/
/**************************************************************************************************************************/
#include <bluefruit.h>
#include <Bluefruit_FileIO.h>
#include "firmware.h"

/**************************************************************************************************************************/
// Keyboard Matrix
byte rows[] MATRIX_ROW_PINS;        // Contains the GPIO Pin Numbers defined in keyboard_config.h
byte columns[] MATRIX_COL_PINS;     // Contains the GPIO Pin Numbers defined in keyboard_config.h  

const uint8_t boot_mode_commands [BOOT_MODE_COMMANDS_COUNT][2] BOOT_MODE_COMMANDS;

bool isReportedReleased = true;
uint8_t monitoring_state = STATE_BOOT_INITIALIZE;

/**************************************************************************************************************************/
// put your setup code here, to run once:
/**************************************************************************************************************************/
void setup() {

  LOG_LV1("BLEMIC","Starting %s" ,DEVICE_NAME);

  setupGpio();                                                                // checks that NFC functions on GPIOs are disabled.

  Scheduler.startLoop(monitoringloop);                                        // Starting second loop task for monitoring tasks
  Scheduler.startLoop(keyscanningloop);                                       // Starting third loop task for key scanning

  #if BACKLIGHT_PWM_ON == 1 //setup PWM module
    setupPWM();
  #endif
  // Set up keyboard matrix and start advertising
  Keyboard::setupKeyboard();
  setupMatrix();

  setupBluetooth();
  startAdv(); 
};
/**************************************************************************************************************************/
//
/**************************************************************************************************************************/
void setupMatrix(void) {
    //inits all the columns as INPUT
   for (const auto& column : columns) {
      LOG_LV2("BLEMIC","Setting to INPUT Column: %i" ,column);
      pinMode(column, INPUT);
    }

   //inits all the rows as INPUT_PULLUP
   for (const auto& row : rows) {
      LOG_LV2("BLEMIC","Setting to INPUT_PULLUP Row: %i" ,row);
      pinMode(row, INPUT_PULLUP);
    }
};
/**************************************************************************************************************************/
// Keyboard Scanning
/**************************************************************************************************************************/
void scanMatrix() {
  uint32_t pindata = 0;
  for(int j = 0; j < MATRIX_ROWS; ++j) {                             
    //set the current row as OUPUT and LOW
    pinMode(rows[j], OUTPUT);
    #if DIODE_DIRECTION == COL2ROW                                         
    digitalWrite(rows[j], LOW);                                       // 'enables' a specific row to be "low" 
    #else
    digitalWrite(rows[j], HIGH);                                       // 'enables' a specific row to be "HIGH"
    #endif
    //loops thru all of the columns
    for (int i = 0; i < MATRIX_COLS; ++i) {
          #if DIODE_DIRECTION == COL2ROW                                         
          pinMode(columns[i], INPUT_PULLUP);                              // 'enables' the column High Value on the diode; becomes "LOW" when pressed 
          #else
          pinMode(columns[i], INPUT_PULLDOWN);                              // 'enables' the column High Value on the diode; becomes "LOW" when pressed
          #endif
    }

      delay(1);   // using the FreeRTOS delay function reduced power consumption from 1.5mA down to 0.9mA
      // need for the GPIO lines to settle down electrically before reading.
     /*#ifdef NRFX_H__  // Added to support BSP 0.9.0
         nrfx_coredep_delay_us(1);
      #else            // Added to support BSP 0.8.6
        nrf_delay_us(1);
      #endif*/

      pindata = NRF_GPIO->IN;                                         // read all pins at once
     for (int i = 0; i < MATRIX_COLS; ++i) {
      Keyboard::scanMatrix((pindata>>(columns[i]))&1, millis(), j, i);       // This function processes the logic values and does the debouncing
      pinMode(columns[i], INPUT);                                     //'disables' the column that just got looped thru
     }
    pinMode(rows[j], INPUT);                                          //'disables' the row that was just scanned
   }                                                                  // done scanning the matrix
};
/**************************************************************************************************************************/
// Communication with computer and other boards
/**************************************************************************************************************************/
void sendKeyPresses() {
#pragma GCC message "Compiling key sending in firmware"
    //update state data - Data is in Keyboard::currentReport  
    Keyboard::updateReport(); 
    
    if (Keyboard::getReportChanged())  //any key presses anywhere?
    {                                                                              
        //get the current report from the keyboard
        auto currentReport = Keyboard::getCurrentReport(); 

        //convert the current report into a c-style array
        //and retreive it's length (constant if the keyboard
        //has HID, but variable is it doesn't)
        auto report = currentReport.data();
        auto length = currentReport.size();

        if (Keyboard::getReportEmpty())
        {
            sendRelease();
        }
        else 
        {
            sendKeys(report, length);
        }

        //isReportedReleased = false;
        /*
        LOG_LV1("MXSCAN","SEND: %i %i %i %i %i %i %i %i %i %i", millis(),
                report[0], report[1],
                report[2], report[3], 
                report[4], report[5], 
                report[6], report[7]);        
                */
    }
    
    //layer comms 
#if defined(KBLINK_CLIENT) || defined(KBLINK_SERVER)
    if(Keyboard::getLayerChanged())                                              
    {   
        auto layer = Keyboard::getLocalLayer();
        sendlayer(layer);
        LOG_LV1("MXSCAN","Layer %i  %i", millis(), layer);
    } 
#endif
}

/**************************************************************************************************************************/
// put your main code here, to run repeatedly:
/**************************************************************************************************************************/
void loop() {
    // put your main code here, to run repeatedly:

    unsigned long timesincelastkeypress = millis() - Keyboard::getLastPressed();

#if SLEEP_ACTIVE == 1
    gotoSleep(timesincelastkeypress, Bluefruit.connected());
#endif

#if BLE_CENTRAL_COUNT != 0  
    if ((timesincelastkeypress < 10)
            && !Bluefruit.Central.connected()
            && !Bluefruit.Scanner.isRunning())
    {
        //0 = Don't stop scanning after 0 seconds
        Bluefruit.Scanner.start(0);                                                     
    }
#endif

#if BACKLIGHT_PWM_ON == 1
    updatePWM(timesincelastkeypress);
#endif

    if (monitoring_state == STATE_BOOT_MODE)
    {
        //update state data - Data is in Keyboard::currentReport
        Keyboard::updateReport();                                            
        if (!(Keyboard::getReportEmpty()))
        {
            //loop through BOOT_MODE_COMMANDS and compare with the first key being pressed
            //assuming only 1 key will be pressed when in boot mode.
            for (int i = 0; i < BOOT_MODE_COMMANDS_COUNT; ++i)          
            {
                if(Keyboard::getCurrentReport().data()[1] == boot_mode_commands[i][0])
                {
                    monitoring_state = boot_mode_commands[i][1];
                }
            }
        }
    } 
    delay(HIDREPORTINGINTERVAL*4);
};
/**************************************************************************************************************************/
// put your key scanning code here, to run repeatedly:
/**************************************************************************************************************************/
void keyscanningloop () {
  #if MATRIX_SCAN == 1
    scanMatrix();
  #endif
  #if SEND_KEYS == 1
    sendKeyPresses();    // how often does this really run?
  #endif

  delay(HIDREPORTINGINTERVAL);
}
//********************************************************************************************//
//* Battery Monitoring Task - runs infrequently - except in boot mode                        *//
//********************************************************************************************//
void monitoringloop() {
  switch(monitoring_state)
  {
    case STATE_BOOT_INITIALIZE:
        monitoring_state = STATE_BOOT_MODE;
      break;    
    case STATE_BOOT_MODE:
      if (millis()>BOOT_MODE_DELAY) {monitoring_state = STATE_MONITOR_MODE;}
      delay(25); // adds a delay to minimize power consumption during boot mode. 
      break;    
    case STATE_BOOT_CLEAR_BONDS:
       // Serial.println();
       // Serial.println("----- Before -----\n");
       // bond_print_list(BLE_GAP_ROLE_PERIPH);
       // bond_print_list(BLE_GAP_ROLE_CENTRAL);
      //  Bluefruit.clearBonds();
      //  Bluefruit.Central.clearBonds();
        InternalFS.format(true);  // using formatting instead of clearbonds due to the potential issue with corrupted file system and the keybord being stuck not being able to pair and save bonds.
        
      //  Serial.println();
       // Serial.println("----- After  -----\n");
        
       // bond_print_list(BLE_GAP_ROLE_PERIPH);
       // bond_print_list(BLE_GAP_ROLE_CENTRAL);
        monitoring_state = STATE_MONITOR_MODE;
      break;    
    case STATE_BOOT_SERIAL_DFU:
        enterSerialDfu();
      break;    
    case STATE_BOOT_WIRELESS_DFU:
        enterOTADfu();
      break;
    case STATE_MONITOR_MODE:
                #if BLE_LIPO_MONITORING == 1
                  updateBattery();
                #endif
                delay(30000);                                             // wait 30 seconds before a new battery update.  Needed to minimize power consumption.
      break;    
    case STATE_BOOT_UNKNOWN:
      break;
    default:
      break;
    
  } 
};
//********************************************************************************************//
//* Idle Task - runs when there is nothing to do                                             *//
//* Any impact of placing code here on current consumption?                                  *//
//********************************************************************************************//
void rtos_idle_callback(void) {
  // Don't call any other FreeRTOS blocking API()
  // Perform background task(s) here
  sd_app_evt_wait();  // puts the nrf52 to sleep when there is nothing to do.  You need this to reduce power consumption. (removing this will increase current to 8mA)
};
