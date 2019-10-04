#ifndef GXX_RPC_INVOKER_H
#define GXX_RPC_INVOKER_H

#include <gxx/serialize/serialize.h>
#include <gxx/event/delegate.h>
#include <gxx/rpc/status.h>
#include <gxx/util/tuple.h>

namespace gxx {
	namespace rpc {
		template <typename Input, typename Output>
		struct basic_invoker {
			virtual status invoke(Input&, Output&) = 0;
		};

		template <typename Input, typename Output, typename Ret, typename ... Args> 
		struct invoker : public basic_invoker<Input, Output> {
			gxx::delegate<Ret, Args ...> dlg;

			status invoke(Input& inp, Output& out) {
				std::tuple<Args ...> args;
				gxx::deserialize(inp, args);
				Ret ret = gxx::apply(dlg, args);

				if (ret.is_error()) {
					gxx::serialize(out, ret.getError());
					return ret.getError();
				} else {
					gxx::serialize(out, status::OK);
					gxx::serialize(out, ret.getData());
					return status::OK;		
				}
			}

			template<typename Func>
			invoker(Func&& f) : dlg(gxx::make_delegate(std::forward<Func>(f))) {}
		};
	}
}

#endif