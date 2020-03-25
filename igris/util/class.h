// Утилиты для анализа классов

namespace igris 
{
	//template <class T, class M> class __class_hasattr
	
	template <class T, class M>
	class __class_hasattr
	{
	private:
		template <class U>
		static decltype( 
				std::declval<U>().print_to(std::declval<nos_test_ostream&>())
			, std::true_type()) test(int);

		template <typename> static std::false_type test(...);

	public:
		static constexpr const bool value = decltype(test<T>(0))::value;
	};

	template<class T, class M>
	constexpr bool class_hasattr() 
	{
		//return __class_hasattr<T, M, mtd>::value;
	}
}