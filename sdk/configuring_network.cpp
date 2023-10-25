// sdk.cpp : ���徲̬��ĺ�����
//

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "pch.h"
#include "framework.h"
#include "configuring_network.h"
#include "../firmware/easylogging++.h"


INITIALIZE_EASYLOGGINGPP

// TODO: ����һ���⺯��ʾ��
/*******************************************************************************************/

bool BIND_FLAG_ = false;
const int MAX_BUF_LEN = 255;
// ����socket
int connect_socket;
// �����������ϵĹ㲥��ַ��������
struct sockaddr_in sin_from;

int send_server_data(const char* buff)
{
    // Create a socket
    int connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (-1 == connect_socket)
    {
        std::cerr << "socket error: " << strerror(errno) << std::endl;
        return -1;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERVER_PORT);
    sin.sin_addr.s_addr = INADDR_BROADCAST;

    int bOpt = 1;
    // Set the socket to broadcast mode
    if (setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, &bOpt, sizeof(bOpt)) == -1)
    {
        std::cerr << "setsockopt error: " << strerror(errno) << std::endl;
        close(connect_socket);
        return -1;
    }

    socklen_t nAddrLen = sizeof(sin);

    // Send the data
    ssize_t nSendSize = sendto(connect_socket, buff, strlen(buff), 0, (struct sockaddr*)&sin, nAddrLen);
    if (-1 == nSendSize)
    {
        std::cerr << "sendto error: " << strerror(errno) << std::endl;
        close(connect_socket);
        return -1;
    }

    close(connect_socket);
    return 0;
}

/*
int send_param(const char* buff)
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	// ����socket api
	wVersionRequested = MAKEWORD(2, 2);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		return -1;
	}

	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		return -1;
	}

	// ����socket
	int connect_socket;
	connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (-1 == connect_socket)
	{
		err = WSAGetLastError();
		std::cout << "socket error: error code is " << err;
		return -1;
	}

	struct sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	sin.sin_addr.s_addr = INADDR_BROADCAST;

	bool bOpt = true;
	//���ø��׽���Ϊ�㲥����
	setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, (char*)&bOpt, sizeof(bOpt));

	int nAddrLen = sizeof(SOCKADDR);

	int ret = bind_server_port();

	if (0 != ret)
	{
		std::cout << "bind error" << std::endl;
	}

	// ��������
	int nSendSize = sendto(connect_socket, buff, strlen(buff), 0, (SOCKADDR*)&sin, nAddrLen);
	if (-1 == nSendSize)
	{
		err = WSAGetLastError();
		//printf("/"sendto / " error!, error code is %d/n", err);
		std::cout << "sendto error: error code is " << err;
		return -1;
	}
	printf("Send: %s \n", buff);

	return 0;
}
*/
int send_command(int command)
{
    // Create a socket
    int connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (-1 == connect_socket)
    {
        std::cerr << "socket error: " << strerror(errno) << std::endl;
        return -1;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(SERVER_PORT);
    sin.sin_addr.s_addr = INADDR_BROADCAST;

    int bOpt = 1;
    // Set the socket to broadcast mode
    if (setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, &bOpt, sizeof(bOpt)) == -1)
    {
        std::cerr << "setsockopt error: " << strerror(errno) << std::endl;
        close(connect_socket);
        return -1;
    }

    socklen_t nAddrLen = sizeof(sin);

    char buff[MAX_BUF_LEN] = "";

    snprintf(buff, sizeof(buff), "%8d", command);

    // Send the data
    ssize_t nSendSize = sendto(connect_socket, buff, strlen(buff), 0, (struct sockaddr*)&sin, nAddrLen);
    if (-1 == nSendSize)
    {
        std::cerr << "sendto error: " << strerror(errno) << std::endl;
        close(connect_socket);
        return -1;
    }

    close(connect_socket);
    return 0;
}


int set_recv_timeout(int nNetTimeout)
{
    struct timeval timeout;
    timeout.tv_sec = nNetTimeout / 1000;
    timeout.tv_usec = (nNetTimeout % 1000) * 1000;
    setsockopt(connect_socket, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
    return 0;
}

int set_send_timeout(int nNetTimeout)
{
    struct timeval timeout;
    timeout.tv_sec = nNetTimeout / 1000;
    timeout.tv_usec = (nNetTimeout % 1000) * 1000;
    setsockopt(connect_socket, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout));
    return 0;
}

