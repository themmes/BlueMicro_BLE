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
#include "bluetooth.h"

// Device Information Service
BLEDis bledis;  

//This is usually on HUB/SPOKE 
#if BLE_HID_COUNT != 0
//The handle for the HID connection
//TODO if we want multiple hid connections, then there need
//to be more connection handler
uint16_t hid_conn_hdl;

//The HID Service;
BLEHidAdafruit blehid;
#endif 

#if BLE_LIPO_MONITORING == 1 
extern BLEBas blebas; 
#endif

//SPOKE is the kblink server 
#ifdef KBLINK_SERVER

// Keyboard Link Service - Slave/Server Side
BLEService KBLinkService = BLEService(UUID128_SVC_KEYBOARD_LINK);
BLECharacteristic KBLinkChar_Layers        = BLECharacteristic(UUID128_CHR_KEYBOARD_LAYERS);
BLECharacteristic KBLinkChar_Layer_Request = BLECharacteristic(UUID128_CHR_KEYBOARD_LAYER_REQUEST); 
BLECharacteristic KBLinkChar_Buffer        = BLECharacteristic(UUID128_CHR_KEYBOARD_BUFFER); 
#endif

// TODO: provision for multiple master/slave links
//
//HUB is the kblink client
#ifdef KBLINK_CLIENT
BLEClientService KBLinkClientService = BLEClientService(UUID128_SVC_KEYBOARD_LINK);     
BLEClientCharacteristic KBLinkClientChar_Layers        = BLEClientCharacteristic(UUID128_CHR_KEYBOARD_LAYERS);
BLEClientCharacteristic KBLinkClientChar_Layer_Request = BLEClientCharacteristic(UUID128_CHR_KEYBOARD_LAYER_REQUEST);
BLEClientCharacteristic KBLinkClientChar_Buffer        = BLEClientCharacteristic(UUID128_CHR_KEYBOARD_BUFFER); 
#endif
/**************************************************************************************************************************/
void setupBluetooth(void)
{

    //Defined in firmware_config.h
    Bluefruit.begin(BLE_PERIPHERAL_COUNT, BLE_CENTRAL_COUNT);

    //make sure the BlueFruit connection LED is not toggled.
    Bluefruit.autoConnLed(false);
    // Defined in bluetooth_config.h
    Bluefruit.setTxPower(DEVICE_POWER);
    Bluefruit.configUuid128Count(UUID128_COUNT);
    
    // Defined in keyboard_config.h
    Bluefruit.setName(DEVICE_NAME); 
    
    // helps troubleshooting...
    Bluefruit.configServiceChanged(true);

    // 0.10.1: not needed for master...
    //Bluefruit.setConnInterval(9, 12);                                 

    // Configure and Start Device Information Service
    bledis.setManufacturer(MANUFACTURER_NAME);                                  // Defined in keyboard_config.h
    bledis.setModel(DEVICE_MODEL);                                              // Defined in keyboard_config.h
    bledis.begin();

#if BLE_LIPO_MONITORING == 1
    // Configure and Start Battery Service
    blebas.begin();
    blebas.write(100); // put the battery level at 100% - until it is updated by the battery monitoring loop.
    readVBAT(); // Get a single ADC sample and throw it away
#endif

//peripheral should always be on
#if BLE_PERIPHERAL_COUNT != 0
    Bluefruit.Periph.setConnectCallback(prph_connect_callback);
    Bluefruit.Periph.setDisconnectCallback(prph_disconnect_callback);  
#endif

#ifdef KBLINK_SERVER
    //uint8_t linkdata[7] = {0,0,0,0,0,0,0};
    
    //Configure Keyboard Link Service
    KBLinkService.begin();

    KBLinkChar_Layers.setProperties(CHR_PROPS_NOTIFY+ CHR_PROPS_READ);
    KBLinkChar_Layers.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    KBLinkChar_Layers.setFixedLen(1);
    KBLinkChar_Layers.setUserDescriptor("Keyboard Layer");

    //0.10.1 - second parameter is the "use adafruit calback" 
    //to call adafruit's method before ours.  Not sure what it does.
    KBLinkChar_Layers.setCccdWriteCallback(cccd_callback,true);     
    KBLinkChar_Layers.begin();
    
    //initialize with layer 0 
    KBLinkChar_Layers.write8(0); 

    KBLinkChar_Layer_Request.setProperties(CHR_PROPS_WRITE + CHR_PROPS_WRITE_WO_RESP);
    KBLinkChar_Layer_Request.setPermission(SECMODE_NO_ACCESS, SECMODE_OPEN );
    KBLinkChar_Layer_Request.setFixedLen(1);
    KBLinkChar_Layer_Request.setUserDescriptor("Keyboard Layer Request");
    KBLinkChar_Layer_Request.setWriteCallback(layer_request_callback);
    KBLinkChar_Layer_Request.begin();
    KBLinkChar_Layer_Request.write8(0);  // initialize with layer 0

    KBLinkChar_Buffer.setProperties(CHR_PROPS_NOTIFY+ CHR_PROPS_READ);
    KBLinkChar_Buffer.setPermission(SECMODE_OPEN, SECMODE_NO_ACCESS);
    //Shouldn't set fixed length - the server sends a variable 
    //length, encoded message 
    //KBLinkChar_Buffer.setFixedLen(7);
    KBLinkChar_Buffer.setUserDescriptor("Keyboard HID Buffer");
    KBLinkChar_Buffer.setCccdWriteCallback(cccd_callback,true);     /// 0.10.1 - second parameter is the "use adafruit calback" to call adafruit's method before ours.  Not sure what it does.
    KBLinkChar_Buffer.begin();
    
    //initialize with empty buffer 
    KBLinkChar_Buffer.write(nullptr, 0); 
#endif

    /* Start BLE HID
     * Note: Apple requires BLE device must have min connection interval >= 20m
     * ( The smaller the connection interval the faster we could send data).
     * However for HID and MIDI device, Apple could accept min connection interval 
     * up to 11.25 ms. Therefore BLEHidAdafruit::begin() will try to set the min and max
     * connection interval to 11.25  ms and 15 ms respectively for best performance.
     */
#if BLE_HID_COUNT != 0
    blehid.begin();
    // Set callback for set LED from central
    // TODO
    // takes two parameters with library version 0.10.1
    // uint16_t conn_hdl and uint8_t leds_bitmap
    // blehid.setKeyboardLedCallback(set_keyboard_led);
#endif

    /* Set connection interval (min, max) to your perferred value.
     * Note: It is already set by BLEHidAdafruit::begin() to 11.25ms - 15ms
     * min = 9*1.25=11.25 ms, max = 12*1.25= 15 ms 
     */

#if KEYBOARD_MODE == HUB 
#pragma message "compiling callbacks and scanner in bluetooth"
    //forward declare these callback functions
    void notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len);
    void scan_callback(ble_gap_evt_adv_report_t* report);
    void cent_connect_callback(uint16_t conn_handle);
    void cent_disconnect_callback(uint16_t conn_handle, uint8_t reason);

    KBLinkClientService.begin();
    KBLinkClientChar_Layers.begin();
    KBLinkClientChar_Layers.setNotifyCallback(notify_callback);
    KBLinkClientChar_Buffer.begin();
    KBLinkClientChar_Buffer.setNotifyCallback(notify_callback);
    KBLinkClientChar_Layer_Request.begin(); 

    Bluefruit.Scanner.setRxCallback(scan_callback);
    Bluefruit.Scanner.restartOnDisconnect(true);
    Bluefruit.Scanner.filterRssi(-80);                                              // limits very far away devices - reduces load
    Bluefruit.Scanner.filterUuid(BLEUART_UUID_SERVICE, UUID128_SVC_KEYBOARD_LINK);  // looks specifically for these 2 services (A OR B) - reduces load
    Bluefruit.Scanner.setInterval(160, 80);                                         // in unit of 0.625 ms  Interval = 100ms, Window = 50 ms
    Bluefruit.Scanner.useActiveScan(false);                                         // If true, will fetch scan response data
    Bluefruit.Scanner.start(0);                                                     // 0 = Don't stop scanning after 0 seconds

    Bluefruit.Central.setConnectCallback(cent_connect_callback);
    Bluefruit.Central.setDisconnectCallback(cent_disconnect_callback);
