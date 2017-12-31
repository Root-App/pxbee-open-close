#include <types.h>
#include <xbee_config.h>

/* Callback function for XBee commands */
int xbee_cmd_callback ( const xbee_cmd_response_t FAR *response )
{
	bool_t printable;
	uint_fast8_t length, i;
	uint8_t status;
	const uint8_t FAR *p;

	printf( "AT%.*" PRIsFAR " ", 2, response->command.str);

	if (response->flags & XBEE_CMD_RESP_FLAG_TIMEOUT)
	{
		puts( "(timed out)");
		return XBEE_ATCMD_DONE;
	}

	status = response->flags & XBEE_CMD_RESP_MASK_STATUS;
	if (status != XBEE_AT_RESP_SUCCESS)
	{
		printf( "(error: %s)\n",
			(status == XBEE_AT_RESP_ERROR) ? "general" :
			(status == XBEE_AT_RESP_BAD_COMMAND) ? "bad command" :
			(status == XBEE_AT_RESP_BAD_PARAMETER) ? "bad parameter" :
			(status == XBEE_AT_RESP_TX_FAIL) ? "Tx failure" :
														  "unknown error");
		return XBEE_ATCMD_DONE;
	}

	length = response->value_length;
	if (! length)		// command sent successfully, no value to report
	{
		puts( "(success)");
		return XBEE_ATCMD_DONE;
	}

	// check to see if we can print the value out as a string
	printable = 1;
	p = response->value_bytes;
	for (i = length; i; ++p, --i)
	{
		if (! isprint( *p))
		{
			printable = 0;
			break;
		}
	}

	if (printable)
	{
		printf( "= \"%.*" PRIsFAR "\" ", length, response->value_bytes);
	}
	if (length <= 4)
	{
		// format hex string with (2 * number of bytes in value) leading zeros
		printf( "= 0x%0*" PRIX32 " (%" PRIu32 ")\n", length * 2, response->value,
			response->value);
	}
	else
	{
		printf( "= %d bytes:\n", length);
		hex_dump( response->value_bytes, length, HEX_DUMP_FLAG_TAB);
	}

	return XBEE_ATCMD_DONE;
}
// END: Callback functions for XBee commands
