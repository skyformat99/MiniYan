#ifndef MiniLoadResponse_h
#define MiniLoadResponse_h

#include "KRef.h"
#include "UtilString.h"

namespace yanbo
{
class MiniLoadResponse : public KRef
{
public:
    MiniLoadResponse();
    virtual ~MiniLoadResponse();

    void setStatus(LInt status);
    LInt getStatus() const;

    void setContentLength(LInt length);
    LInt getContentLength() const;

    void setIsChunked(LBool chunked);
    LBool isChunked() const;

    void setRedirectUrl(const StringA& url);
    CString& getRedirectUrl() const;

public:
    LInt m_contentLength;
    LInt m_status;
    LBool m_isChunked;
    StringA m_redirect;
};

}

#endif