#endif
}


/***********************************************************************************************/
/* Start advertising */
void startAdv(void)
{  
    // Advertising packet
    Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
    Bluefruit.Advertising.addTxPower();

#if BLE_HID_COUNT != 0
    Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);

    // Include BLE HID service
    Bluefruit.Advertising.addService(blehid);
#endif

#ifdef KBLINK_SERVER
    Bluefruit.Advertising.addUuid(UUID128_SVC_KEYBOARD_LINK);
    Bluefruit.Advertising.addUuid(UUID128_CHR_KEYBOARD_LAYERS);
    Bluefruit.Advertising.addUuid(UUID128_CHR_KEYBOARD_LAYER_REQUEST);
    Bluefruit.Advertising.addUuid(UUID128_CHR_KEYBOARD_BUFFER); 
    Bluefruit.Advertising.addService(KBLinkService);  /// Advertizing Keyboard Link Service
#endif

    // There is no other service for Central 
    // ToDo: Consider Configuration Service... Save config to board, reset to default values, go to DFU, etc...

    // There is probably not enough room for the dev name in the advertising packet. Putting it in the ScanResponse Packet
    Bluefruit.ScanResponse.addName();

    /* Start Advertising
     * - Enable auto advertising if disconnected
     * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
     * - Timeout for fast mode is 30 seconds
     * - Start(timeout) with timeout = 0 will advertise forever (until connected)
     * 
     * For recommended advertising interval
     * https://developer.apple.com/library/content/qa/qa1931/_index.html   
     */

    Bluefruit.Advertising.restartOnDisconnect(true);
    Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
    Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
    Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}



