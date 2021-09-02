#ifndef DEXFORCE_PROTOCAL_H
#define DEXFORCE_PROTOCAL_H
 

#define DF_PORT 8080
#define CLIENT_PORT 8081
#define SERVER_PORT 8082

#define DF_CMD_CONNECT 10000001
#define DF_CMD_DISCONNECT 10000002
#define DF_CMD_GET_BRIGHTNESS 10000003
#define DF_CMD_GET_DEPTH 10000004
#define DF_CMD_GET_POINTCLOUD 10000005
#define DF_CMD_GET_CONFIDENCE 10000006
#define DF_CMD_GET_RAW 10000007
#define DF_CMD_GET_TEMPERATURE 10000008
#define DF_CMD_SET_CAMERA_PARAMETERS 10000009
#define DF_CMD_GET_CAMERA_PARAMETERS 10000010
#define DF_CMD_SET_PATTERN_TABLE 10000011
#define DF_CMD_GET_PATTERN_TABLE 10000012
#define DF_CMD_GET_RAW_TEST 10000013


#define DF_CMD_GET_RAW_01 11000001
#define DF_CMD_GET_RAW_02 11000002
#define DF_CMD_GET_RAW_03 11000003
#define DF_CMD_GET_RAW_04 11000004


#define DF_CMD_GET_FRAME_01 12000001
#define DF_CMD_GET_FRAME_02 12000002
#define DF_CMD_GET_FRAME_03 12000003

#define DF_CMD_GET_FRAME_HDR 12000103


#define DF_CMD_GET_NETWORK_ADDRESS 13000001
#define DF_CMD_GET_NETWORK_IP 13000002
#define DF_CMD_GET_NETWORK_NETMASK 13000003
#define DF_CMD_GET_NETWORK_GATEWAY 13000004
#define DF_CMD_GET_NETWORK_DNS 13000005
#define DF_CMD_SET_NETWORK_IP 13000006
#define DF_CMD_SET_NETWORK_NETMASK 13000007
#define DF_CMD_SET_NETWORK_GATEWAY 13000008
#define DF_CMD_SET_NETWORK_DNS 13000009
#define DF_CMD_GET_NETWORK_MAC 13000010

#define DF_CMD_OK 20000001
#define DF_CMD_REJECT 20000002

#define DF_CMD_HEARTBEAT 30000001

#define DF_SUCCESS 0
#define DF_FAILED 1


#endif
