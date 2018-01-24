#ifndef StringBuffer_h
#define StringBuffer_h

#include "klist.h"
#include "UtilString.h"
#include "KRefPtr.h"

namespace yanbo
{
struct BufferItem;
class StringBuffer
{
public:
	StringBuffer();
	virtual ~StringBuffer();

public:
	void append(const char ch);
	void append(const CString& str);
	void append(const char* buffer, int pos, int len, LBool isConst);
	//void append(const char* buffer, int len, LBool isConst = LFalse);
	void append(const char* buffer);
	KRefPtr<CString> toString() const;
	void clear();

	LInt size() const;

private:
    KList<KRefPtr<BufferItem> > m_buffer;
    int m_length;
};
}
#endif
