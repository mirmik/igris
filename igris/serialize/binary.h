#ifndef IGRIS_BINARY_SERIALIZER_H
#define IGRIS_BINARY_SERIALIZER_H

namespace igris
{
	template < class Archive >
	class archive_adaptor_load
	{
		int status = 0;
		Archive & archive;
		template < class Object > size_t operator &(Object & obj) { archive.load(object); }
	};

	template < class Archive >
	class archive_adaptor_dump
	{
		int status = 0;
		Archive & archive;
		template < class Object > size_t operator &(Object & obj) { archive.dump(object); }
	};

	template < class Archive >
	class archive_adaptor_size
	{
		int error = 0;
		unsigned int accum = 0;

		Archive & archive;

		template < class Object > size_t operator &(Object & obj)
		{
			if (error) return error;

			int ret = archive.size(object);
			if (ret < 0)
				error = ret;

			return ret;
		}

		unsigned int result()
		{
			if (error) return error;
			return accum;
		}
	};

	class basic_binary_archive
	{
		virtual int load_data(char* dat, uint16_t sz) = 0;
		virtual int dump_data(const char* dat, uint16_t sz) = 0;

		int dump(const char* dat, uint16_t sz)
		{
			int ret;
			if ((ret = dump(sz)) < 0) return ret;
			if ((ret = dump_data(dat, sz)) < 0) return ret;
		}

		int dump(char i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(short i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(int i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(long i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(unsigned char i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(unsigned short i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(unsigned int i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(unsigned long i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(unsigned long long i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(float i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(double i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(long double i) { return dump_data((char*)&i, sizeof(i)); }
		int dump(igris::buffer buf) { return dump(buf.data(), buf.size()); }

		template<typename T>
		int dump(const T& ref)
		{
			archive_adaptor_dump adaptor {*this};
			((std::remove_cv_t<std::remove_reference_t<T>>&)(ref)).reflect(adaptor);
		}

		int load(char* dat, uint16_t maxsz)
		{
			uint16_t sz;
			load(sz);
			assert(sz <= maxsz);
			load_data(dat, sz);
		}

		int load(char& i) { return load_data((char*)&i, sizeof(i)); }
		int load(short& i) { return load_data((char*)&i, sizeof(i)); }
		int load(int& i) { return load_data((char*)&i, sizeof(i)); }
		int load(long& i) { return load_data((char*)&i, sizeof(i)); }
		int load(unsigned char& i) { return load_data((char*)&i, sizeof(i)); }
		int load(unsigned short& i) { return load_data((char*)&i, sizeof(i)); }
		int load(unsigned int& i) { return load_data((char*)&i, sizeof(i)); }
		int load(unsigned long& i) { return load_data((char*)&i, sizeof(i)); }
		int load(unsigned long long& i) { return load_data((char*)&i, sizeof(i)); }
		int load(float& i) { return load_data((char*)&i, sizeof(i)); }
		int load(double& i) { return load_data((char*)&i, sizeof(i)); }
		int load(long double& i) { return load_data((char*)&i, sizeof(i)); }
		//int load(igris::buffer buf) { lo }

		template<typename T>
		int load(T&& ref)
		{
			archive_adaptor_load adaptor {*this};
			((std::remove_cv_t<std::remove_reference_t<T>>&)(ref)).reflect(adaptor);
			return
		}

		void size(char& i) { return load_data((char*)&i, sizeof(i)); }
		void size(short& i) { return load_data((char*)&i, sizeof(i)); }
		void size(int& i) { return load_data((char*)&i, sizeof(i)); }
		void size(long& i) { return load_data((char*)&i, sizeof(i)); }
		void size(unsigned char& i) { return load_data((char*)&i, sizeof(i)); }
		void size(unsigned short& i) { return load_data((char*)&i, sizeof(i)); }
		void size(unsigned int& i) { return load_data((char*)&i, sizeof(i)); }
		void size(unsigned long& i) { return load_data((char*)&i, sizeof(i)); }
		void size(unsigned long long& i) { return load_data((char*)&i, sizeof(i)); }
		void size(float& i) { return load_data((char*)&i, sizeof(i)); }
		void size(double& i) { return load_data((char*)&i, sizeof(i)); }
		void size(long double& i) { return load_data((char*)&i, sizeof(i)); }
		//int load(igris::buffer buf) { lo }

		template<typename T>
		void size(T&& ref)
		{
			archive_adaptor_size adaptor {*this};
			((std::remove_cv_t<std::remove_reference_t<T>>&)(ref)).reflect(adaptor);
			return adaptor.result();
		}

	};
}

#endif