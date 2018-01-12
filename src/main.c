#include <xbee_config.h>
#include <types.h>
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

int rx_cluster_callback ( const xbee_cmd_response_t FAR *response);

const wpan_cluster_table_entry_t custom_ep_clusters[] = {
    {CUSTOM_NULL_CLUSTER, NULL, NULL, WPAN_CLUST_FLAG_INPUT},
    {CUSTOM_SENSOR_CLUSTER, rx_cluster_callback, NULL, WPAN_CLUST_FLAG_INPUT},
    WPAN_CLUST_ENTRY_LIST_END
};

void main(void)
{
	sys_hw_init();
	sys_xbee_init();
	sys_app_banner();

	printf("> ");

  for (;;) {
		read_console_commands();

		sys_watchdog_reset();
		sys_xbee_tick();
	}
}
