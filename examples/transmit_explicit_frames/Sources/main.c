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

#define MAX_PAYLOAD_LENGTH			150
#define DEFAULT_DEST_EP				WPAN_ENDPOINT_DIGI_DATA
#define DEFAULT_SOURCE_EP			WPAN_ENDPOINT_DIGI_DATA
#define DEFAULT_PROFILE_ID			WPAN_PROFILE_DIGI
#define DEFAULT_CLUSTER_ID			DIGI_CLUST_SERIAL
#define DEFAULT_PAYLOAD				"Default payload"

wpan_envelope_t envelope;
char tx_buffer[MAX_PAYLOAD_LENGTH] = DEFAULT_PAYLOAD;
char addr_buf[ADDR64_STRING_LENGTH];

void main(void)
{
	int endpoint;

	sys_hw_init();
	sys_xbee_init();
	sys_app_banner();
	
	wpan_envelope_create(&envelope, &xdev.wpan_dev, WPAN_IEEE_ADDR_COORDINATOR,
													WPAN_NET_ADDR_UNDEFINED);

	envelope.payload = tx_buffer;
	envelope.options = 0x00;
	
	envelope.dest_endpoint = DEFAULT_DEST_EP;
	envelope.source_endpoint = DEFAULT_SOURCE_EP;
	envelope.profile_id = DEFAULT_PROFILE_ID;
	envelope.cluster_id = DEFAULT_CLUSTER_ID;
		
	for (;;) {
		printf("\nEnter destination MAC address (XX-XX-XX-XX-XX-XX-XX-XX): ");
		scanf("%s", addr_buf);
		puts("\n");
		addr64_parse(&envelope.ieee_address, addr_buf);
		addr64_format(addr_buf, &envelope.ieee_address);
		printf("Dest: %s\n", addr_buf);

		printf("\nEnter payload: ");
		if (scanf("%s", tx_buffer) > 0)
			envelope.length = strlen(tx_buffer);
		else
			envelope.length = sprintf(tx_buffer, DEFAULT_PAYLOAD);
		
		printf("\nEnter Destination End-point [%02X]: ", envelope.dest_endpoint);
		if (scanf("%hx", &endpoint) == 1)
			envelope.dest_endpoint = (uint8_t)endpoint;
		
		printf("\nEnter Source End-point [%02X]: ", envelope.source_endpoint);
		if (scanf("%hx", &endpoint) == 1)
			envelope.source_endpoint = (uint8_t)endpoint;
		
		printf("\nEnter Profile ID [%02X]: ", envelope.profile_id);
		scanf("%x", &envelope.profile_id);
		printf("\nEnter Cluster ID [%02X]: ", envelope.cluster_id);
		scanf("%x", &envelope.cluster_id);
		
		wpan_envelope_send(&envelope);
		sys_watchdog_reset();
		sys_xbee_tick();
	}
}