/**************************************************************************************************************************/
// This callback is called when a Notification update even occurs (This occurs on the client)
/**************************************************************************************************************************/
#if KEYBOARD_MODE == HUB
void notify_callback(BLEClientCharacteristic* chr, uint8_t* data, uint16_t len)
{
    LOG_LV1("CB NOT","notify_callback: Length %i data[0] %i" ,len, data[0]);
    if (len > 0)  // check if there really is data...
    {
        if (chr->uuid == KBLinkClientChar_Layers.uuid){
            LOG_LV1("CB NOT","notify_callback: Layers Data");
            //Layer is only a single uint8, dereference the pointer
            //to get the layer information
            Keyboard::updateRemoteLayer(*data); 
        }

        if (chr->uuid == KBLinkClientChar_Buffer.uuid){
            LOG_LV1("CB NOT","notify_callback: Buffer Data");
            std::vector<uint8_t> v { };
            v.assign(data, data+len);
            Keyboard::updateRemoteReport(v);
        }
    }
}

/**************************************************************************************************************************/
// This callback is called when the scanner finds a device. This happens on the Client/Central
/**************************************************************************************************************************/
void scan_callback(ble_gap_evt_adv_report_t* report)
{
    if ( Bluefruit.Scanner.checkReportForService(report, KBLinkClientService) )
    {
        LOG_LV1("KBLINK","KBLink service detected. Connecting ... ");
        Bluefruit.Central.connect(report);
    } 
}

