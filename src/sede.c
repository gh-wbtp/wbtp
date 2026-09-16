#include "wbtp.h"
#include <string.h>

void overwrite_size(uint32_t i, char *buf, uint32_t buf_size, uint32_t size)
{
    buf[i] = (char)((size >> 24) & 0xFF);
    buf[i + 1] = (char)((size >> 16) & 0xFF);
    buf[i + 2] = (char)((size >> 8) & 0xFF);
    buf[i + 3] = (char)(size & 0xFF);
}

void write_size(uint32_t *i, char *buf, uint32_t buf_size, uint32_t size)
{
    buf[(*i)++] = (char)((size >> 24) & 0xFF);
    buf[(*i)++] = (char)((size >> 16) & 0xFF);
    buf[(*i)++] = (char)((size >> 8) & 0xFF);
    buf[(*i)++] = (char)(size & 0xFF);
}

void write_str(uint32_t *i, char *buf, uint32_t buf_size, const char *str)
{
    uint32_t str_size = strlen(str);
    write_size(i, buf, buf_size, str_size);
    memcpy(buf + *i, str, str_size);
    *i += str_size;
}

uint32_t peek_size(uint32_t i, const char *buf, uint32_t buf_size)
{
    return ((uint32_t)(uint8_t)buf[i] << 24) |
           ((uint32_t)(uint8_t)buf[i + 1] << 16) |
           ((uint32_t)(uint8_t)buf[i + 2] << 8) |
           ((uint32_t)(uint8_t)buf[i + 3]);
}

uint32_t read_size(uint32_t *i, const char *buf, uint32_t buf_size)
{
    return ((uint32_t)(uint8_t)buf[(*i)++] << 24) |
           ((uint32_t)(uint8_t)buf[(*i)++] << 16) |
           ((uint32_t)(uint8_t)buf[(*i)++] << 8) |
           ((uint32_t)(uint8_t)buf[(*i)++]);
}

void read_str(uint32_t *i, const char *buf, uint32_t buf_size, char *strout)
{
    uint32_t str_size = read_size(i, buf, buf_size);
    memcpy(strout, buf + *i, str_size);
    strout[str_size] = '\0';
    *i += str_size;
}

// Public

uint32_t wbtp_request_serialize(const WbtpRequest request, char *buf, uint32_t buf_size)
{
    uint32_t i = sizeof(uint32_t);
    if (i + sizeof(uint8_t) + sizeof(uint32_t) + strlen(request.path) + sizeof(uint32_t) + strlen(request.params) + sizeof(uint32_t) + request.payload_size > buf_size)
        return 0;

    // Header
    buf[i++] = (uint8_t)request.type;
    write_str(&i, buf, buf_size, request.path);
    write_str(&i, buf, buf_size, request.params);

    // Payload
    write_size(&i, buf, buf_size, request.payload_size);
    if (request.payload_size > 0)
    {
        memcpy(buf + i, request.payload, request.payload_size);
        i += request.payload_size;
    }

    overwrite_size(0, buf, buf_size, i - sizeof(uint32_t));
    return i;
}

uint32_t wbtp_request_deserialize(WbtpRequest *request, const char *buf, uint32_t buf_size)
{
    uint32_t i = 0;

    if (i + sizeof(uint32_t) > buf_size)
        return 0;

    if (read_size(&i, buf, buf_size) > buf_size)
        return 0;

    // Header
    if (i + sizeof(uint8_t) > buf_size)
        return 0;

    request->type = (WbtpRequestType)(uint8_t)buf[i++];

    if (i + sizeof(uint32_t) > buf_size)
        return 0;

    uint32_t path_size = peek_size(i, buf, buf_size);
    if (path_size >= WBTP_PATH_MAX)
        return 0;

    if (i + sizeof(uint32_t) + path_size > buf_size)
        return 0;

    read_str(&i, buf, buf_size, request->path);

    if (i + sizeof(uint32_t) > buf_size)
        return 0;

    uint32_t params_size = peek_size(i, buf, buf_size);
    if (params_size >= WBTP_PARAMS_MAX)
        return 0;

    if (i + sizeof(uint32_t) + params_size > buf_size)
        return 0;

    read_str(&i, buf, buf_size, request->params);

    // Payload
    if (i + sizeof(uint32_t) > buf_size)
        return 0;

    request->payload_size = read_size(&i, buf, buf_size);
    if (request->payload_size > 0)
    {
        if (i + request->payload_size > buf_size)
            return 0;

        memcpy(request->payload, buf + i, request->payload_size);
        i += request->payload_size;
    }

    return i;
}

uint32_t wbtp_response_serialize(const WbtpResponse response, char *buf, uint32_t buf_size)
{
    uint32_t i = sizeof(uint32_t);
    if (i + sizeof(uint8_t) + sizeof(uint32_t) + strlen(response.params) + sizeof(uint32_t) + response.payload_size > buf_size)
        return 0;

    // Header
    buf[i++] = (uint8_t)response.type;
    write_str(&i, buf, buf_size, response.params);

    // Payload
    write_size(&i, buf, buf_size, response.payload_size);
    if (response.payload_size > 0)
    {
        memcpy(buf + i, response.payload, response.payload_size);
        i += response.payload_size;
    }

    overwrite_size(0, buf, buf_size, i - sizeof(uint32_t));
    return i;
}

uint32_t wbtp_response_deserialize(WbtpResponse *response, const char *buf, uint32_t buf_size)
{
    uint32_t i = 0;

    if (i + sizeof(uint32_t) > buf_size)
        return 0;

    if (read_size(&i, buf, buf_size) > buf_size)
        return 0;

    // Header
    if (i + sizeof(uint8_t) > buf_size)
        return 0;

    response->type = (WbtpResponseType)(uint8_t)buf[i++];

    if (i + sizeof(uint32_t) > buf_size)
        return 0;

    uint32_t params_size = peek_size(i, buf, buf_size);
    if (params_size >= WBTP_PARAMS_MAX)
        return 0;

    if (i + params_size > buf_size)
        return 0;

    read_str(&i, buf, buf_size, response->params);

    // Payload
    if (i + sizeof(uint32_t) > buf_size)
        return 0;

    response->payload_size = read_size(&i, buf, buf_size);
    if (response->payload_size > 0)
    {
        if (i + response->payload_size > buf_size)
            return 0;

        memcpy(response->payload, buf + i, response->payload_size);
        i += response->payload_size;
    }

    return i;
}
