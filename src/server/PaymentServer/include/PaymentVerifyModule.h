#pragma once
#include "commdata.h"
#include "Singleton.h"
#include <time.h>
#include <string>

#define MG_CONN_VAR_LEN		64
#define GET_PAY_GATEID(R)	(R * 1000 + 200)

using namespace std;

struct SIAPRInfo
{
	int64  m_pid;
	string m_receipt;
	string m_postData;
	SOCKET m_socket;
	int    m_result;

	SIAPRInfo()
	{
		m_pid = 0;
		m_socket = -1;
		m_result = -1;
	}
};

struct mg_connection;
class Packet;
class CPaymentVerifyModule : public Singleton< CPaymentVerifyModule >
{
public:
	CPaymentVerifyModule();
	~CPaymentVerifyModule();

	void	OnLogic();
	bool	OnMsg(Packet* pack);

	bool	OnChargeFromSWServer(struct mg_connection *conn);

private:
	void	InitWhiteList();

	void	_HandlePacket_PaymentVerify(Packet *pack); 

	static void httpIAPCheckThread(void *pParam);
	static void requestFromIAPServer(SIAPRInfo *pIAPInfo);
	static size_t recvBackData(void *buffer, size_t nsize, size_t nmemb, void *pInfo);

// 	int VerifyPayment(string postData);
// 	int sendPaymentVerify(string  postData);
// 	void getDataFromJson();
// 	static size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream);
// 	void reset_data();

	bool	PayToGateServer(int gateId, int64 userId, int money);

private:
	static string m_productid;
	static string m_transactionid;
	static int	  m_stats;

	CStlMap<string, int> m_mapWhite;

// private:
// 	int m_status;
// 	string m_productid;
// 	string m_transactionid;
// 	static string m_VerifyJson;
};

#define PaymentVerifyModule CPaymentVerifyModule::getSingleton()
