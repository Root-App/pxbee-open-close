/***** XBEE APPLICATION PROJECT *****
 * 
 * Auto-generated header with information about the 
 * relation between the XBee module pins and the 
 * project components.
 * 
 ************ XBEE LAYOUT ***********
 * 
 * This layout represents the XBee ZB (S2C) module 
 * selected for the project with its pin distribution:
 *            ________________________
 *           |                        |- XPIN37
 *           |                ____    |- XPIN36
 *           |              //    \\  |- XPIN35
 *   XPIN1  -|             ||      || |- XPIN34
 *   XPIN2  -|              \\____//  |- XPIN33
 *   XPIN3  -|                        |- XPIN32
 *   XPIN4  -| =====================  |- XPIN31
 *   XPIN5  -| #   # ####  #### ####  |- XPIN30
 *   XPIN6  -|  # #  #   # #    #     |- XPIN29
 *   XPIN7  -|   #   ####  ###  ###   |- XPIN28
 *   XPIN8  -|  # #  #   # #    #     |- XPIN27
 *   XPIN9  -| #   # ####  #### ####  |- XPIN26
 *   XPIN10 -| =====================  |- XPIN25
 *   XPIN11 -|                        |- XPIN24
 *   XPIN12 -|                        |- XPIN23
 *   XPIN13 -|                        |- XPIN22
 *           |________________________|
 *             |  |  |  |  |  |  |  |
 *             |  |  |  |  |  |  |  XPIN21
 *             |  |  |  |  |  |  XPIN20
 *             |  |  |  |  |  XPIN19
 *             |  |  |  |  XPIN18
 *             |  |  |  XPIN17
 *             |  |  XPIN16
 *             |  XPIN15
 *             XPIN14
 * 
 ************ PINS LEGEND ***********
 * 
 * The following list displays all the XBee Module pins 
 * with the project component which is using each one:
 * 
 *   XPIN1 = GND
 *   XPIN2 = VCC
 *   XPIN3 = uart0 [TX Pin]
 *   XPIN4 = uart0 [RX Pin]
 *   XPIN5 = <<UNUSED>>
 *   XPIN6 = special0 [Reset Pin]
 *   XPIN7 = special0 [RSSI PWM Pin]
 *   XPIN8 = <<UNUSED>>
 *   XPIN9 = special0 [BKGD Pin]
 *   XPIN10 = <<UNUSED>>
 *   XPIN11 = GND
 *   XPIN12 = <<UNUSED>>
 *   XPIN13 = GND
 *   XPIN14 = <<UNUSED>>
 *   XPIN15 = <<UNUSED>>
 *   XPIN16 = <<UNUSED>>
 *   XPIN17 = <<UNUSED>>
 *   XPIN18 = <<UNUSED>>
 *   XPIN19 = <<UNUSED>>
 *   XPIN20 = <<UNUSED>>
 *   XPIN21 = Do not Connect
 *   XPIN22 = GND
 *   XPIN23 = Do not Connect
 *   XPIN24 = <<UNUSED>>
 *   XPIN25 = <<UNUSED>>
 *   XPIN26 = <<UNUSED>>
 *   XPIN27 = VCC REF
 *   XPIN28 = special0 [Association Pin]
 *   XPIN29 = <<UNUSED>>
 *   XPIN30 = <<UNUSED>>
 *   XPIN31 = <<UNUSED>>
 *   XPIN32 = <<UNUSED>>
 *   XPIN33 = special0 [Commissioning Pin]
 *   XPIN34 = Do not Connect
 *   XPIN35 = GND
 *   XPIN36 = <<UNUSED>>
 *   XPIN37 = Do not Connect
 *
 ************************************/


#include <xbee_config.h>
#include <types.h>

#define CUSTOM_EP_CUSTOM_CLUSTER	0x23

int custom_ep_rx_cluster(const wpan_envelope_t FAR *envelope, void FAR *context);

const wpan_cluster_table_entry_t custom_ep_clusters[] = {
		{CUSTOM_EP_CUSTOM_CLUSTER, custom_ep_rx_cluster, NULL,
		WPAN_CLUST_FLAG_INOUT | WPAN_CLUST_FLAG_NOT_ZCL},
		WPAN_CLUST_ENTRY_LIST_END
};


int custom_ep_default_cluster(const wpan_envelope_t FAR *envelope, void FAR *context)
{
	printf("\nCUSTOM ENDPOINT'S DEFAULT CLUSTER HANDLER\n");
	printf("Received  : %.*s\n", envelope->length, envelope->payload);
	printf("Cluster ID: %02X\n", envelope->cluster_id);
	printf("Profile ID: %02X\n", envelope->profile_id);
	printf("Destin. EP: %02X\n", envelope->dest_endpoint);
	printf("Source  EP: %02X\n", envelope->source_endpoint);
	
	return 0;
}

int custom_ep_rx_cluster(const wpan_envelope_t FAR *envelope, void FAR *context)
{
	printf("\nCUSTOM ENDPOINT'S RX CLUSTER HANDLER\n");
	printf("Received  : %.*s\n", envelope->length, envelope->payload);

	return 0;
}

#ifdef ENABLE_XBEE_HANDLE_RX
int xbee_transparent_rx(const wpan_envelope_t FAR *envelope, void FAR *context)
{
	printf("\nDIGI SERIAL CLUSTER HANDLER\n");
	printf("Received  : %.*s\n", envelope->length, envelope->payload);

	return 0;
}
#endif

void main(void)
{
	sys_hw_init();
	sys_xbee_init();
	sys_app_banner();
	
	printf("Send explicit frames endpoint 0x%02X, Profile ID 0x%02X, \n"
		   "cluster 0x%02X of for Digi Serial Cluster callback.\n\n"
		   "Send explicit frames to endpoint 0x%02X, Profile ID 0x%02X,\n"
		   "cluster 0x%02X for Custom EndPoint's Cluster callback, or to\n"
		   "other cluster to test default handler.",
		   WPAN_ENDPOINT_DIGI_DATA, WPAN_PROFILE_DIGI, DIGI_CLUST_SERIAL,
		   CUSTOM_ENDPOINT, CUSTOM_EP_PROFILE, CUSTOM_EP_CUSTOM_CLUSTER);
	
	for (;;) {
		/* Nothing to do...
		 * Everything is done in frame handlers
		 */
		sys_watchdog_reset();
		sys_xbee_tick();
	}
}
