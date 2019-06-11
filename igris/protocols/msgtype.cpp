#include <igris/protocols/msgtype.h>
#include <igris/dprint.h>

#include <igris/util/dstring.h>
#include <igris/util/string.h>
#include <igris/util/tokenizer.h>

#include <iostream>
#include <fstream>

std::map<std::string, igris::msgtype*> igris::base_msgtypes =
{
	{ "i8", (igris::msgtype*)&igris::msgtype_i8 },
	{ "i16", (igris::msgtype*)&igris::msgtype_i16 },
	{ "i32", (igris::msgtype*)&igris::msgtype_i32 },
	{ "i64", (igris::msgtype*)&igris::msgtype_i64 },

	{ "u8", (igris::msgtype*)&igris::msgtype_u8 },
	{ "u16", (igris::msgtype*)&igris::msgtype_u16 },
	{ "u32", (igris::msgtype*)&igris::msgtype_u32 },
	{ "u64", (igris::msgtype*)&igris::msgtype_u64 },

	{ "f32", (igris::msgtype*)&igris::msgtype_f32 },
	{ "f64", (igris::msgtype*)&igris::msgtype_f64 },

	{ "str", (igris::msgtype*)&igris::msgtype_str },
	{ "longstr", (igris::msgtype*)&igris::msgtype_longstr },
};

std::unique_ptr<igris::msgtype> make_str_msgtype() 
{
	return std::make_unique<igris::msgtype_string>(2);
}

std::unique_ptr<igris::msgtype> make_longstr_msgtype() 
{
	return std::make_unique<igris::msgtype_string>(4);
}

std::map<std::string, std::unique_ptr<igris::msgtype>(*)()> 
constructors_msgtypes =
{
	{ "i8", &igris::msgtype_integer<int8_t>::make_unique },
	{ "i16", &igris::msgtype_integer<int16_t>::make_unique },
	{ "i32", &igris::msgtype_integer<int32_t>::make_unique },
	{ "i64", &igris::msgtype_integer<int64_t>::make_unique },

	{ "u8", &igris::msgtype_integer<uint8_t>::make_unique },
	{ "u16", &igris::msgtype_integer<uint16_t>::make_unique },
	{ "u32", &igris::msgtype_integer<uint32_t>::make_unique },
	{ "u64", &igris::msgtype_integer<uint64_t>::make_unique },

	{ "f32", &igris::msgtype_float<float32_t>::make_unique },
	{ "f64", &igris::msgtype_float<float64_t>::make_unique },

	{ "str", make_str_msgtype },
	{ "longstr", make_longstr_msgtype }
};


igris::msgtype_struct igris::msgtype_read_type(const std::string& type, const std::string& filepath)
{
	std::string line;
	std::string_view view;
	igris::msgtype_struct msgreader;

	/*auto file = std::fstream(filepath);

	if (!file.good())
	{
		perror("open");
		return msgreader;
	}

	while (1)
	{
		std::getline(file, line);

		if (igris::trim_right(line) == type)
			break;

		if (file.eof())
		{
			dprln("not found");
			return msgreader;
		}
	}*/

	//std::cout << "founded line: " << line << std::endl;

	msgreader.readtype(type);

	nos::println(msgreader);

	return msgreader;
}

#include <nos/print.h>

std::pair<std::string, std::string> type_to_name_and_mod(const std::string& str) 
{
	std::string name;
	std::string mod;

	char *ptr = (char*)str.data();

	while (isalpha(*ptr) || isdigit(*ptr)) 
		++ptr;

	name = str.substr(0, ptr - str.data());

	while (*ptr != 0) 
	{
		if (*ptr == '[') 
			mod += 'a';
		++ptr;
	}

	return std::make_pair(name, mod);
} 

/*std::vector<igris::msgtype_object> igris::type_objectize(const std::string& str) 
{
	std::vector<igris::msgtype_object> ret;
	auto v = igris::split(str, "\n,");

	for(auto& n : v) 
	{
		auto w = igris::split(n, ':');
		auto name_and_mod = type_to_name_and_mod(w[0]);

		ret.emplace_back(
			name_and_mod.first, 
			w.size() > 1 ? w[1] : "", 
			name_and_mod.second);		
	}

	return ret;*/


std::unique_ptr<igris::msgtype> make_msgtype_for(const std::string & str) 
{
	if (constructors_msgtypes.count(str)) 
	{
		return constructors_msgtypes[str]();
	}	

	else 
	{
		dprln("undefined msgtype:", str.data());
		return nullptr;
	}
}

std::unique_ptr<igris::msgtype> make_msgtype_for_mod(
	char c, 
	std::unique_ptr<igris::msgtype>&& base) 
{
	switch (c) 
	{
		case 'a': return std::make_unique<igris::msgtype_array>(std::move(base));
		default : BUG();
	}
	//return std::move(base);
}

int igris::msgtype_struct::readtype(const std::string & typestr) 
{
	auto v = igris::split(typestr, "\n,");

	for(auto& n : v) 
	{
		auto w = igris::split(n, ':');
		auto name_and_mod = type_to_name_and_mod(w[0]);

		std::unique_ptr<igris::msgtype> m = make_msgtype_for(name_and_mod.first);
		if (m == nullptr)
			return -1;

		for (auto c : name_and_mod.second) 
		{
			m = make_msgtype_for_mod(c, std::move(m));
		}

		objects.emplace_back(w.size() > 1 ? w[1] : "", std::move(m));
	}

	return 0;
}