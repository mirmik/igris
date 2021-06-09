#ifndef igris_JSON_SETTINGS_H
#define igris_JSON_SETTINGS_H

/**
    @file
*/

#include <fstream>
#include <sstream>
#include <string>

#include <igris/trent/json.h>
#include <igris/trent/settings.h>

#include <nos/io/file.h>

#include <exception>

namespace igris
{
    class json_settings : public trent_settings
    {
        std::string pathstr;

      public:
        const std::string & path() {return pathstr;}
        void path(const std::string & str) { pathstr = str;}

        json_settings() = default;
        json_settings(const std::string &str) : pathstr(str){};

      private:
        void load()
        {
            std::fstream file(pathstr);
            if (!file.good())
            {
                perror("json_settings::load:");
                return;
            }
            std::stringstream file_contents;
            file_contents << file.rdbuf();
            tr = json::parser_str(file_contents.str()).parse();
            file.close();
            synced = true;
        }

      public:
        void sync() override { load(); }

        void save() override
        {
            // std::fstream file(pathstr, std::ios_base::trunc | std::ios::out);
            // if (!file.good())
            //{
            //	BUG();
            //}
            nos::file file(pathstr.c_str(), O_WRONLY);
            json::pretty_print_to(tr, file);
            // file.close();
        }
    };

    class json_syncer : public trent_syncer
    {
        igris::trent tr;
        bool synced = false;

        std::string pathstr;

      public:        
        const std::string & path() {return pathstr;}
        void path(const std::string & str) { pathstr = str;}


        json_syncer() = default;
        json_syncer(const std::string &str) : pathstr(str) {}

      private:
        void load()
        {
            std::fstream file(pathstr);
            if (!file.good())
            {
                perror("json_syncer::load:");
                throw std::logic_error("file isn't exist");
            }
            std::stringstream file_contents;
            file_contents << file.rdbuf();
            tr = json::parser_str(file_contents.str()).parse();
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
            // std::fstream file(pathstr, std::ios_base::trunc | std::ios::out);
            // if (!file.good())
            //{
            //	BUG();
            //}
            nos::file file(pathstr.c_str(), O_WRONLY);
            json::pretty_print_to(tr, file);
            // file.close();
            return 0;
        }

        igris::trent &node() { return tr; }
    };
}

#endif
