#include <gxx/trent/gbson.h>
#include <gxx/io/std.h>

static ssize_t dump_numer(const gxx::trent& tr, char* buffer, size_t maxlen)
{
	(void) maxlen;
	long double d64 = tr.unsafe_numer_const();
	*buffer++ = (uint8_t)gxx::gbson::type::numer;
	memcpy(buffer, &d64, sizeof(d64));
	return 1 + sizeof(long double);
}

static ssize_t dump_integer(const gxx::trent& tr, char* buffer, size_t maxlen)
{
	(void) maxlen;
	int64_t i64 = tr.unsafe_integer_const();
	*buffer++ = (uint8_t)gxx::gbson::type::integer;
	memcpy(buffer, &i64, sizeof(i64));
	return 1 + sizeof(int64_t);
}

static ssize_t dump_string(const gxx::trent& tr, char* buffer, size_t maxlen)
{
	(void) maxlen;
	const auto& str = tr.unsafe_string_const();
	*buffer++ = (uint8_t)gxx::gbson::type::string;
	*buffer++ = (uint8_t)str.size();
	memcpy(buffer, str.data(), str.size());
	return 2 + str.size();
}

static ssize_t dump_list(const gxx::trent& tr, char* buffer, size_t maxlen)
{
	(void) maxlen;
	const auto& list = tr.unsafe_list_const();
	ssize_t ret = 2;

	*buffer++ = (uint8_t)gxx::gbson::type::list;
	*buffer++ = (uint8_t)list.size();

	for (const auto& trent : list)
	{
		ssize_t lret = gxx::gbson::dump(trent, buffer, maxlen - ret);

		if (lret < 0) return lret;

		ret += lret;
		buffer += lret;
	}

	return ret;
}

static ssize_t dump_dict(const gxx::trent& tr, char* buffer, size_t maxlen)
{
	(void) maxlen;
	const auto& dict = tr.unsafe_dict_const();
	ssize_t ret = 2;

	*buffer++ = (uint8_t)gxx::gbson::type::dict;
	*buffer++ = (uint8_t)dict.size();

	for (const auto& pair : dict)
	{
		const auto& name = pair.first;
		const auto& trent = pair.second;

		*buffer++ = (uint8_t) name.size();
		memcpy(buffer, name.data(), name.size());
		buffer += name.size();

		ret += 1 + name.size();

		ssize_t lret = gxx::gbson::dump(trent, buffer, maxlen - ret);

		if (lret < 0) return lret;

		ret += lret;
		buffer += lret;
	}

	return ret;
}


ssize_t gxx::gbson::dump(const gxx::trent& tr, char* buffer, size_t maxlen)
{
	(void) maxlen;
	ssize_t ret;
	gxx::trent::type type = tr.get_type();

	switch (type)
	{
		case gxx::trent::type::numer: ret = dump_numer(tr, buffer, maxlen); break;

		case gxx::trent::type::integer: ret = dump_integer(tr, buffer, maxlen); break;

		case gxx::trent::type::string: ret = dump_string(tr, buffer, maxlen); break;

		case gxx::trent::type::list: ret = dump_list(tr, buffer, maxlen); break;

		case gxx::trent::type::dict: ret = dump_dict(tr, buffer, maxlen); break;

		default: return gxx::GBSON_INTERNAL_ERROR;
	}

	return ret;
}

static ssize_t load_integer(gxx::trent& tr, const char* buffer, size_t maxlen)
{
	(void) maxlen;
	tr = *(int64_t*)++buffer;
	return 1 + sizeof(int64_t);
}

static ssize_t load_numer(gxx::trent& tr, const char* buffer, size_t maxlen)
{
	(void) maxlen;
	tr = *(long double*)++buffer;
	return 1 + sizeof(long double);
	return 0;
}

static ssize_t load_string(gxx::trent& tr, const char* buffer, size_t maxlen)
{
	(void) maxlen;
	uint8_t slen = *++buffer;
	const char* sptr = ++buffer;
	tr = gxx::buffer(sptr, slen);
	return 2 + slen;
}

static ssize_t load_list(gxx::trent& tr, const char* buffer, size_t maxlen)
{
	(void) maxlen;
	const char* start = buffer++;
	uint8_t size = *buffer++;

	tr.init(gxx::trent::type::list);

	for (int i = 0; i < size; ++i)
	{
		ssize_t lret = gxx::gbson::load(tr[i], buffer, maxlen);

		if (lret < 0) return lret;

		buffer += lret;
	}

	return buffer - start;

	return 0;
}

static ssize_t load_dict(gxx::trent& tr, const char* buffer, size_t maxlen)
{
	const char* start = buffer++;
	uint8_t size = *buffer++;

	tr.init(gxx::trent::type::dict);

	for (int i = 0; i < size; ++i)
	{
		uint8_t nlen = *buffer++;
		const char* nptr = buffer;
		buffer += nlen;

		ssize_t lret = gxx::gbson::load(tr[gxx::buffer(nptr, nlen)], buffer, maxlen);

		if (lret < 0) return lret;

		buffer += lret;
	}

	return buffer - start;
}

ssize_t gxx::gbson::load(gxx::trent& tr, const char* buffer, size_t maxlen)
{
	ssize_t ret;
	gxx::gbson::type type = gxx::gbson::type(*buffer);

	switch (type)
	{
		case gxx::gbson::type::numer: ret = load_numer(tr, buffer, maxlen); break;

		case gxx::gbson::type::integer: ret = load_integer(tr, buffer, maxlen); break;

		case gxx::gbson::type::string: ret = load_string(tr, buffer, maxlen); break;

		case gxx::gbson::type::list: ret = load_list(tr, buffer, maxlen); break;

		case gxx::gbson::type::dict: ret = load_dict(tr, buffer, maxlen); break;

		default: return gxx::GBSON_INTERNAL_ERROR;
	}

	return ret;
}
