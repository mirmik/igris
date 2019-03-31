#ifndef IGRIS_STRING_H
#define IGRIS_STRING_H

#include <cstdint>
#include <list>
#include <vector>
#include <string>

#include <igris/buffer.h>

namespace igris
{
	using strvec = std::vector<std::string>;
	using strlst = std::list<std::string>;

	strvec split(const std::string& str, char delim);

	std::string join(const strvec&, char delim);

	template<class Iter>
	std::string join(Iter start, Iter end, const char* delim, const char* prefix, const char* postfix)
	{
		std::string ret;

		size_t sz = 0;
		size_t tot = 0;

		for (Iter it = start; it != end; ++it)
		{
			sz += it->size(); 
			++tot;
		}

		ret.reserve(sz + strlen(prefix) + strlen(postfix) + tot * strlen(delim));

		ret.append(prefix);
		Iter it = start;
		for (unsigned int i = 0; i < tot; ++i) 
		{
			ret.append(*it++);
			ret.append(delim);
		}
		ret.append(*it);
		ret.append(postfix);

		return ret;
	}

	static inline std::string serialstr8(const std::string& str)
	{
		std::string ret;
		ret.push_back((uint8_t)str.size());
		ret.append(str);
		return ret;
	}

	static inline std::string serialstr8(const char* data, size_t size)
	{
		std::string ret;
		ret.push_back((uint8_t) size);
		ret.append(data, size);
		return ret;
	}
}

#endif