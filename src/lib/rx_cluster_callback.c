#include <types.h>
#include <xbee_config.h>

int rx_cluster_callback(const wpan_envelope_t FAR *envelope, void FAR *context)
{
  zcl_command_t zcl;
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