/**************************************************************************************************************************/
// This callback is called when the central connects to a peripheral
/**************************************************************************************************************************/
void cent_connect_callback(uint16_t conn_handle)
{
    char peer_name[32] = { 0 };

    BLEConnection* connection = Bluefruit.Connection(conn_handle);
    connection->getPeerName(peer_name, sizeof(peer_name));
    LOG_LV1("CENTRL","Connected to %i %s",conn_handle,peer_name );

    if (KBLinkClientService.discover(conn_handle)) // validating that KBLink service is available to this connection
    {
        if (KBLinkClientChar_Layers.discover()) {
            KBLinkClientChar_Layers.enableNotify();      
        }
        if (KBLinkClientChar_Buffer.discover()) {
            KBLinkClientChar_Buffer.enableNotify();      
        }
    }
    else 
    {
        LOG_LV1("CENTRL","No KBLink Service on this connection"  );
        // disconect since we couldn't find KBLink service
        Bluefruit.disconnect(conn_handle);
    }   
}
/**************************************************************************************************************************/
// This callback is called when the central disconnects from a peripheral
/**************************************************************************************************************************/
void cent_disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
    (void) conn_handle;
    (void) reason;
    LOG_LV1("CENTRL","Disconnected"  );
    // if the half disconnects, we need to make sure that the received buffer is set to empty.
    // Layer is only a single uint8 
    Keyboard::updateRemoteLayer(0); 

    Keyboard::updateRemoteReport({ });
}
#endif /* HUB */

/**************************************************************************************************************************/
// This callback is called when a Notification subscription event occurs (This occurs on the server)
/**************************************************************************************************************************/
#ifdef KBLINK_SERVER
void cccd_callback(uint16_t conn_hdl, BLECharacteristic* chr, uint16_t cccd_value)    
{
    LOG_LV1("CBCCCD","cccd_callback: %i " ,cccd_value);

    // Check the characteristic this CCCD update is associated with in case
    // this handler is used for multiple CCCD records.
    if (chr->uuid == KBLinkChar_Layers.uuid) {
        if (chr->notifyEnabled()) {              
            LOG_LV1("CBCCCD","Layers 'Notify' enabled");
        } else {
            LOG_LV1("CBCCCD","Layers 'Notify' disabled");
        }
    }
    if (chr->uuid == KBLinkChar_Layer_Request.uuid) {
        if (chr->notifyEnabled()) {
            LOG_LV1("CBCCCD","KBLinkChar_Layer_Request 'Notify' enabled");
        } else {
            LOG_LV1("CBCCCD","KBLinkChar_Layer_Request 'Notify' disabled");
        }
    }
    if (chr->uuid == KBLinkChar_Buffer.uuid) {
        if (chr->notifyEnabled()) {
            LOG_LV1("CBCCCD","KBLinkChar_Buffer 'Notify' enabled");
        } else {
            LOG_LV1("CBCCCD","KBLinkChar_Buffer 'Notify' disabled");
        }
    }

}
/**************************************************************************************************************************/
// This callback is called layer_request when characteristic is being written to.  This occurs on the server (Peripheral)
// Called by KBLinkChar_Layer_Request
/**************************************************************************************************************************/
void layer_request_callback (uint16_t conn_hdl, BLECharacteristic* chr, uint8_t* data, uint16_t len)
{
    LOG_LV1("CB_CHR","layer_request_callback: len %i offset %i  data %i" ,len, data[0]);
    if (len>0)
    {
        // update state
        Keyboard::updateRemoteLayer(data[0]);
    }  
}
#endif

/**************************************************************************************************************************/
// This callback is called when the master connects to a slave
/**************************************************************************************************************************/

#if BLE_PERIPHERAL_COUNT != 0
void prph_connect_callback(uint16_t conn_handle)
{
    char peer_name[32] = { 0 };
    BLEConnection* connection = Bluefruit.Connection(conn_handle);
    connection->getPeerName(peer_name, sizeof(peer_name));
    LOG_LV1("PRPH","Connected to %i %s",conn_handle,peer_name  );

    // if HID then save connection handle to HID_connection handle
#if BLE_HID_COUNT != 0
    hid_conn_hdl = conn_handle;
#endif
}

