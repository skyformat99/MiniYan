#ifndef MiniLoadRequest_h
#define MiniLoadRequest_h

#include "UtilString.h"
#include "KListMap.h"
#include "KRef.h"

namespace yanbo
{
typedef KListMap<CString, CString> RequestMap;
class MiniLoadRequest : public KRef
{
public:
	enum RequestMethod
	{
        EGet,
        EPost,
        EPut,
	};

public:
	MiniLoadRequest();
	~MiniLoadRequest();

	void resetRequest();

public:
	CString m_url;
	RequestMap m_headers;
	RequestMap m_params;
	int m_method;
};
}
#endif
