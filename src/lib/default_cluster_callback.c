#include <types.h>
#include <xbee_config.h>

int default_cluster_callback(const wpan_envelope_t FAR *envelope, void FAR *context)
{
  zcl_command_t zcl;
  
  uint8_t *payload_pointer = envelope->payload;
  int i = 0;

  payload_pointer[envelope->length] = '\0'; /* Add Null-terminator for printing */
  printf("\nCUSTOM ENDPOINT'S DEFAULT CLUSTER HANDLER\n");
  printf("Received  : %s\n", payload_pointer);
  if(envelope->length > 0) {
    printf("Payload in Hex: ");
    for(i = 0; i < envelope->length; i++) {
      printf("%02X ", payload_pointer[i]);
    }
    printf("\n");
  }
  printf("Payload length: %02X\n", envelope->length);
  printf("Cluster ID:     %02X\n", envelope->cluster_id);
  printf("Profile ID:     %02X\n", envelope->profile_id);
  printf("Destin. EP:     %02X\n", envelope->dest_endpoint);
  printf("Source  EP:     %02X\n", envelope->source_endpoint);

  /* Let's try to debug the ZCL command */
  zcl_envelope_payload_dump(envelope);

  /* Let's try to respond */
  printf("\n\nBuilding ZCL Command based on received envelope: ");
  if(zcl_command_build(&zcl, envelope, context) == 0) {
    printf("OK!\n");
    printf("----------------------\n");
    printf("Frame Control: %02X\n", zcl.frame_control);
    printf("Command: %02X\n", zcl.command);
    printf("ZCL Payload length: %02X\n", zcl.length);
    hex_dump(zcl.zcl_payload, zcl.length, HEX_DUMP_FLAG_TAB);

    if(ZCL_CMD_MATCH( &zcl.frame_control, GENERAL, CLIENT_TO_SERVER, PROFILE)) {
      if(zcl.command == ZCL_CMD_READ_ATTRIB && zcl.length == 2) {
        const char FAR *zclPayload = zcl.zcl_payload;
        uint8_t                 *start_response;
        uint8_t									*end_response;
        PACKED_STRUCT {
          zcl_header_response_t	header;
          uint8_t								buffer[20];
        } response;

        response.header.command = ZCL_CMD_READ_ATTRIB_RESP;
        start_response = (uint8_t *)&response + zcl_build_header(&response.header, &zcl);
        end_response = response.buffer;

        if(zclPayload[0] == 0x01 && zclPayload[1] == 0x00) {
          /* Application version request */
          printf("Handling response for Application version request\n");
          *end_response++ = 0x01;
          *end_response++ = 0x00;
          *end_response++ = ZCL_STATUS_SUCCESS;
          *end_response++ = 0x20;
          *end_response++ = 0x00;

          printf("Response length: %02X\n", end_response - start_response);
          if(zcl_send_response(&zcl, start_response, end_response - start_response) == 0) {
            printf("Response sent successfully");
          }
        }
        else if(zclPayload[0] == 0x04 && zclPayload[1] == 0x00) {
          /* Device manufacturer request */
          printf("Handling response for Device manufacturer request\n");
          *end_response++ = 0x04;
          *end_response++ = 0x00;
          *end_response++ = ZCL_STATUS_SUCCESS;
          *end_response++ = 0x42;
          *end_response++ = 0x05; // Length of data
          *end_response++ = 'P';
          *end_response++ = 'X';
          *end_response++ = 'B';
          *end_response++ = 'e';
          *end_response++ = 'e';

          printf("Response length: %02X\n", end_response - start_response);
          if(zcl_send_response(&zcl, start_response, end_response - start_response) == 0) {
            printf("Response sent successfully");
          }
        }
        else if(zclPayload[0] == 0x05 && zclPayload[1] == 0x00) {
          /* Device developer request */
          printf("Handling response for Device developer request\n");
          *end_response++ = 0x05;
          *end_response++ = 0x00;
          *end_response++ = ZCL_STATUS_SUCCESS;
          *end_response++ = 0x42;
          *end_response++ = 0x07; // Length of data
          *end_response++ = 'T';
          *end_response++ = 'r';
          *end_response++ = 'i';
          *end_response++ = 'g';
          *end_response++ = 'g';
          *end_response++ = 'e';
          *end_response++ = 'r';

          printf("Response length: %02X\n", end_response - start_response);
          if(zcl_send_response(&zcl, start_response, end_response - start_response) == 0) {
            printf("Response sent successfully\n");
          }
        }
      }
    }
  }
  else {
    printf("Error!\n");
  }

  printf("=====================================================================\n");

  return 0;
}
