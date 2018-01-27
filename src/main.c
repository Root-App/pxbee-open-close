#include <xbee_config.h>
#include <types.h>
#include <xbee_cmd_callback.h>
#include <rx_cluster_callback.h>
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


const wpan_cluster_table_entry_t custom_ep_clusters[] = {
    {CUSTOM_NULL_CLUSTER, NULL, NULL, WPAN_CLUST_FLAG_INPUT},
    {CUSTOM_SENSOR_CLUSTER, rx_cluster_callback, NULL, WPAN_CLUST_FLAG_INPUT},
    WPAN_CLUST_ENTRY_LIST_END
};

void main(void)
{
	uint8_t option;

	sys_hw_init();
	sys_xbee_init();
	sys_app_banner();

  gpio_config(SENSOR, GPIO_CFG_PULL_UP_DIS);

	printf("> ");

  for (;;) {

		/* Interactive menu for action control */
    if(uart_bytes_in_rx_buffer() > 0) {
      uart_read(&option, 1);
      printf("Got char: %u\n", option);
      if(option == 49) { /* 1 */
        int16_t request;

        printf("\nRequesting Operating PAN ID (OP)\n");
        request = xbee_cmd_create(&xdev, "OP");
        if (request < 0) {
		      printf( "Error creating request: %d (%" PRIsFAR ") \n", request, strerror( -request));
	      }
	      else  {
	        xbee_cmd_set_callback(request, xbee_cmd_callback, NULL);
	        xbee_cmd_send(request);
	      }
      }
      else if(option == 50) { /* 2 */
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
      else if(option == 48) { /* 0 */
        printf("\n Resetting Network\n");
        xbee_cmd_simple(&xdev, "NR", 0);
      }
      else if(option == 98 || option == 66) { /* b || B */
        sys_app_banner();
      }
      else if(option == 104 || option == 72) { /* h || H */

      }
      else {
        puts("-------------------------------------");
        puts("|              H E L P              |");
        puts("-------------------------------------");
        puts("[1] - Print Operating PAN ID");
        puts("[2] - Init additional radio settings");
        puts("[0] - Local network reset");
        puts("");
      }
      printf("> ");
    }
    /* End of interactive menu */

		sys_watchdog_reset();
		sys_xbee_tick();
	}
}
