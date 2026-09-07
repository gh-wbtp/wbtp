#pragma once

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>

#define WBTP_SOCKET_INVALID_VALUE INVALID_SOCKET
#define WBTP_SOCKET_ERROR_VALUE SOCKET_ERROR

typedef SOCKET WbtpSocket;

#else

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#define WBTP_SOCKET_INVALID_VALUE (-1)
#define WBTP_SOCKET_ERROR_VALUE (-1)

typedef int WbtpSocket;

#endif

#include "wbtp/winshit.h"
#include "wbtp.h"

#define WBTP_PACKET_MAX_SIZE 4096

WBTP_API uint32_t wbtp_request_packet_deserialize(WbtpRequest *request, WbtpSocket connection);

WBTP_API uint32_t wbtp_request_packet_serialize(const WbtpRequest request, WbtpSocket connection);

WBTP_API uint32_t wbtp_response_packet_deserialize(WbtpResponse *response, WbtpSocket connection);

WBTP_API uint32_t wbtp_response_packet_serialize(const WbtpResponse response, WbtpSocket connection);
