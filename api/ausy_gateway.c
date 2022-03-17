#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "ausy_gateway.h"
#include "deps/json-parser/json.h"

enum client_context AUSY_CLIENT_CONTEXT = OCF;
Gateway *AUSY_GATEWAY = NULL;

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

static bool is_number(const char* input){
    for (size_t i = 0; i < strlen(input); i++) {
        if(isdigit(input[i]) == 0) return false;
    }
    return true;
}

bool is_request_for_ikea_gateway(const char* uri_path, char** paths, int* const size)
{
    if (uri_path == NULL || strlen(uri_path) == 0) {
        return false;
    }

    char* copy = strdup(uri_path);
    paths = str_split(copy, '/');

    if (paths)
    {
        *size = (int)(sizeof(paths)/sizeof(paths[0]));

        if (!is_number(paths[0])) {
            return false;
        }

        int root = strtol(paths[0], NULL, 0);
        const int gateway_roots[] = {DEVICES, GROUPS, SCENES, NOTIFICATIONS, SMART_TASKS, GATEWAY};
        int len = (int)sizeof(gateway_roots)/sizeof(*gateway_roots);
        for (int i = 0; i < len; i++)
        {
            if(root == gateway_roots[i]) return true;
        }
    }

    free(copy);

    return false;
}

/*oc_status_t ikea_remove_device(int device_id) {

}

oc_status_t ikea_add_device(int device_id) {

}

oc_status_t ikea_get_devive_info(int device_id) {

}*/

oc_status_t ikea_get_all_devices() {
    //uint8_t* bufffer = NULL;
    //AUSY_GATEWAY->response_buffer->buffer = buffer;

    return OC_STATUS_OK;
}

oc_status_t ikea_process(char *uri_arr[], int size) {
    if (uri_arr == NULL || size < 1 || size > 2) {
        return OC_STATUS_NOT_ACCEPTABLE;
    }

    oc_status_t status = OC_STATUS_BAD_REQUEST;
    // roots
    enum ikea_root_eps root = atoi(uri_arr[0]);
    if (root == DEVICES) {
        if (size == 1) {
            return OC_STATUS_OK;
        }
    } else if (root == GROUPS) {

    } else if (root == SCENES) {

    } else if (root == NOTIFICATIONS) {

    } else if (root == SMART_TASKS) {

    } else if (root == GATEWAY) {

    }

    return status;
}
