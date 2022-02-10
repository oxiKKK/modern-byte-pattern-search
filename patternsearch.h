#ifndef PATTERN_SEARCH_H
#define PATTERN_SEARCH_H

#pragma once

class CBytePattern
{
public:
	template<uint32_t N> requires(N < 256)
	CBytePattern(const char(&szPattern)[N], uint32_t dwOffset = 0x0) :
		m_dwOffset(dwOffset)
	{
		m_szMask = GenerateMaskFromPattern(szPattern);

		// Does count the nullcharacter if we specify the length.
		m_szPattern = std::string(szPattern, N);
	}

	CBytePattern(const CBytePattern&) = default;
	CBytePattern(CBytePattern&&) = default;

public:
	template<uint32_t N> requires(N < 256)
	static inline auto GenerateMaskFromPattern(const char(&szPattern)[N])
	{
		std::string a;
		for (uint32_t i = 0; i < N; i++)
			a.push_back(szPattern[i] == '\x00' ? '?' : 'x');
		return a;
	}

	inline const auto Pattern() const { return m_szPattern; }
	inline const auto Mask() const { return m_szMask; }
	inline const auto Offset() const { return m_dwOffset; }
	inline const auto Length() const { return m_szMask.length(); }

	// Searches for a byte pattern within a range [dwStart, dwEnd]
	uint32_t* Search(uint32_t dwStart, uint32_t dwEnd)
	{
		bool bFound = false;
		uint32_t i = dwStart;
		for (; i < dwEnd - Length(); i++)
		{
			bFound = true;
			for (uint32_t k = 0; k < Length(); k++)
			{
				if (m_szMask[k] == 'x' && m_szPattern[k] != *reinterpret_cast<char*>(i + k))
				{
					bFound = false;
					break;
				}
			}

			if (bFound)
				return reinterpret_cast<uint32_t*>(i + Offset());
		}
	
		return nullptr;
	}

private:
	std::string m_szPattern, m_szMask;
	uint32_t m_dwOffset;
};

#endif