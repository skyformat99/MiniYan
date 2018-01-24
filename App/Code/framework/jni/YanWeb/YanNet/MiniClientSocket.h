#ifndef MiniSocket_h
#define MiniSocket_h

#include "UtilString.h"
#include "KRef.h"
#include "MiniClientListener.h"
#include "MiniMutex.h"

namespace yanbo
{
// 客户端socket的封装
class MiniClientSocket : public KRef
{
public:
	enum ClientState
	{
        EParseHostError,
        ECreateSocketError,
        EConnectError,
        ESendDataError,
        ESendDataSuccess,
        EReceiveDataError,
        EReceiveDataSuccess,
	};
public:
	MiniClientSocket(MiniClientListener* listener);
	virtual ~MiniClientSocket();

public:
	int initClientSocket(const CString& hostName, int serverPort);
	int requestData(const CString& data);
    int receiveData();
	void blockClose();

private:
	void closeSocket();

	int m_socketHandle;
	MiniClientListener *m_listener;
	MiniMutex m_mutex;
};
}
#endif
