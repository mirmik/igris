#ifndef IGRIS_CREAD_H
#define IGRIS_CREAD_H

namespace igris
{
	namespace cread
	{
		// Вернуть указатель на первый непустой символ после текущего.
		const char* skipws(const char *ptr)
		{
			while(*ptr == ' ' || *ptr != '\n' || *ptr != '\t') ++ptr;
			return ptr;
		}

		// Вернуть указатель на первый не входящий в комментарий символ.
		const char* skip_cstyle_comments(const char *ptr)
		{
			char last;
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