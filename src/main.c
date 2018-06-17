#include <xbee_config.h>
#include <types.h>
#include <rx_cluster_callback.h>

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
#define DEFAULT_PROFILE_ID			WPAN_PROFILE_DIGI
#define DEFAULT_CLUSTER_ID			DIGI_CLUST_SERIAL

wpan_envelope_t envelope;
char tx_buffer[5] = "";

#if defined(RTC_ENABLE_PERIODIC_TASK)
void rtc_periodic_task(void)
{

	if(gpio_get(LED) == gpio_get(SENSOR)) {
		gpio_set(LED, !gpio_get(SENSOR));

		wpan_envelope_create(&envelope, &xdev.wpan_dev, WPAN_IEEE_ADDR_COORDINATOR,	WPAN_NET_ADDR_UNDEFINED);

		tx_buffer[0] = 0x00;
		tx_buffer[1] = 0x00;
		tx_buffer[2] = ZCL_STATUS_SUCCESS;
		tx_buffer[3] = ZCL_TYPE_LOGICAL_BOOLEAN;
		if (gpio_get(SENSOR)) {
			tx_buffer[4] = 0x01;
		}else{
			tx_buffer[4] = 0x00;
		}
		envelope.payload = tx_buffer;
		envelope.options = 0x00;

		envelope.dest_endpoint = 0x01;
		envelope.source_endpoint = CUSTOM_ENDPOINT;
		envelope.profile_id = CUSTOM_EP_PROFILE;
		envelope.cluster_id = CUSTOM_SENSOR_CLUSTER;

		envelope.length = 5;

		wpan_envelope_send(&envelope);
	}
}
#endif

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

	gpio_set(LED, !gpio_get(SENSOR));

  for (;;) {
		read_console_commands();

		sys_watchdog_reset();
		sys_xbee_tick();
	}
}
