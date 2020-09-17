#ifndef GXX_INVOKER_H
#define GXX_INVOKER_H

#include <gxx/event/delegate.h>
#include <gxx/util/tuple.h>
#include <gxx/util/signature.h>
#include <tuple>

namespace gxx {
	template <typename Ret, typename ... Args>
	class deffered{
	public:
		gxx::delegate<Ret, Args ...> dlg;
		std::tuple<Args ...> args;

		template <typename ... UArgs>
		deffered(const gxx::delegate<Ret, Args...>& dlg, UArgs ... args) : dlg(dlg), args(std::forward<UArgs>(args) ...) {}

		Ret invoke() {
			return gxx::apply(dlg, args); 
		}
	};

	template <typename Func, typename ... UArgs>
	deffered<typename gxx::signature<Func>::return_type, std::remove_reference_t<UArgs> ...> make_deffered(Func&& f, UArgs ... args) {
		return deffered<
			typename gxx::signature<Func>::return_type, 
			std::remove_reference_t<UArgs> ...
		>(gxx::make_delegate(std::forward<Func>(f)), std::forward<UArgs>(args) ...);
	}

	template <typename Ret>
	struct basic_vdeffered {
		virtual Ret invoke() = 0;
	};

	template <typename Ret, typename ... Args>
	class vdeffered : public basic_vdeffered<Ret> {
	public:
		gxx::delegate<Ret, Args ...> dlg;
		std::tuple<Args ...> args;

		template <typename ... UArgs>
		vdeffered(const gxx::delegate<Ret, Args...>& dlg, UArgs ... args) : dlg(dlg), args(std::forward<UArgs>(args) ...) {}

		Ret invoke() {
			return gxx::apply(dlg, args); 
		}
	};

	template <typename Func, typename ... UArgs>
	vdeffered<typename gxx::signature<Func>::return_type, std::remove_reference_t<UArgs> ...> make_vdeffered(Func&& f, UArgs ... args) {
		return vdeffered<
			typename gxx::signature<Func>::return_type, 
			std::remove_reference_t<UArgs> ...
		>(gxx::make_delegate(std::forward<Func>(f)), std::forward<UArgs>(args) ...);
	}
}

#endif