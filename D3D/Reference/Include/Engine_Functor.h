#pragma once

BEGIN(Engine)

class CTagFinder
{
public:

	CTagFinder(const _tchar* pTargetTag)
		: m_pTargetTag(pTargetTag){}
	~CTagFinder() = default;

public:

	template <typename T>
	bool operator() (T& rPair)
	{
		if (0 == lstrcmp(rPair.first, m_pTargetTag))
		{
			return true;
		}

		return false;
	}

private:

	const _tchar* m_pTargetTag = nullptr;

};

END