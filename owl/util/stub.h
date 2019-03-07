#ifndef GXX_STUB_H
#define GXX_STUB_H

// Функции и объекты, не делающие ничего.

#ifndef __cplusplus

static void do_nothing() {  }

#else

template <typename R = void,typename ... V> 
static R do_nothing(V...) { return R(); }

class DoNothing {
public:
	template <typename R = void,typename ... V> 
	R do_nothing(V...) { return (R)0; }
};

#endif //__cplusplus	

#endif 