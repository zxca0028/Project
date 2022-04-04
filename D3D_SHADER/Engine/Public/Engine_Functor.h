#pragma once

BEGIN(Engine)

class CTagFinder
{
public:

	CTagFinder(std::string pTargetTag)
		: m_pTargetTag(pTargetTag)
	{
	}
	~CTagFinder() = default;

public:

	template <typename T>
	bool operator() (T& rPair)
	{
		if (rPair.first == m_pTargetTag)
		{
			return true;
		}

		return false;
	}

private:

	std::string m_pTargetTag = nullptr;

};

END