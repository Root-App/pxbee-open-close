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
#include <string.h>
#include <wpan/aps.h>
#include <xbee/transparent_serial.h>
#include <xbee/atcmd.h>

/* Start joined... just in case we lose the first modem status frame */
uint8_t joined = 1;

#ifdef ENABLE_XBEE_HANDLE_MODEM_STATUS_FRAMES
int xbee_modem_status_handler(xbee_dev_t *xbee, const void FAR *payload,
                              uint16_t length, void FAR *context)
{
	const xbee_frame_modem_status_t FAR *frame = payload;

	if (frame->status == XBEE_MODEM_STATUS_JOINED)
		joined = 1;
	if (frame->status == XBEE_MODEM_STATUS_DISASSOC)
		joined = 0;
	
	return 0;
}
#endif

void main(void)
{
	wpan_envelope_t env;
	uint32_t now, before = -1;
	char txbuf[30];
	uint16_t msgcnt = 1;

	sys_hw_init();
	sys_xbee_init();
	sys_app_banner();

	wpan_envelope_create(&env, &xdev.wpan_dev, WPAN_IEEE_ADDR_COORDINATOR,
											   WPAN_NET_ADDR_UNDEFINED);
	env.payload = txbuf;

	for (;;) {
		now = rtc_get_uptime();
		
		if (now != before) {
			before = now;
			if (joined) {
				/* Construct and send the message... */
				env.length = sprintf(txbuf, "Hello coordinator - %u", msgcnt++);
				xbee_transparent_serial(&env);
			}
		}
		
		sys_watchdog_reset();
		sys_xbee_tick();
	}
}