int bind_server_port(int nNetTimeout)
{
    if (BIND_FLAG_)
    {
        set_recv_timeout(nNetTimeout);
        std::cerr << "BIND_FLAG_: already true." << std::endl;
        return -1;
    }

    connect_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (-1 == connect_socket)
    {
        std::cerr << "socket error: " << strerror(errno) << std::endl;
        return -1;
    }

    struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(CLIENT_PORT);
    sin.sin_addr.s_addr = INADDR_ANY;

    struct sockaddr_in sin_from;
    sin_from.sin_family = AF_INET;
    sin_from.sin_port = htons(CLIENT_PORT);
    sin_from.sin_addr.s_addr = INADDR_BROADCAST;

    int bOpt = 1;
    setsockopt(connect_socket, SOL_SOCKET, SO_BROADCAST, &bOpt, sizeof(bOpt));

    set_recv_timeout(nNetTimeout);

    if (bind(connect_socket, (struct sockaddr*)&sin, sizeof(sin)) == -1)
    {
        std::cerr << "bind error: " << strerror(errno) << std::endl;
        return -1;
    }

    BIND_FLAG_ = true;
    
    return 0;
}
/*
int recv_server_command(int& command)
{
	int err;

	int nAddrLen = sizeof(SOCKADDR);
	char buff[MAX_BUF_LEN] = "";
	int nLoop = 0;

	int nSendSize = recvfrom(connect_socket, buff, MAX_BUF_LEN, 0, (SOCKADDR*)&sin_from, &nAddrLen);
	if (-1 == nSendSize)
	{
		err = WSAGetLastError();

		return -1;
	}
	buff[nSendSize] = '\0';
	printf("Recv: %s \n", buff);

	std::cout << "nSendSize: " << nSendSize << std::endl;

	std::string command_str(buff);

	std::stringstream ss;
	ss << command_str;
	ss >> command;

	return 0;;
}
*/
int recv_server_data(char* buffer, int& buffer_size)
{
    socklen_t nAddrLen = sizeof(struct sockaddr_in);

    int nRecvSize = recvfrom(connect_socket, buffer, MAX_BUF_LEN, 0, (struct sockaddr*)&sin_from, &nAddrLen);
    if (-1 == nRecvSize)
    {
        std::cerr << "recvfrom error: " << strerror(errno) << std::endl;
        return -1;
    }

    buffer[nRecvSize] = '\0';
    buffer_size = nRecvSize;

    return 0;
}

std::vector<std::string> vStringSplit(const std::string& s, const std::string& delim)
{
	std::vector<std::string> elems;
	size_t pos = 0;
	size_t len = s.length();
	size_t delim_len = delim.length();
	if (delim_len == 0)
		return elems;
	while (pos < len)
	{
		int find_pos = s.find(delim, pos);
		if (find_pos < 0)
		{
			elems.push_back(s.substr(pos, len - pos));
			break;
		}
		elems.push_back(s.substr(pos, find_pos - pos));
		pos = find_pos + delim_len;
	}
	return elems;
}

int SetCameraIp(std::string mac, std::string& ip)
{
    LOG(INFO) << "SET_IP......";
    int ret = bind_server_port(static_cast<int>(0.3 * 1000));
    if (0 != ret) {
        LOG(INFO) << "SetCameraIp: bind error and continue";
    }

    // set ip address
    char command_c[MAX_BUF_LEN] = "";
    sprintf(command_c, "%8d", DF_CMD_SET_NETWORK_IP);
    std::string str_data = std::string(command_c) + ";" + mac + ";" + ip;
    char* message_buff = const_cast<char*>(str_data.c_str());
    send_server_data(message_buff);
    LOG(INFO) << "Set static ip complete, wait for network to be accessed.";

    // wait 5sec at least for camera network re-up
    sleep(5);

    // get the new ip address
    memset(command_c, 0xff, MAX_BUF_LEN);
    sprintf(command_c, "%8d", DF_CMD_GET_NETWORK_IP);
    str_data = std::string(command_c) + ";" + mac + ";" + ip;
    message_buff = const_cast<char*>(str_data.c_str());
    send_server_data(message_buff);

    char recv_buff[MAX_BUF_LEN] = "";
    int recv_buff_size = 0;
    ret = recv_server_data(recv_buff, recv_buff_size);
    if (0 != ret)
    {
        LOG(INFO) << "get new ip address error";
        return -1;
    }

    std::string rec_str = std::string(recv_buff);
    std::vector<std::string> str_list = vStringSplit(rec_str, ";");
    LOG(INFO) << "new ip address:" << str_list[0];

    if (str_list.size() > 0) {
        ip = str_list[0];
    } else {
        ip = "";
    }

    return 0;
}

