#ifndef GATEWAY_H
#define GATEWAY_H

#include <stdbool.h>

extern client_context AUSY_CLIENT_CONTEXT;

enum root_endpoint {
    DEVICES = 15001,
    GROUPS = 15004,
    SCENES = 15005,
    NOTIFICATIONS = 15006,
    SMART_TASKS = 15010,
    GATEWAY = 15011
};

enum gateway_children_endpoint {
    REBOOT = 9030,
    RESET = 9031,
    UPDATE_FIRMWARE = 9034,
    DETAILS = 15012
};

/*typedef struct gateway {
    //client_context client_context;
    void (*set_client_context)(client_context);
    //void process(char*** uri_array, int size);
} gateway;*/


enum client_context {
    OCF,
    IKEA,
    MATTER,
    UNKNOWN
};
/*client_context ausy_get_client_context(const char* uri_path,
                                       char*** uri_array,
                                       int* const uri_array_length);*/

bool is_request_for_ikea_gateway(const char* uri_path,
                                 char*** uri_array,
                                 int* const uri_array_length);

#endif // GATEWAY_H
