#ifndef GXX_JSON_SETTINGS_H
#define GXX_JSON_SETTINGS_H

#include <string>
#include <sstream>
#include <fstream>

#include <gxx/trent/json.h>
#include <gxx/trent/settings.h>

#include <exception>

namespace gxx
{
	class json_settings : public trent_settings
	{
		std::string pathstr;

	public:
		ACCESSOR(path, pathstr);

		json_settings() = default;
		json_settings(const std::string& str) : pathstr(str) {};

	private:
		void load()
		{
			std::fstream file(pathstr);
			if (!file.good()) return;
			std::stringstream file_contents;
			file_contents << file.rdbuf();
			tr = json::parse(file_contents).unwrap();
			file.close();
			synced = true;
		}

	public:
		void sync() override
		{
			load();
		}

		void save() override
		{
			std::fstream file(pathstr, std::ios_base::trunc | std::ios::out);
			if (!file.good())
			{
				PANIC_TRACED();
			}
			json::pretty_print_to(tr, file);
			file.close();
		}
	};

	class json_syncer : public trent_syncer
	{
		gxx::trent tr;
		bool synced = false;

		std::string pathstr;

	public:
		ACCESSOR(path, pathstr);

		json_syncer() = default;
		json_syncer(const std::string& str) : pathstr(str) {}

	private:
		void load()
		{
			std::fstream file(pathstr);
			if (!file.good()) {
				throw std::logic_error("file isn't exist");
			}
			std::stringstream file_contents;
			file_contents << file.rdbuf();
			tr = json::parse(file_contents).unwrap();
			file.close();
			synced = true;
		}

	public:
		int sync() override
		{
			if (synced)
				return 0;
			load();
			return 0;
		}

		int save() override
		{
			std::fstream file(pathstr, std::ios_base::trunc | std::ios::out);
			if (!file.good())
			{
				PANIC_TRACED();
			}
			json::pretty_print_to(tr, file);
			file.close();
			return 0;
		}

		gxx::trent& node() 
		{
			return tr;
		}
	};
}

#endif
