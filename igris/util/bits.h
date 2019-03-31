#ifndef GENOS_UTIL_BITS_H
#define GENOS_UTIL_BITS_H

//Установить все биты в переменной.	
#define bits_set_all(a) {a = -1;}

//Очистить все биты в переменной.
#define bits_clr_all(a) {a = 0;}

//#define bits_lvl_mask(a,m,l) (a = (l) ? ((a) | (m)) : ((a) & (~(m))))

//Установить все биты маски в переменную.
#define bits_set(a, b) {a |= (b);}

//Снять все биты маски в переменной.
#define bits_clr(a, b) {a &= ~(b);}

//Установить все биты маски в переменную согласно уровню.
#define bits_lvl(a, b, c) {a = ((c) ? ((a) | (b)) : ((a) & (~(b))));}
#define bits_rev(a) {a = ~(a);}

#define bits_mask(a,m) ((a) & (m))
#define bits_invmask(a,m) ((a) & (~(m)))

#define bits_mask_rev(a,m) {a ^= (m);}
#define bits_mask_eq(a,m,b) (((a) & (m)) == (b))

//Присвоить биты с предварительным наложением маски.
#define bits_mask_assign(a,m,b) (a = bits_invmask((a),(m)) | bits_mask((b),(m)))
#define bits_mask_assign_bias(a,m,b,s) (bits_mask_assign(a,((m)<<(s)),((b)<<(s))))


#define bits_multimap_uint32(input,clone,cllen) 	\
({													\
	uint32_t 			__ret = 0;					\
	uint16_t 			__input = (input);			\
	uint32_t 			__clone = (clone);			\
	uint8_t			 	__cllen = (cllen);			\
	while(__input) {								\
		uint8_t nbit = __builtin_ctz(__input);		\
		__ret |= (__clone << (cllen * nbit));		\
		__input &= ~(1 << nbit);					\
	}												\
	__ret;											\
})

#define bits_mask_assign_multimap_uint32(target,input,clone,cllen) 		\
({																		\
	uint16_t 			__input = (input);								\
	uint32_t 			__clone = (clone);								\
	uint8_t			 	__cllen = (cllen);								\
	uint32_t __or = bits_multimap_uint32(input,clone,cllen);			\
	uint32_t __full = 0;												\
	for (int i = 0; i < cllen; i++) {									\
		__full = (__full << 1) + 1;										\
	};																	\
	uint32_t __ia = bits_multimap_uint32(input,__full,cllen);			\
	bits_mask_assign(target, __ia, __or);								\
})

//		bit = __builtin_ctz(mask);
//
//		nmoder |= (1 << (bit*2));
//		mask &= ~(1 << bit);
//	

#endif