int SetCameraIpAuto(std::string mac, std::string& ip)
{
    LOG(INFO) << "SET_AUTO......";
    int ret = bind_server_port(static_cast<int>(0.3 * 1000));
    if (0 != ret) {
        LOG(INFO) << "SetCameraIpAuto: bind error and continue";
    }

    // set ip address be dhcp mode
    char command_c[MAX_BUF_LEN] = "";
    sprintf(command_c, "%8d", DF_CMD_SET_NETWORK_DHCP);
    printf("mac: %s \n", mac.c_str());
    std::string str_data = std::string(command_c) + ";" + mac;
    char* message_buff = const_cast<char*>(str_data.c_str());
    send_server_data(message_buff);
    LOG(INFO) << "Set dhcp complete, wait for DHCP discover";

    // wait 20sec at least for camera network re-up
    sleep(20);

    // get the new ip address
    memset(command_c, 0xff, MAX_BUF_LEN);
    sprintf(command_c, "%8d", DF_CMD_GET_NETWORK_IP);
    str_data = std::string(command_c) + ";" + mac;
    message_buff = const_cast<char*>(str_data.c_str());
    send_server_data(message_buff);

    char recv_buff[MAX_BUF_LEN] = "";
    int recv_buff_size = 0;
    ret = recv_server_data(recv_buff, recv_buff_size);
    if (0 != ret) 
    {
        LOG(INFO) << "get new ip address error";
        return -1;
    }

    std::string rec_str = std::string(recv_buff);
    std::vector<std::string> str_list = vStringSplit(rec_str, ";");
    LOG(INFO) << "new ip address:" << str_list[0];

    if (str_list.size() > 0) {
        ip = str_list[0];
    } else {
        ip = "";
    }

    return 0;
}


int SetCameraNetwork(long cmd, std::string mac, std::string &cfg, long sleep_duration)
{
    LOG(INFO) << "Camera Network Setting";
    int ret = bind_server_port(static_cast<int>(0.3 * 1000));
    if (0 != ret) {
        LOG(INFO) << "bind error and continue";
    }

    // set command
    char command_c[MAX_BUF_LEN] = "";
    sprintf(command_c, "%8ld", cmd);  // Use %8ld for long
    std::string str_data = std::string(command_c) + ";" + mac + ";" + cfg;
    char* message_buff = const_cast<char*>(str_data.c_str());
    send_server_data(message_buff);

    // instruction list
    long getCmd = DF_CMD_GET_NETWORK_IP;
    switch (cmd)
    {
        case DF_CMD_SET_NETWORK_IP:
            getCmd = DF_CMD_GET_NETWORK_IP;
            LOG(INFO) << "Set static ip complete, wait for network to be accessed.";
            break;

        case DF_CMD_SET_NETWORK_NETMASK:
            getCmd = DF_CMD_GET_NETWORK_NETMASK;
            LOG(INFO) << "Set netmask complete, wait for network to be accessed.";
            break;

        case DF_CMD_SET_NETWORK_GATEWAY:
            getCmd = DF_CMD_GET_NETWORK_GATEWAY;
            LOG(INFO) << "Set gateway complete, wait for network to be accessed.";
            break;

        case DF_CMD_SET_NETWORK_DHCP:
            getCmd = DF_CMD_GET_NETWORK_IP;
            LOG(INFO) << "Set DHCP complete, wait for DHCP discover";
            break;

        default:
            break;
    }

    // wait Xsec at least for camera network re-up
    sleep(sleep_duration);

    // get the new setting
    memset(command_c, 0xff, MAX_BUF_LEN);
    sprintf(command_c, "%8ld", getCmd);  // Use %8ld for long
    str_data = std::string(command_c) + ";" + mac + ";" + cfg;
    message_buff = const_cast<char*>(str_data.c_str());
    send_server_data(message_buff);

    char recv_buff[MAX_BUF_LEN] = "";
    int recv_buff_size = 0;
    ret = recv_server_data(recv_buff, recv_buff_size);
    if (0 != ret)
    {
        LOG(INFO) << "get new setting error";
        return -1;
    }

    // unpack the setting
    std::string rec_str = std::string(recv_buff);
    std::vector<std::string> str_list = vStringSplit(rec_str, ";");
    LOG(INFO) << "new setting:" << str_list[0];

    if (str_list.size() > 0) {
        cfg = str_list[0];
    }
    else {
        cfg = "";
    }

    return 0;
}

int GetCameraList(std::vector<std::string>& mac_list, std::vector<std::string>& ip_list)
{
	std::cout << "GET_MAC......" << std::endl;

	int ret = bind_server_port(0.1 * 1000);
	if (0 != ret)
	{
		LOG(INFO) << "bind error";
	}

	send_command(DF_CMD_GET_NETWORK_MAC);

	std::vector<std::string> str_list;

	for (int i = 0; i < 20; i++)
	{
		char recv_buff_temp[MAX_BUF_LEN] = "";
		int recv_buff_size_temp = 0;
		recv_server_data(recv_buff_temp, recv_buff_size_temp);

		if (recv_buff_size_temp > 0)
		{
			LOG(INFO) << "mac: " << recv_buff_temp;

			std::string mac_str(recv_buff_temp);
			str_list.push_back(mac_str);
		}
	}

	mac_list.clear();
	ip_list.clear();

	for (int n = 0; n < str_list.size(); n++)
	{
		std::vector<std::string> str = vStringSplit(str_list[n], ";");
		
		if (str.size() >= 2)
		{
			mac_list.push_back(str[0]);
			ip_list.push_back(str[1]);
		}
	}

	return 0;
}