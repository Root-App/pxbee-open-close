#include <zigbee/zdo.h>

extern wpan_ep_state_t zdo_ep_state;
extern wpan_ep_state_t custom_ha_ep_state;

#define for_each_item(item, list) \
    for(T * item = list->head; item != NULL; item = item->next)

#define CUSTOM_ENDPOINT     0xEA
#define CUSTOM_EP_PROFILE   0x0104

 /* With this macro the prototypes of clusters' callbacks and extern variables are included in endpoints.c
  * Array custom_ep_data_clusters[] is declared in main.c
  * Function default_cluster_callback() is implemented in default_cluster_callback.c */
#define EP_INCLUDE_DECLARATIONS extern const wpan_cluster_table_entry_t custom_ep_clusters[];  \
                                int default_cluster_callback(const wpan_envelope_t FAR *, void FAR *);

 /* This is a wpan_endpoint_table_entry_t structure, see its declaration in aps.h, the '{}' are because they
  * will be included in endpoints_table[] in endpoints.c */
#define ADDITIONAL_ENDPOINTS {CUSTOM_ENDPOINT, CUSTOM_EP_PROFILE, default_cluster_callback, &custom_ha_ep_state, 0x0002, 0x00, custom_ep_clusters}, \
                             ZDO_ENDPOINT(zdo_ep_state)

 /* This macro is automatically defined if Process Incoming frames, Node Discovery Support or Over-the-Air
  * Firmware Update are enabled, otherwise it should be defined so the library handles frames */
#define ENABLE_XBEE_HANDLE_RX_ZCL_FRAMES
