#pragma once

class InetAddress;

class Socket
{
private:
	// socket �ļ�������
	int fd;

public:
	Socket();
	explicit Socket(int);
	~Socket();

	/*
	* @brief ��Ҫ�����ĵ�ַ
	*
	* @param _addr �󶨵ĵ�ַ
	*/
	void bind(InetAddress *_addr);

	/*
	* @brief ����
	*/
	void listen();

	/*
	* @brief ����Ϊ������
	*/
	void setNonBlocking();

	/*
	* @brief ���ܿͻ�������
	*
	* @param _addr �ͻ��˵�ַ
	* @return �ͻ��� socket fd
	*/
	int accept(InetAddress *_addr);

	/**
	 * @brief ���ӵ�ָ���ĵ�ַ
	 *
	 * @param _addr Ҫ���ӵ�InetAddress��
	 */
	void connect(InetAddress *_addr);

	/*
	* @brief ��ȡ socket fd
	*
	* @return socket fd
	*/
	int getFd();
};
