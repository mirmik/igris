#ifndef ITERATION_COUNTER_H
#define ITERATION_COUNTER_H

#define INTERNAL_VAR __internal_var

//Счетчики итераций. 

//Вспомогательные функции.
#define _concate(a, b) __concate(a, b)
#define __concate(a, b) a ## b

//Выполнить однократно.
#define only_once \
static int _concate(INTERNAL_VAR, __LINE__) = 1; \
if (_concate(INTERNAL_VAR, __LINE__)) if (_concate(INTERNAL_VAR, __LINE__)--) 

//Выполнять b итераций.
#define do_iteration(b) \
static int _concate(INTERNAL_VAR, __LINE__) = 0; \
if (_concate(INTERNAL_VAR, __LINE__) < b) if (++_concate(INTERNAL_VAR, __LINE__)) 

//Выполнять после b итераций (невключительно).
#define do_after_iteration(b) static int _concate(INTERNAL_VAR, __LINE__) = 0; \
if (_concate(INTERNAL_VAR, __LINE__) < b) ++_concate(INTERNAL_VAR, __LINE__); else

//Выполнять от a итерации до b итерации (a включительно, b невключительно).
#define do_between_iteration(a, b) static int _concate(INTERNAL_VAR, __LINE__) = 0; \
if (_concate(INTERNAL_VAR, __LINE__) < a) ++_concate(INTERNAL_VAR, __LINE__); else \
if (_concate(INTERNAL_VAR, __LINE__) < b) if (++_concate(INTERNAL_VAR, __LINE__))

//Выполнять после only_once, do_iteration, do_between_iteration
#define do_after else;else

#undef INTERNAL_VAR

//Структура счётчик инструкций.
/*struct iteration_counter
{
	long long iteration;
	long long helper;
};

#define iter_start(obj) \
static iteration_counter obj = {0, 0}; \
obj.iteration++;\
switch(obj.helper){case 0:

//
#define iter(obj,a) if (obj.iteration == a) obj.helper = a; break; case a:

//Завершение процессора.
#define iter_end(obj,a) if (obj.iteration == a) obj.helper = a; break; default: break; }

//Неостанавливающие завершение.
#define iter_end_unstoped(obj) if (obj.iteration >= obj.helper) obj.iteration = obj.helper; break;}*/

//Формат использования.
//iter_start(obj)     do_something0();	//выполнять до второй итерации 
//iter(obj,2)         do_something1();	//выполнять со второй до четвертой итерации
//iter(obj,4)         do_something2();	//выполнять с четвертой по десятую итерации
//iter_end(obj,10); 
	
#endif