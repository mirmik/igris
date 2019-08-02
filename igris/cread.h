#ifndef IGRIS_CREAD_H
#define IGRIS_CREAD_H

namespace igris
{
	namespace cread
	{
		const char* skip_cstyle_comments(const char *ptr)
		{
			char fsymb = *ptr;
			char ssymb = *(ptr + 1);

			if (fsymb != '/')
				return nullptr;

			if (ssymb != '/' && ssymb != '*')
				return nullptr;

			ptr = ptr + 2;

			if (ssymb == '*')
			{
				char last = 0;
				while (!(*ptr == '/' && last == '*' ) && *ptr != '\0')
				{
					last = *ptr++;
				}
			}

			if (ssymb == '/')
			{
				while (*ptr != '\n' && *ptr != '\0')
				{
					last = *ptr++;
				}
			}

			if (*ptr != 0)
				++ptr;

			return ptr;
		}
	}
}

#endif