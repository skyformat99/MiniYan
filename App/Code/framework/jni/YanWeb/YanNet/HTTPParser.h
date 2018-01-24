//
// Created by yanbo on 2017/8/28.
//

#ifndef Mini_HTTPParser_h
#define Mini_HTTPParser_h

#include "StringBuffer.h"
#include "KRefPtr.h"
#include "MiniLoadResponse.h"
#include "MiniLoaderCallback.h"

namespace yanbo
{
class HTTPParser
{
public:
    HTTPParser(MiniLoaderCallback* callback);
    ~HTTPParser();

public:
    LBool parseResponse(const char* data, int length);
    // 解析HTTP头部
    void parseResponseHeader();
    // 解析HTTP正文，主要用于chuncked解析
    LBool parseResponseBody(const char* data, int begin, int length);
    // 判断回车换行标记
    void execHeaderEndByte(const char ch);

private:
    StringBuffer* m_buffer;
    // m_headerEnd作用如下：
    // 1, /r; 2, /n; 3, /r; 4, /n
    LInt m_headerEnd;

    KRefPtr<MiniLoadResponse> m_response;

    LBool m_chunkedFlag;
    LInt m_currentChunkSize;
    LInt m_currentChunkLength;
    MiniLoaderCallback* m_callback;
};
}

#endif //
