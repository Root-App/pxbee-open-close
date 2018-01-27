/*
 * This is an auto-generated file based on the
 * configuration of the XBee Programmable project.
 *
 * Edit this file to change project parameters.
 */

#ifndef __XBEE_CONFIG_H_
#define __XBEE_CONFIG_H_

/* Project definitions */
#define APP_VERSION_STRING              "OpenClose v0.0.1"
#define CONFIG_XBEE_ZB
#define CONFIG_XBEE_SMT
#define CONFIG_XBEE_S2C
#define CONFIG_XBEE_FLASH_LEN           32

/* system0 component */
#define SYS_CFG_CLK_48_MHz
#define SYS_CFG_BUSCLK_SYSCLK_DIV2
#define ENABLE_WD
#define WD_CFG_LONG_TOUT

/* special0 component */
#define ENABLE_RESET_PIN_XPIN_6
#define ENABLE_RSSI_PWM_XPIN_7
#define ENABLE_BKGD_PIN_XPIN_9
#define ENABLE_ASSOCIATION_LED_XPIN_28
#define ENABLE_COMMISSIONING_XPIN_33

/* rtc0 component */
#define ENABLE_RTC
#define RTC_ENABLE_PERIODIC_TASK
#define RTC_CFG_PERIODIC_TASK_PERIOD    125

/* LED component */
#define ENABLE_GPIO_XPIN_24

#define SENSOR                           XPIN_5
#define LED                              XPIN_24
#define GPIO_CFG_DIR_24                  GPIO_CFG_OUTPUT
#define GPIO_CFG_PULL_UP_EN_24           GPIO_CFG_PULL_UP_DIS
#define GPIO_CFG_SLEW_RATE_EN_24         GPIO_CFG_SLEW_RATE_EN
#define GPIO_CFG_DRV_STR_24              GPIO_CFG_DRV_STR_LOW

/* network0 component */
#define ENABLE_XBEE_HANDLE_REMOTE_AT
#define ENABLE_XBEE_HANDLE_ND_RESPONSE_FRAMES
#define ENABLE_XBEE_HANDLE_RX
#define ENABLE_OTA_UPDATE

/* uart0 component */
#define ENABLE_UART
#define UART_CFG_MODE_2W                1
#define UART_CFG_BAUDRATE               115200
#define UART_CFG_PAR_EN                 UART_CFG_PARITY_DIS
#define UART_CFG_PAR_VAL                UART_CFG_PARITY_ODD
#define UART_CFG_BITS                   UART_CFG_BITS_8
#define UART_CFG_RX_WATERMARK           1
#define UART_CFG_RX_BUF_LEN             32
#define ENABLE_STDIO_PRINTF_SCANF       1

/* xbee_config0 component */
//#define XBEE_ATCMD_PARAM_ID             "0123456789ABCDEF"
#define XBEE_ATCMD_PARAM_NI             "PROGRAMMABLE TEST 1"

/* Used pins macros */
#define XPIN_28_USED
#define XPIN_33_USED
#define XPIN_6_USED
#define XPIN_9_USED
#define XPIN_7_USED
#define XPIN_4_USED
#define XPIN_3_USED
#define XPIN_5_USED
#define XPIN_8_USED
#define XPIN_24_USED

/* Components includes */
#include <custom.h>
#include <system.h>
#include <rtc.h>
#include <gpios.h>
#include <pan_init.h>
#include <endpoints.h>
#include <xbee/discovery.h>
#include <xbee/wpan.h>
#include <xbee/atcmd.h>
#include <uart.h>

#endif /* __XBEE_CONFIG_H_ */
