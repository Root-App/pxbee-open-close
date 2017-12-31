
  o-------------------------------o
  | Transmit basic frames Example |
  o-------------------------------o

  Compatible platforms
  --------------------
  * XBee ZB (S2B) 32KB
  * XBee ZB (S2C) 32KB (current)
  * XBee ZB (S2B) 128KB
  * XBee ZB (S2C) 128KB
  * XBee ZB (S2CTH) 32KB

  Introduction
  ------------
  This example generates a frames with the "Hello coordinator - C" text 
  (where 'C' is a counter) and sends it to the coordinator of the XBee network 
  once per second.
  
  One of the most basic operations that can be performed with the XBee 
  layer, is to transmit messages to other XBee devices. In this example 
  you will learn how to create and send a simple XBee frame to the 
  coordinator of the XBee network.
  
  To learn more on how to communicate with non-Digi devices, try the
  'Transmit Explicit Frames', 'Adding a cluster handler to Digi's endpoint' and
  'Adding a custom Endpoint' examples. Notice that those are advanced examples
  and are not necessary for simple applications.

  Requirements
  ------------
  To run this example you will need:
    * One X-Stick running as coordinator.
    * The X-CTU software application.
    * One programmable XBee module to host the application.
    * One PE-Micro debugger to debug the application.
    * One XBee Interface Board.
    * A serial console terminal to view the application output.

  Example setup
  -------------
  1) Make sure the hardware is set up correctly:
       a) The XBee module into the headers on the XBee USB interface board.
       b) The debugger is connected to the interface board by its ribbon 
          cable.
       c) Both the XBIB device and the debugger are connected to your PC via 
          USB.
     
  2) Make sure the X-Stick is connected to one of the USB ports of your PC. 
     The Programmable XBee Development Kit includes an X-Stick device already 
     flashed with coordinator firmware. It will be used in this example to 
     receive the frames sent by the XBee module.
	 Note: if you have manually flashed the X-Stick, ensure it has coordinator
	 API firmware.
	 
  3) Check XBee module firmware:
       - Programmable XBee S2B modules:
  			Make sure the XBee module has router firmware. If it does not, 
  			you should re-flash it with the following firmware:
 	          	XB24-ZB  - ZIGBEE ROUTER API - 23A7
     
       - Programmable XBee S2C modules:
			S2C modules have a single firmware for coordinator, router and end 
			device functionality; so following firmware suits all applications:
          		XB24C   - ZIGBEE            - 401E

       - Programmable XBee S2CTH modules:
			S2CTH modules have a single firmware for coordinator, router and end 
			device functionality; so following firmware suits all applications:
          		XB24C   - ZIGBEE TH PRO     - 4055
     
     Note: Router or End Device firmware would work for this example, but we 
     recommend you use router firmware because routers don't go to sleep.
     
     To flash a new firmware into an XBee module, you need to use the X-CTU 
     software and follow the instructions found in the topic "Re-flashing the 
     XBee firmware" of the "XBee Extensions User's Guide."
     
  4) Make sure the X-Stick and XBee module have the same PAN ID so the XBee 
     module will join the network started by the coordinator (X-Stick). If 
     they don't, you can change the XBee module's PAN ID using one of the 
     following methods:
       a) With the X-CTU software. Refer to the topic "Changing XBee firmware 
          settings" of the "XBee Extensions User's Guide" for more information.
       b) With the "Set PAN ID to" setting of the "XBee configuration 
          component" (in the XBee Project Smart Editor).
     
  5) This demo also requires a serial console terminal in order to see the 
     frames sent by the XBee module. You can use the embedded terminal 
     included with the XBee extensions or any other serial terminal 
     application.
     
     Configure the terminal and open a serial connection with the XBee module.
     
     The baud rate of the serial console must be 115200, as the application 
     will configure the UART of the XBee module with that baud rate.
     
     Refer to the topic "Terminal" of the "XBee Extensions User's Guide" for 
     more information.
     
  6) Finally, the X-CTU software will be used to see the XBee frames received 
     by the X-Stick (which have been sent by the XBee module). Execute the 
     application and configure it to use the serial port where the X-Stick 
     is connected. 
     
     Remember to configure the baud rate (9600 by default) and to check the 
     "Enable API" option to communicate with the X-Stick correctly. You can 
     verify if the configuration is correct by pressing the "Test / Query" 
     button.

  Running the example
  -------------------
  The example is already configured, so all you need to do is to compile and 
  launch the application.
  
  While it is running, you will see the frames sent by the XBee module 
  through the serial console terminal.
  
  Verify that the X-Stick is receiving the frames by accessing the "Terminal" 
  tab of the X-CTU. If the frames are successfully sent, they will be 
  displayed there.
