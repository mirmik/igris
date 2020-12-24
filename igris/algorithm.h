#ifndef igris_ALGORITHM_H
#define igris_ALGORITHM_H

namespace igris{
	template<class InputIterator, class Function>  
	Function for_each_safe(InputIterator first, InputIterator last, Function f) {
		InputIterator next(first); 
		++next;
		
		while(first !=last){
			f(*first);
			first = next;
			++next;
		}
		return f;
	}
}

#endif
