#include <xbee_config.h>
#include <types.h>
#include <xbee_cmd_callback.h>
// #include <rx_cluster_callback.h>
// #include <default_cluster_callback.h>

#ifdef ENABLE_XBEE_HANDLE_ND_RESPONSE_FRAMES
void node_discovery_callback(xbee_dev_t *xbee, const xbee_node_id_t *node_id)
{
	puts("Received ND Frame");
	/* This function is called every time a node is discovered, either by
	 * receiving a NodeID message or because a node search was started with
	 * function xbee_disc_discover_nodes() */
	return;
}
#endif

#ifdef ENABLE_XBEE_HANDLE_RX
int xbee_transparent_rx(const wpan_envelope_t FAR *envelope, void FAR *context)
{
	char addrbuf[ADDR64_STRING_LENGTH];

	puts("Received Simple Frame");
	puts("---------------------");
	sys_watchdog_reset();
	printf("Source     : %s\n", addr64_format(addrbuf, &envelope->ieee_address));
	printf("Network    : %04x\n", be16toh(envelope->network_address));
	printf("Data length: %u\n", envelope->length);
	sys_watchdog_reset();
	dump(envelope->payload, envelope->length);
	puts("\n");

	return 0;
}
#endif

// Custom profile and cluster implementation
#define CUSTOM_NULL_CLUSTER    0x0000
#define CUSTOM_SENSOR_CLUSTER  0x0006

#if defined(RTC_ENABLE_PERIODIC_TASK)
void rtc_periodic_task(void)
{
	// printf("Sensor is: %d\n", !gpio_get(SENSOR));
	gpio_set(LED, !gpio_get(SENSOR));
}
#endif

zcl_command_t zcl;
int rx_cluster_callback ( const xbee_cmd_response_t FAR *response);

const wpan_cluster_table_entry_t custom_ep_clusters[] = {
    {CUSTOM_NULL_CLUSTER, NULL, NULL, WPAN_CLUST_FLAG_INPUT},
    {CUSTOM_SENSOR_CLUSTER, rx_cluster_callback, NULL, WPAN_CLUST_FLAG_INPUT},
    WPAN_CLUST_ENTRY_LIST_END
};

int rx_cluster_callback(const wpan_envelope_t FAR *envelope, void FAR *context)
{
  uint8_t                 *start_response;
  uint8_t									*end_response;
  PACKED_STRUCT {
    zcl_header_response_t	header;
    uint8_t								buffer[20];
  } response;

  printf("\nCUSTOM ENDPOINT HANDLER\n");
  printf("=======================\n");
  printf("\n\nBuilding ZCL Command based on received envelope: ");
  if(zcl_command_build(&zcl, envelope, context) == 0) {
    printf("OK!\n");
    printf("----------------------\n");
    printf("Frame Control: %02X\n", zcl.frame_control);
    printf("Command: %02X\n", zcl.command);
    printf("ZCL Payload length: %02X\n", zcl.length);
    hex_dump(zcl.zcl_payload, zcl.length, HEX_DUMP_FLAG_TAB);
    printf("----------------------\n");
    zcl_envelope_payload_dump(envelope);
    // Handle all the commands

    if(ZCL_CMD_MATCH(&zcl.frame_control, GENERAL, CLIENT_TO_SERVER, PROFILE)) {
      if(zcl.command == ZCL_CMD_READ_ATTRIB) {
        // Profile command received (Read attribute) for switch state
        printf("Handling response for switch state read\n");

        response.header.command = ZCL_CMD_READ_ATTRIB_RESP;
        start_response = (uint8_t *)&response + zcl_build_header(&response.header, &zcl);
        end_response = response.buffer;

        *end_response++ = 0x00;
        *end_response++ = 0x00;
        *end_response++ = ZCL_STATUS_SUCCESS;
        *end_response++ = ZCL_TYPE_LOGICAL_BOOLEAN;
        if (gpio_get(SENSOR)) {
          *end_response++ = 0x01;
        }else{
          *end_response++ = 0x00;
        }

        printf("Response length: %02X\n", end_response - start_response);
        if(zcl_send_response(&zcl, start_response, end_response - start_response) == 0) {
          printf("Response sent successfully\n");
        }
      }
      else if(zcl.command == ZCL_CMD_CONFIGURE_REPORT) {
        // Profile command received (Configure report) for switch state
        printf("Handling response for configure report command\n");

        response.header.command = ZCL_CMD_CONFIGURE_REPORT_RESP;
        start_response = (uint8_t *)&response + zcl_build_header(&response.header, &zcl);
        end_response = response.buffer;

        *end_response++ = ZCL_STATUS_SUCCESS;
        *end_response++ = ZCL_DIRECTION_SEND;
        *end_response++ = 0x00;
        *end_response++ = 0x00;

        printf("Response length: %02X\n", end_response - start_response);
        if(zcl_send_response(&zcl, start_response, end_response - start_response) == 0) {
          printf("Response sent successfully\n");
        }
      }
      else {
        printf("Unhandled Profile command received\n");
      }
    }
  }
  else {
    printf("Error!\n");
  }

  return 0;
}
// END: Custom profile and cluster implementation

