#ifndef IGRIS_TRENT_SETTINGS_H
#define IGRIS_TRENT_SETTINGS_H

/**
    @file
*/

#include <igris/trent/trent.h>
#include <igris/trent/trent_path.h>

namespace igris
{

    class trent_syncer
    {
    public:
        virtual int sync() = 0;
        virtual int save() = 0;
        virtual igris::trent &node() = 0;
        virtual ~trent_syncer() = default;
    };

    class trent_syncer_slice : public trent_syncer
    {
    public:
        igris::trent_syncer *syncer;
        igris::trent_path path;

        trent_syncer_slice(igris::trent_syncer &stgs,
                           const igris::trent_path &path)
            : syncer(&stgs), path(path)
        {
        }

        trent_syncer_slice(){};

        void init(igris::trent_syncer &stgs, const igris::trent_path &path)
        {
            syncer = &stgs;
            this->path = path;
        }

        int sync() override { return syncer->sync(); }

        int save() override { return syncer->save(); }

        igris::trent &node() override { return syncer->node()[path]; }
    };

    class trent_settings
    {
    public:
        igris::trent tr;
        bool synced = false;

        virtual void sync() = 0;
        virtual void save() = 0;

        igris::trent &node() { return tr; }
        const igris::trent &node() const { return tr; }
        virtual ~trent_settings() = default;
    };

    class trent_settings_slice : public trent_settings
    {
    public:
        igris::trent_settings &settings;
        igris::trent_path path;

        trent_settings_slice(igris::trent_settings &stgs,
                             const igris::trent_path &path)
            : settings(stgs), path(path)
        {
        }

        void sync() override
        {
            if (!settings.synced)
            {
                settings.sync();
            }
            tr = settings.node()[path];
            synced = true;
        }

        void save() override
        {
            settings.node()[path] = tr;
            settings.save();
        }
    };

    class settings_binder_int64 : public trent_settings_slice
    {
    public:
        settings_binder_int64(trent_settings &base, const trent_path &name)
            : trent_settings_slice(base, name)
        {
        }

        void sync_default(int64_t def)
        {
            sync();
            if (node().is_nil())
                node() = def;
        }

        settings_binder_int64 &operator=(int64_t i)
        {
            node() = i;
            return *this;
        }

        operator int64_t() const { return node().as_integer(); }
    };

    class settings_binder_int32 : public trent_settings_slice
    {
    public:
        settings_binder_int32(trent_settings &base, const trent_path &name)
            : trent_settings_slice(base, name)
        {
        }

        void sync_default(int32_t def)
        {
            sync();
            if (node().is_nil())
                node() = def;
        }

        settings_binder_int32 &operator=(int32_t i)
        {
            node() = i;
            return *this;
        }

        operator int32_t() const { return node().as_integer(); }
    };

}

#endif
