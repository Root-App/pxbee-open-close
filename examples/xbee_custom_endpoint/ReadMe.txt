
  o----------------------------------o
  | Adding a custom endpoint Example |
  o----------------------------------o

  Compatible platforms
  --------------------
  * XBee ZB (S2B) 32KB
  * XBee ZB (S2C) 32KB (current)
  * XBee ZB (S2B) 128KB
  * XBee ZB (S2C) 128KB
  * XBee ZB (S2CTH) 32KB

  Introduction
  ------------
  This example shows how to use the ZigBee communication
  layer to handle the reception of data sent to an specific endpoint which can have 
  one or more clusters that will be implemented by the user. Also, it shows how to add
  a "default cluster handler" which is called when a message for a cluster which has not
  a defined handler arrives to this endpoint.
  
  To understand this example you have to check the macros defined at custom.h
  header file. It is done this way so the library is not modified, as endpoints.c and 
  endpoints.h are prepared to handle these macros.
  
  Implementing an endpoint is useful when communicating with non-Digi
  devices, with Public profiles as SmartEnergy or Home Automation or even with a 
  custom device.
  
  
  The use of clusters is useful when implementing custom protocols or new 
  functionalities. For example, a user could write code to perform actions only when
  data is received in specific clusters instead of parsing commands in a single callback.

  Requirements
  ------------
  To run this example you will need:
    * One X-Stick running as coordinator.
    * One PE-Micro debugger to debug the application.
    * One XBee Interface Board.
    * The X-CTU software application.
    * One programmable XBee module to host the application.
    * A serial console terminal to view the application output.
	* OPTIONAL: Another programmable XBee module running 'Transmit Explicit Frames'
      example. With this application you can send data to any cluster and endpoint
	  and see how this example handles them.

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
     send XBee frames to the XBee module. The X-Stick will be the coordinator 
     of the XBee network.
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
     XBee frames information received by the XBee module. You can use the 
     embedded terminal included with the XBee extensions or any other serial 
     terminal application.
     
     Configure the terminal and open a serial connection with the XBee module.
     
     The baud rate of the serial console must be 115200, as the application 
     will configure the UART of the XBee module with that baud rate.
     
     For more information, please refer to the topic "Terminal" of the "XBee 
     Extensions User's Guide" for more information.
     
  6) Finally, the X-CTU software will be used to transmit XBee frames over 
     the XBee network generated by the X-Stick. Launch X-CTU and configure
	 it to use the serial port where the X-Stick is connected.
     
     Remember to configure the baud rate (9600 by default) and to check the 
     "Enable API" option to communicate with the X-Stick correctly. You can 
     verify the connection by pressing the "Test / Query" button.

  Running the example
  -------------------
  The example is already configured, so all you need to do is to compile and 
  launch the application.
  
  While it is running, go to the "Terminal" tab of the X-CTU. Press the 
  "Assemble Packet" button to display the "Send Packet" dialog. Configure the 
  display to "HEX" and copy the following frame in the text box:
  
    * 7E 00 39 11 01 00 00 00 00 00 00 FF FF FF FE E8 E8 00 11 C1 05 00 00 54
      68 69 73 20 66 72 61 6D 65 20 69 73 20 66 6F 72 20 44 69 67 69 20 53 65 
	  72 69 61 6C 20 43 6C 75 73 74 65 72 06
	
	Note: This is a Brodcast frame for Cluster ID: 11, Sourc EP: E8, Destination EP: E8,
	Profile ID: C105.
	
  Press the "Send" button to send the frame ("This frame is for Digi Serial Cluster")
  to the XBee.
  
  Verify that the XBee module has received the frame on Digi Serial Cluster by
  checking the serial console terminal you configured previously. If it was successful,
  you should view the following message:
		DIGI SERIAL CLUSTER HANDLER
		Received  : This frame is for Digi Serial Cluster

  Go back to X-CTU and press "Clear" button in Send Packet dialog. Ensure that the
  display is set to "HEX" and copy the following frame in the test box:
	* 7E 00 3E 11 01 00 00 00 00 00 00 FF FF FF FE E8 13 00 23 5E BA 00 00 54 
	  68 69 73 20 66 72 61 6D 65 20 69 73 20 66 6F 72 20 65 6E 64 70 6F 69 6E 
	  74 20 31 33 20 2D 20 43 6C 75 73 74 65 72 20 32 33 BD
	 
	 Note: This is a Brodcast frame for Cluster ID: 23, Sourc EP: E8, Destination EP: 13,
	 Profile ID: 5EBA.

  Press the "Send" button to send the frame ("This frame is for endpoint 13 - Cluster 23")
  to the XBee.
  
  Check the Programmable XBee serial console terminal to see if it received the message. If
  it was successful, you should view the following message:
		CUSTOM ENDPOINT'S RX CLUSTER HANDLER
		Received  : This frame is for End Point 13 - Cluster 23
		
  Finally, press on Clear again and paste the following frame in the test box:
	* 7E 00 3E 11 01 00 00 00 00 00 00 FF FF FF FE E8 13 00 11 5E BA 00 00 54 
	  68 69 73 20 66 72 61 6D 65 20 69 73 20 66 6F 72 20 65 6E 64 70 6F 69 6E 
	  74 20 31 33 20 2D 20 43 6C 75 73 74 65 72 20 31 31 D2 
	  
	  Note: This is a Brodcast frame for Cluster ID: 0F, Sourc EP: E8, Destination EP: E8,
	  Profile ID: 5EBA.
  Press the "Send" button to send the frame ("This frame is for endpoint 13 - Cluster 11")
  to the XBee.
  Check the Programmable XBee serial console terminal to see if it received the message. If
  it was successful, you should view the following message:
		CUSTOM ENDPOINT'S DEFAULT CLUSTER HANDLER
		Received  : This frame is for endpoint 13 - Cluster 11
		Cluster ID: 11
		Profile ID: 5EBA
		Destin. EP: 13
		Source  EP: E8
		
  If you have the "Transmit explicit frames" application running in the same network, you can
  test sending explicit frames to Custom Endpoint (13) and Digi Endpoint (E8), check that 
  sending to other endpoints will have no effect, also check that Digi EP will handle only
  messages sent to Serial Cluster (11) while Custom endpoint will handle messages sent to
  cluster 23 with one handler and all other clusters with the default handler.
  
  Another way of testing it is by generating Transmit Explicit Frames (Frame type 0x11) either
  manually or with a frame builder like the one you can find in the following URL:
	ftp://ftp1.digi.com/support/utilities/digi_apiframes.htm
