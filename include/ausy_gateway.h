#ifndef GATEWAY_H
#define GATEWAY_H

#include <stdbool.h>
#include "oc_ri.h"

extern enum client_context AUSY_CLIENT_CONTEXT;

enum ikea_root_eps {
    DEVICES = 15001,
    GROUPS = 15004,
    SCENES = 15005,
    NOTIFICATIONS = 15006,
    SMART_TASKS = 15010,
    GATEWAY = 15011
};

enum ikea_gateway_children_eps {
    REBOOT = 9030,
    RESET = 9031,
    UPDATE_FIRMWARE = 9034,
    DETAILS = 15012
};

typedef struct ikea_general_parameters {
    char *name; //--> 9001
    int creation_date; //--> 9002
    int instance_id; //--> 9003
} ikea_general_parameters;

typedef struct ikea_motion_sensor
{

} ikea_motion_sensor; // <--> oic.d.sensor

typedef struct ikea_light
{
    char* color_hex_string;
    int hue;
    int saturation;
    int color_x;
    int color_y;
    int color_temperature;
    int transition_time;
    bool on_off;
    int brightness;
} ikea_light; // <--> oic.d.light

typedef struct ikea_plug
{
    bool on_off;
    int brightness;
} ikea_plug; // <--> oic.d.smartplug

typedef struct ikea_blind
{
    float position;
} ikea_blind; // <-->oic.d.blind

typedef struct ikea_air_purifier
{
    int air_quality;
    bool controls_locked;
    int fan_mode;
    int fan_speed;
    int total_filter_lifetime;
    int filter_runtime;
    int filter_remaining_lifetime;
    int filter_status;
    bool status_leds;
    int total_motor_runtime;
} ikea_air_purifier;

typedef struct ikea_device {
    int code;
    void* array;
} ikea_devices;

enum client_context {
    OCF,
    IKEA,
    MATTER,
    GOOGLE,
    APPLE,
    UNKNOWN
};

typedef struct Gateway {
    oc_response_buffer_t *response_buffer;
    enum client_context mode;
    //oc_status_t (*add_device)(int device_id);
    //oc_status_t (*remove_device)(int device_id);
    oc_status_t (*process)(char* uri_array[], int size);
} Gateway;

extern Gateway *AUSY_GATEWAY;

/*client_context ausy_get_client_context(const char* uri_path,
                                       char*** uri_array,
                                       int* const uri_array_length);*/

oc_status_t ikea_process(char* uri_arra[], int size);

bool is_request_for_ikea_gateway(const char* uri_path,
                                 char** uri_array,
                                 int* const uri_array_length);

#endif // GATEWAY_H
