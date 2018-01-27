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
 *   XPIN5 = pwm0 [PWM Pin]
 *   XPIN6 = special0 [Reset Pin]
 *   XPIN7 = special0 [RSSI PWM Pin]
 *   XPIN8 = adc0 [ADC Pin]
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
 *   XPIN24 = gpio0 [GPIO Pin]
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

#if defined(RTC_ENABLE_PERIODIC_TASK)
void rtc_periodic_task(void)
{
    /*
     * Function call every RTC_CFG_PERIODIC_TASK_PERIOD * 8 ms.
     * This function is called from the timer ISR, please be brief
     * and exit, or just set a flag and do your home work in the 
     * main loop
     */

    /* Implement your code here */
}
#endif

#ifdef ENABLE_XBEE_HANDLE_ND_RESPONSE_FRAMES
void node_discovery_callback(xbee_dev_t *xbee, const xbee_node_id_t *node_id)
{
	/* This function is called every time a node is discovered, either by
	 * receiving a NodeID message or because a node search was started with
	 * function xbee_disc_discover_nodes() */
	return;
}
#endif


#ifdef ENABLE_XBEE_HANDLE_RX
int xbee_transparent_rx(const wpan_envelope_t FAR *envelope, void FAR *context)
{
    /* Add your code here... */

	return 0;
}
#endif

void main(void)
{
	sys_hw_init();
	sys_xbee_init();
	sys_app_banner();

	for (;;) {
		/* Write your code here... */
		sys_watchdog_reset();
		sys_xbee_tick();
	}
}
