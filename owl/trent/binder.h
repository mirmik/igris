#ifndef GXX_TRENT_BINDER_H
#define GXX_TRENT_BINDER_H

#include <gxx/trent/trent.h>
#include <gxx/trent/settings.h>

namespace gxx 
{
	template <typename T> struct trent_binder_ops {};

	template <typename Integer> 
	struct trent_binder_ops_integer {
		static Integer from_trent(const gxx::trent& tr) { return tr.as_integer(); }
		static void set_trent(gxx::trent& tr, const Integer& val) { tr = val; }
	};
	
	template <typename Float> 
	struct trent_binder_ops_float {
		static Float from_trent(const gxx::trent& tr) { return tr.as_numer(); }
		static void set_trent(gxx::trent& tr, const Float& val) { tr = val; }
	};
	
	template <> struct trent_binder_ops<int32_t> : public trent_binder_ops_integer<int32_t> {};
	template <> struct trent_binder_ops<int64_t> : public trent_binder_ops_integer<int64_t> {};

	template <> struct trent_binder_ops<float> : public trent_binder_ops_float<float> {};
	template <> struct trent_binder_ops<double> : public trent_binder_ops_float<double> {};

	template <> struct trent_binder_ops<std::string> 
	{
		static std::string from_trent(const gxx::trent& tr) { return tr.as_string(); }
		static void set_trent(gxx::trent& tr, const std::string& val) { tr = val; }	
	};

	template < typename T >
	class trent_binder
	{
		mutable trent_syncer_slice saver;
		trent_binder_ops<T> ops;	

		mutable bool synced = false;
		mutable T _local;

	public:
		trent_binder(){};

		template <typename ... Args>
		trent_binder(Args && ... args) : saver(std::forward<Args>(args) ...) 
		{
			sync();
		}
	
		template <typename ... Args>
		void init(Args && ... args) 
		{
			saver.init(args ...);
		}

		void sync() const
		{
			saver.sync();
			_local = ops.from_trent(saver.node());
			synced = true;
		}

		void sync_default(T defa)
		{
			saver.sync();

			if (saver.node().is_nil()) 
			{
				update(defa);
				synced = true;
				return;
			}

			_local = ops.from_trent(saver.node());
			synced = true;
		}

		void update(const T& val) 
		{
			_local = val;
			ops.set_trent(saver.node(), _local);
			saver.save();
		}		

		T get() const
		{
			if (!synced) 
			{
				sync();
			} 

			return _local;
		}
	};	
}

#endif