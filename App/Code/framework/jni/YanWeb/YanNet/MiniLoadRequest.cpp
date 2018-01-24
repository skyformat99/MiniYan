#include "MiniLoadRequest.h"

namespace yanbo
{
MiniLoadRequest::MiniLoadRequest()
{
	m_url = (LCharA*)NULL;
	m_headers.clear();
	m_params.clear();
	m_method = EGet;
}

MiniLoadRequest::~MiniLoadRequest()
{
}

void MiniLoadRequest::resetRequest()
{
	m_url = (LCharA*)NULL;
	m_headers.clear();
	m_params.clear();
	m_method = EGet;
}

}
