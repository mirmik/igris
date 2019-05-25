#ifndef IGRIS_TOKENIZER_H
#define IGRIS_TOKENIZER_H

namespace igris 
{
	struct tokenizer 
	{
		const char* ptr;
		const char* eptr;

		const char* space;

		tokenizer(const char* data, size_t size) : ptr(data), eptr(data + size), space(" \t\n") {}

		bool isend() 
		{
			return ptr == eptr;
		}

		void skip_space() 
		{
			while(ptr != eptr) 
			{
				const char* sp = space;
				char c = *ptr;

				while(c != *sp && *sp != 0) 
					++sp;

				if (*sp == 0) 
					break;

				++ptr;
				//continue
			} 
		}

		void skip_word() 
		{
			while(ptr != eptr) 
			{
				const char* sp = space;
				char c = *ptr;

				while(c != *sp && *sp != 0) 
					++sp;

				if (*sp != 0)
					break; 

				++ptr;
				//continue
			} 
		}

		igris::buffer getword() 
		{
			const char* strt;
			const char* fini;

			skip_space();
			strt = ptr;

			skip_word();
			fini = ptr;

			return igris::buffer(strt, fini - strt);
		}
	};
}

#endif