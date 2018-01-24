#ifndef ListMap_h
#define ListMap_h

#include "klist.h"
#include "KRef.h"
#include "KRefPtr.h"
#include "SalLog.h"

namespace util
{

template<class Key, class Value>
class ObjectPair : public KRef
{
public:
	ObjectPair(const Key& k, const Value& v)
	    : mKey(k)
	    , mValue(v)
	{
	}
	virtual ~ObjectPair() { KLOG("~ObjectPair"); }

    Key    mKey;
    Value  mValue;

	Key getKey() const
	{
		return mKey;
	}

	Value getValue() const
	{
		return mValue;
	}
};

template<class Key, class Value>
class KListMap
{
public:
	typedef ObjectPair<Key, Value> MapPair;
	typedef KList<KRefPtr<MapPair> > MapContainer;
	typedef ListIterator<KRefPtr<MapPair> > Iterator;

public:
	KListMap() {}
	~KListMap()
	{
		clear();
	}

	Iterator begin() const {
		return m_container.begin();
	}

	Iterator end() const {
		return m_container.end();
	}

	LVoid put(const Key& k, const Value& v)
	{
		Iterator iter = m_container.begin();
		Iterator iterEnd = m_container.end();

		bool find = false;
		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->mKey == k)
			{
				(*iter)->mValue = v;
				return;
			}
		}

		m_container.push_back(new MapPair(k, v));
	}

	LVoid clear()
	{
		m_container.clear();
	}

	LInt size()
	{
		m_container.count();
	}

	LBool isEmpty()
	{
		return m_container.empty();
	}

	Value operator[](const Key& key) const
	{
		KLOG("operator[] 1");
		MapPair* pair = get(key);
		KFORMATLOG("pair count=%d", pair->count());
		return pair->mValue;
	}

	Value& operator[](const Key& key)
	{
		KLOG("operator[] 2");
		MapPair* pair = get(key);
		KFORMATLOG("operator pair count=%d", pair->count());
		return pair->mValue;
	}

private:
	MapPair* get(const Key& k)
	{
		Iterator iter = m_container.begin();
		Iterator iterEnd = m_container.end();
		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->mKey == k)
			{
				return (*iter).get();
			}
		}

		MapPair* pair = new MapPair(k, Value());
		KFORMATLOG("get pair count=%d", pair->count());
		m_container.push_back(pair);
		KFORMATLOG("get pair end count=%d", pair->count());
		return pair;
	}

private:
	MapContainer m_container;
};

}

using util::KListMap;
#endif
