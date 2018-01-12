#include <types.h>
#include <xbee_config.h>
#include <xbee_cmd_callback.h>

int read_console_commands(){
	uint8_t option;

  /* Interactive menu for action control */
  if(uart_bytes_in_rx_buffer() > 0) {
    uart_read(&option, 1);
    printf("Got option: %u\n", option);

		if(option == 49 || option == 50 || option == 51) {
		  int16_t request;
			switch (option) {
				case 49:
        	printf("Requesting Operating PAN ID (OP)\n");
          request = xbee_cmd_create(&xdev, "OP");
					break;
				case 50:
				  printf("Requesting Operating 16-BIT PAN (OI)\n");
          request = xbee_cmd_create(&xdev, "OI");
					break;
				case 51:
  				printf("Requesting Operating PAN ID (MY)\n");
          request = xbee_cmd_create(&xdev, "MY");
					break;
			}
			if (request < 0) {
      	printf( "Error creating request: %d (%" PRIsFAR ") \n", request, strerror( -request));
    	} else  {
      	xbee_cmd_set_callback(request, xbee_cmd_callback, NULL);
      	xbee_cmd_send(request);
    	}
		}
    else if(option == 115) { /* s */
      // Additionnal XBee settings
      printf("\n Setting additional radio settings: ZS ");
      xbee_cmd_simple(&xdev, "ZS", XBEE_PARAM_ZS);
      printf("NJ ");
      xbee_cmd_simple(&xdev, "NJ", XBEE_PARAM_NJ);
      printf("NH ");
      xbee_cmd_simple(&xdev, "NH", XBEE_PARAM_NH);
      printf("NO ");
      xbee_cmd_simple(&xdev, "NO", XBEE_PARAM_NO);
      printf("EE ");
      xbee_cmd_simple(&xdev, "EE", XBEE_PARAM_EE);
      printf("EO ");
      xbee_cmd_simple(&xdev, "EO", XBEE_PARAM_EO);
      printf("AP ");
      xbee_cmd_simple(&xdev, "AP", XBEE_PARAM_AP);
      printf("KY ");
      xbee_cmd_execute(&xdev, "KY", XBEE_PARAM_KY, (sizeof(XBEE_PARAM_KY) - 1) / sizeof(char));
      printf("WR ");
      xbee_cmd_execute(&xdev, "WR", NULL, 0);
      printf("Done!\n\n");
    }
    else if(option == 114) { /* r */
      printf("Resetting Network\n");
      xbee_cmd_simple(&xdev, "NR", 0);
    }
    else if(option == 52) { /* 4 */
      printf("Setting AO to 1\n");
      xbee_cmd_simple(&xdev, "AO", 1);
			xbee_cmd_execute(&xdev, "WR", NULL, 0);
		}
    else if(option == 53) { /* 5 */
      printf("Setting AO to 3\n");
      xbee_cmd_simple(&xdev, "AO", 3);
      xbee_cmd_execute(&xdev, "WR", NULL, 0);
    }
    else if(option == 98 ) { /* b */
      sys_app_banner();
    }
    else {
      puts("-------------------------------------");
      puts("|              H E L P              |");
      puts("-------------------------------------");
      puts("[1] - Print Operating PAN ID");
      puts("[2] - Print Operating PAN (16-BIT) ID");
      puts("[3] - Print PAN Network Address");
      puts("[4] - Set AO to 1");
      puts("[5] - Set AO to 3");
      puts("[s] - Init additional radio settings");
      puts("[b] - Print Banner");
      puts("[r] - Local network reset");
      puts("");
    }
    printf("> ");
  }
  /* End of interactive menu */
}
