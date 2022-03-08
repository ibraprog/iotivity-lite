#include "gateway.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

client_context AUSY_CLIENT_CONTEXT = OCF;

static int split (char *str, char c, char ***arr)
{
    int count = 1;
    int token_len = 1;
    int i = 0;
    char *p;
    char *t;

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
            count++;
        p++;
    }

    *arr = (char**) malloc(sizeof(char*) * count);
    if (*arr == NULL)
        exit(1);

    p = str;
    while (*p != '\0')
    {
        if (*p == c)
        {
            (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
            if ((*arr)[i] == NULL)
                exit(1);

            token_len = 0;
            i++;
        }
        p++;
        token_len++;
    }
    (*arr)[i] = (char*) malloc( sizeof(char) * token_len );
    if ((*arr)[i] == NULL)
        exit(1);

    i = 0;
    p = str;
    t = ((*arr)[i]);
    while (*p != '\0')
    {
        if (*p != c && *p != '\0')
        {
            *t = *p;
            t++;
        }
        else
        {
            *t = '\0';
            i++;
            t = ((*arr)[i]);
        }
        p++;
    }

    return count;
}

static bool is_number(const char* input){
    for (size_t i = 0; i < strlen(input); i++) {
        if(isdigit(input[i]) == 0) return false;
    }
    return true;
}

bool is_request_for_ikea_gateway(const char* uri_path, char*** uri_array, int* const uri_array_length)
{
    if (uri_path == NULL || strlen(uri_path) == 0) {
        return false;
    }

    char* copy = strdup(uri_path);
    *uri_array_length = split(copy, '/', uri_array);
    free(copy);

    const char* first_uri = (*uri_array)[0];
    if (!is_number(first_uri)) {
        return false;
    }

    int root = strtol(first_uri, NULL, 0);
    const int gateway_roots[] = {DEVICES, GROUPS, SCENES, NOTIFICATIONS, SMART_TASKS, GATEWAY};
    int size = sizeof(gateway_roots)/sizeof(*gateway_roots);
    for (int i = 0; i < size; i++)
    {
        if(root == gateway_roots[i]) return true;
    }
    return false;
}
