#pragma once

#include <arpa/inet.h>
#include <string>

class InetAddress
{
private:
	//��ʾIPv4�׽��ֵ�ַ�Ľṹ
	struct sockaddr_in addr;

	//socket��ַ�ṹ�ĳ���
	socklen_t addrLen;

public:
	InetAddress();
	explicit InetAddress(std::string const &ip, uint16_t port);
	~InetAddress();

	/**
	 * @brief ���õ�ַ��Ϣ
	 * @param _addr socket��ַ
	 * @param _addrLen socket��ַ����
	 */
	void setInetAddr(sockaddr_in _addr, socklen_t _addrLen);

	/**
	 * @brief ��ȡsocket��ַ
	 * @return The socket address
	 */
	sockaddr_in getAddr();

	/**
	 * @brief ��ȡ��������ַ�ĳ���
	 * @return socket��ַ�ĳ���
	 */
	socklen_t getAddrLen();
};