/**************************************************************************************************************************/
// This callback is called when the master disconnects from a slave
/**************************************************************************************************************************/
void prph_disconnect_callback(uint16_t conn_handle, uint8_t reason)
{
    (void) conn_handle;
    (void) reason;
    LOG_LV1("PRPH", "Disconnected");

    // if HID then save connection handle to HID_connection handle
#if BLE_HID_COUNT != 0
    hid_conn_hdl = 0;
#endif
}

#endif /* BLE_PERIPHERAL_COUNT != 0 */

/**************************************************************************************************************************/
/**
 * Callback invoked when received Set LED from central.
 * Must be set previously with setKeyboardLedCallback()
 *
 * The LED bit map is as follows: (also defined by KEYBOARD_LED_* )
 *    Kana (4) | Compose (3) | ScrollLock (2) | CapsLock (1) | Numlock (0)
 */
void set_keyboard_led(uint16_t conn_handle, uint8_t led_bitmap)
{
    (void) conn_handle;
    // light up Red Led if any bits is set
    // RED LED is on P0.17 and is not being used on the standard BlueMicro
    if ( led_bitmap )
    {
        ledOn( LED_RED );
    }
    else
    {
        ledOff( LED_RED );
    }
}
/***********************************************/
/* layers are only sent if any kblink role is enabled */
#if defined(KBLINK_SERVER) || defined(KBLINK_CLIENT)
#pragma message "compiling layer comms in bluetooth"
void sendLayer(uint8_t layer)
{
    // Note that HID standard only has a buffer of 6 keys (plus modifiers)
    //#if BLE_PERIPHERAL_COUNT !=0  
#if defined(KBLINK_SERVER)
    //Peripheral->central uses the subscribe/notify mechanism
    KBLinkChar_Layers.notify8(layer);                   
#elif defined(KBLINK_CLIENT)
    LOG_LV1("MXSCAN","Sending Layer %i  %i" ,millis(),layer );
    if (KBLinkClientChar_Layer_Request.discover()) {
        uint16_t msg = KBLinkClientChar_Layer_Request.write8_resp(layer);       // Central->Peripheral uses the write mechanism
        LOG_LV1("MXSCAN","Sending Layer results  %i" ,msg);
    }
#endif
}
#endif 
/**************************************************************************************************************************/
void sendKeys(uint8_t* currentReport, int length)
{

#if BLE_HID_COUNT != 0  
#pragma message "Compiling hid send keys in bluetooth"
    uint8_t keycode[6];
    uint8_t layer = 0;
    uint8_t mods = 0;

    //modifiers
    mods = currentReport[0];

    //keycodes
    keycode[0] = currentReport[1];
    keycode[1] = currentReport[2];
    keycode[2] = currentReport[3];
    keycode[3] = currentReport[4];
    keycode[4] = currentReport[5];
    keycode[5] = currentReport[6];

    //layer
    layer = currentReport[7];
    blehid.keyboardReport(hid_conn_hdl, mods, keycode); 

    LOG_LV2("HID","Sending blehid.keyboardReport " );
#endif

#ifdef KBLINK_SERVER
    KBLinkChar_Buffer.notify(currentReport, length);
#endif
}
/**************************************************************************************************************************/
void sendRelease()
{
#if BLE_HID_COUNT != 0
    blehid.keyRelease(hid_conn_hdl);                                             // HID uses the standard blehid service
    LOG_LV2("HID","Sending blehid.keyRelease " );
#endif

#ifdef KBLINK_SERVER
    //SPOKE -> HUB uses the subscribe/notify mechanism
    //release corresponds to no data, so 
    //send a nullptr and zero length, nullptr shouldn't result 
    //in any issues, as only messages with lengths 
    //of more than 0 are processed
    KBLinkChar_Buffer.notify(nullptr, 0);                       
#endif
    //Only send layer to slaves
    //Central does not need to send the buffer to the Peripheral.
}
