//
// Created by yanbo on 2017/8/29.
//

#include "MiniLoadResponse.h"

namespace yanbo
{
MiniLoadResponse::MiniLoadResponse()
    : m_status(0)
    , m_isChunked(LFalse)
    , m_contentLength(0)
{

}

MiniLoadResponse::~MiniLoadResponse()
{

}

void MiniLoadResponse::setStatus(LInt status)
{
    m_status = status;
}

LInt MiniLoadResponse::getStatus() const
{
    return m_status;
}

void MiniLoadResponse::setContentLength(LInt length)
{
    m_contentLength = length;
}

LInt MiniLoadResponse::getContentLength() const
{
    return m_contentLength;
}

void MiniLoadResponse::setIsChunked(LBool chunked)
{
    m_isChunked = chunked;
}

LBool MiniLoadResponse::isChunked() const
{
    return m_isChunked;
}

void MiniLoadResponse::setRedirectUrl(const StringA& url)
{
    m_redirect = url;
}
}