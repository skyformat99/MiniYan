#ifndef NetWorkBase_h
#define NetWorkBase_h

#include "kmap.h"
#include "UtilString.h"
namespace yanbo
{

// software abstraction layer API
// network abstract class
class NetWorkClient
{
public:
	enum LoadError
	{
	    NETWORK_UNCONNECT,
		NETWORK_FILE_ERROR,
		NETWORK_MEMORY_LACK
	};

public:
	virtual void onDataReceived(const StringA& data) = 0;
	virtual void onStatusCode(int statusCode) = 0;
	virtual void onFileLen(int len) = 0;
	virtual void onRedirectUrl(const StringA& redirectUrl) = 0;
	virtual void onLoadError(int error) = 0;
	virtual void onLoadFinished() = 0;
};

typedef KMap<StringA, StringA> ParamMap;
class NetWorkBase
{
public:
	NetWorkBase()
	    : m_client(NULL)
	{
	}

	virtual ~NetWorkBase() {}

public:	
	void setNetWorkClient(NetWorkClient* client) { m_client = client; }
	virtual void loadUrl(const String& url) {}
	virtual void postData(const ParamMap& params) {}
	virtual void cancel() {}
	
protected:
	NetWorkClient* m_client;
};

}
#endif
