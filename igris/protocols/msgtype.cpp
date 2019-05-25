#include <igris/protocols/msgtype.h>
#include <igris/dprint.h>

#include <igris/util/dstring.h>
#include <igris/util/string.h>
#include <igris/util/tokenizer.h>

#include <iostream>
#include <fstream>

std::map<std::string, igris::msgtype> msgtype_dict =
{
	{ "i8", igris::msgtype_i8 },
	{ "i16", igris::msgtype_i16 },
	{ "i32", igris::msgtype_i32 },
	{ "i64", igris::msgtype_i64 },

	{ "u8", igris::msgtype_u8 },
	{ "u16", igris::msgtype_u16 },
	{ "u32", igris::msgtype_u32 },
	{ "u64", igris::msgtype_u64 },

	{ "f32", igris::msgtype_f32 },
	{ "f64", igris::msgtype_f64 },

	{ "str", igris::msgtype_str },
	{ "longstr", igris::msgtype_longstr },
};

igris::msgtype_reader igris::msgtype_read_type(const std::string& type, const std::string& filepath)
{
	std::string line;
	std::string_view view;
	igris::msgtype_reader msgreader;

	auto file = std::fstream(filepath);

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
	}

	std::cout << "founded line: " << line << std::endl;

	while (1)
	{
		std::getline(file, line);

		if (file.eof() || line[0] != '\t')
		{
			dprln("endtype");
			return msgreader;
		}

		view = igris::trim(line);

		igris::tokenizer wreader(view.data(), view.size());
		igris::buffer word0 = wreader.getword();
		igris::buffer word1 = wreader.getword();

		if (word0.size() == 0 || word1.size() == 0)
		{
			dprln("format error");
			return msgreader;
		}

		msgreader.tstruct.emplace_back(std::make_pair((std::string)word1, msgtype_dict[(std::string)word0]));

		std::cout << line << std::endl;
	}

	return msgreader;
}