int default_cluster_callback(const wpan_envelope_t FAR *envelope, void FAR *context)
{
  uint8_t *payload_pointer = envelope->payload;
  int i = 0;

  payload_pointer[envelope->length] = '\0'; /* Add Null-terminator for printing */
  printf("\nCUSTOM ENDPOINT'S DEFAULT CLUSTER HANDLER\n");
  printf("Received  : %s\n", payload_pointer);
  if(envelope->length > 0) {
    printf("Payload in Hex: ");
    for(i = 0; i < envelope->length; i++) {
      printf("%02X ", payload_pointer[i]);
    }
    printf("\n");
  }
  printf("Payload length: %02X\n", envelope->length);
  printf("Cluster ID:     %02X\n", envelope->cluster_id);
  printf("Profile ID:     %02X\n", envelope->profile_id);
  printf("Destin. EP:     %02X\n", envelope->dest_endpoint);
  printf("Source  EP:     %02X\n", envelope->source_endpoint);

  /* Let's try to debug the ZCL command */
  zcl_envelope_payload_dump(envelope);

  /* Let's try to respond */
  printf("\n\nBuilding ZCL Command based on received envelope: ");
  if(zcl_command_build(&zcl, envelope, context) == 0) {
    printf("OK!\n");
    printf("----------------------\n");
    printf("Frame Control: %02X\n", zcl.frame_control);
    printf("Command: %02X\n", zcl.command);
    printf("ZCL Payload length: %02X\n", zcl.length);
    hex_dump(zcl.zcl_payload, zcl.length, HEX_DUMP_FLAG_TAB);

    if(ZCL_CMD_MATCH( &zcl.frame_control, GENERAL, CLIENT_TO_SERVER, PROFILE)) {
      if(zcl.command == ZCL_CMD_READ_ATTRIB && zcl.length == 2) {
        const char FAR *zclPayload = zcl.zcl_payload;
        uint8_t                 *start_response;
        uint8_t									*end_response;
        PACKED_STRUCT {
          zcl_header_response_t	header;
          uint8_t								buffer[20];
        } response;

        response.header.command = ZCL_CMD_READ_ATTRIB_RESP;
        start_response = (uint8_t *)&response + zcl_build_header(&response.header, &zcl);
        end_response = response.buffer;

        if(zclPayload[0] == 0x01 && zclPayload[1] == 0x00) {
          /* Application version request */
          printf("Handling response for Application version request\n");
          *end_response++ = 0x01;
          *end_response++ = 0x00;
          *end_response++ = ZCL_STATUS_SUCCESS;
          *end_response++ = 0x20;
          *end_response++ = 0x00;

          printf("Response length: %02X\n", end_response - start_response);
          if(zcl_send_response(&zcl, start_response, end_response - start_response) == 0) {
            printf("Response sent successfully");
          }
        }
        else if(zclPayload[0] == 0x04 && zclPayload[1] == 0x00) {
          /* Device manufacturer request */
          printf("Handling response for Device manufacturer request\n");
          *end_response++ = 0x04;
          *end_response++ = 0x00;
          *end_response++ = ZCL_STATUS_SUCCESS;
          *end_response++ = 0x42;
          *end_response++ = 0x05; // Length of data
          *end_response++ = 'R';
          *end_response++ = 'o';
          *end_response++ = 'o';
          *end_response++ = 't';

          printf("Response length: %02X\n", end_response - start_response);
          if(zcl_send_response(&zcl, start_response, end_response - start_response) == 0) {
            printf("Response sent successfully");
          }
        }
        else if(zclPayload[0] == 0x05 && zclPayload[1] == 0x00) {
          /* Device developer request */
          printf("Handling response for Device developer request\n");
          *end_response++ = 0x05;
          *end_response++ = 0x00;
          *end_response++ = ZCL_STATUS_SUCCESS;
          *end_response++ = 0x42;
          *end_response++ = 0x07; // Length of data
          *end_response++ = 'O';
          *end_response++ = 'c';
          *end_response++ = 'c';
          *end_response++ = 'u';
          *end_response++ = 'p';
          *end_response++ = 'i';
          *end_response++ = 'e';
          *end_response++ = 'd';

          printf("Response length: %02X\n", end_response - start_response);
          if(zcl_send_response(&zcl, start_response, end_response - start_response) == 0) {
            printf("Response sent successfully\n");
          }
        }
      }
    }
  }
  else {
    printf("Error!\n");
  }

  printf("=====================================================================\n");

  return 0;
}

void main(void)
{
	uint8_t option;

	sys_hw_init();
	sys_xbee_init();
	sys_app_banner();

	printf("> ");

  for (;;) {

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
        printf("AO ");
        xbee_cmd_simple(&xdev, "AO", XBEE_PARAM_AO);
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
        printf("Setting AP to 1\n");
        xbee_cmd_simple(&xdev, "AO", 1);
				xbee_cmd_execute(&xdev, "WR", NULL, 0);
			}
      else if(option == 53) { /* 5 */
        printf("Setting AP to 3\n");
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

		sys_watchdog_reset();
		sys_xbee_tick();
	}
}
