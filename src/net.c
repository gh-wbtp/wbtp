#include "wbtp/net.h"
#include <stdio.h>
#include "wbtp/errors.h"

// Private

bool recv_all(WbtpSocket socket, char *buf, uint32_t size)
{
    uint32_t received = 0;
    while (received < size)
    {
#ifdef _WIN32
        int n = recv(socket, buf + received, size - received, 0);
#else
        ssize_t n = recv(socket, buf + received, size - received, 0);
#endif

        if (n <= 0)
            return false;

        received += (uint32_t)n;
    }

    return true;
}

bool send_all(WbtpSocket socket, char *buf, uint32_t size)
{
    uint32_t sent = 0;
    while (sent < size)
    {
#ifdef _WIN32
        int n = send(socket, buf + sent, size - sent, 0);
#else
        ssize_t n = send(socket, buf + sent, size - sent, 0);
#endif

        if (n <= 0)
            return false;

        sent += (uint32_t)n;
    }

    return true;
}

// Public

uint32_t wbtp_request_packet_deserialize(WbtpRequest *request, WbtpSocket connection)
{
    if (connection == WBTP_SOCKET_INVALID_VALUE)
    {
        wbtp_set_error("Passed socket is in an invalid state!");
        return 0;
    }

    char inbuf[WBTP_PACKET_MAX_SIZE];
    if (!recv_all(connection, inbuf, sizeof(uint32_t)))
    {
        wbtp_set_error("Failed to receive packet size!");
        return 0;
    }

    uint32_t packet_size = ((uint32_t)inbuf[0] << 24) |
                           ((uint32_t)inbuf[1] << 16) |
                           ((uint32_t)inbuf[2] << 8) |
                           ((uint32_t)inbuf[3]);

    if (packet_size > WBTP_PACKET_MAX_SIZE - sizeof(uint32_t))
    {
        wbtp_set_error("Packet too large!");
        return 0;
    }

    if (!recv_all(connection, inbuf + sizeof(uint32_t), packet_size))
    {
        wbtp_set_error("Failed to receive packet body!");
        return 0;
    }

    wbtp_set_error("");
    return wbtp_request_deserialize(request, inbuf, sizeof(uint32_t) + packet_size);
}

uint32_t wbtp_request_packet_serialize(const WbtpRequest request, WbtpSocket connection)
{
    if (connection == WBTP_SOCKET_INVALID_VALUE)
    {
        wbtp_set_error("Passed socket is in an invalid state!");
        return 0;
    }

    char outbuf[WBTP_PACKET_MAX_SIZE];
    uint32_t request_serialized = wbtp_request_serialize(request, outbuf, WBTP_PACKET_MAX_SIZE);
    if (!request_serialized)
    {
        char error[256];
        snprintf(error, 256, "Unable to serialize request! Apparently %u KiB of allocated space wasn't big enough.", WBTP_PACKET_MAX_SIZE / 1024);
        wbtp_set_error(error);
        return 0;
    }

    wbtp_set_error("");
    return send_all(connection, outbuf, request_serialized);
}

uint32_t wbtp_response_packet_deserialize(WbtpResponse *response, WbtpSocket connection)
{
    if (connection == WBTP_SOCKET_INVALID_VALUE)
    {
        wbtp_set_error("Passed socket is in an invalid state!");
        return 0;
    }

    char inbuf[WBTP_PACKET_MAX_SIZE];
    if (!recv_all(connection, inbuf, sizeof(uint32_t)))
    {
        wbtp_set_error("Failed to receive packet size!");
        return 0;
    }

    uint32_t packet_size = ((uint32_t)inbuf[0] << 24) |
                           ((uint32_t)inbuf[1] << 16) |
                           ((uint32_t)inbuf[2] << 8) |
                           ((uint32_t)inbuf[3]);

    if (packet_size > WBTP_PACKET_MAX_SIZE - sizeof(uint32_t))
    {
        wbtp_set_error("Packet too large!");
        return 0;
    }

    if (!recv_all(connection, inbuf + sizeof(uint32_t), packet_size))
    {
        wbtp_set_error("Failed to receive packet body!");
        return 0;
    }

    wbtp_set_error("");
    return wbtp_response_deserialize(response, inbuf, sizeof(uint32_t) + packet_size);
}

uint32_t wbtp_response_packet_serialize(const WbtpResponse response, WbtpSocket connection)
{
    if (connection == WBTP_SOCKET_INVALID_VALUE)
    {
        wbtp_set_error("Passed socket is in an invalid state!");
        return 0;
    }

    char outbuf[WBTP_PACKET_MAX_SIZE];
    uint32_t response_serialized = wbtp_response_serialize(response, outbuf, WBTP_PACKET_MAX_SIZE);
    if (!response_serialized)
    {
        char error[256];
        snprintf(error, 256, "Unable to serialize response! Apparently %u KiB of allocated space wasn't big enough.", WBTP_PACKET_MAX_SIZE / 1024);
        wbtp_set_error(error);
        return 0;
    }

    wbtp_set_error("");
    return send_all(connection, outbuf, response_serialized);
}
