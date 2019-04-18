#ifndef NOS_UTIL_MACRO_H
#define NOS_UTIL_MACRO_H

#define ARGS_INVOKE_FOR_EACH_9(I,a,b,c,d,e,f,g,h)   I(a);I(b);I(c);I(d);I(e);I(f);I(g);I(h);
#define ARGS_INVOKE_FOR_EACH_8(I,a,b,c,d,e,f,g)     I(a);I(b);I(c);I(d);I(e);I(f);I(g);
#define ARGS_INVOKE_FOR_EACH_7(I,a,b,c,d,e,f)       I(a);I(b);I(c);I(d);I(e);I(f);
#define ARGS_INVOKE_FOR_EACH_6(I,a,b,c,d,e)         I(a);I(b);I(c);I(d);I(e);
#define ARGS_INVOKE_FOR_EACH_5(I,a,b,c,d)           I(a);I(b);I(c);I(d);
#define ARGS_INVOKE_FOR_EACH_4(I,a,b,c)             I(a);I(b);I(c);
#define ARGS_INVOKE_FOR_EACH_3(I,a,b)               I(a);I(b);
#define ARGS_INVOKE_FOR_EACH_2(I,a)                 I(a)

#define ARGS_INVOKE_FOR_EACH(...) CONCAT2(ARGS_INVOKE_FOR_EACH_,COUNT_ARGS(__VA_ARGS__))(__VA_ARGS__)

#define STRINGIFY(...) #__VA_ARGS__
#define STRINGIFY2(...) STRINGIFY(__VA_ARGS__)

#define CONCAT(a,b) a##b
#define CONCAT2(a,b) CONCAT(a,b)

#define ELEVENTH_ARGUMENT(a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, ...) a11
#define COUNT_ARGS(...) ELEVENTH_ARGUMENT(dummy, ## __VA_ARGS__, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)

#endif