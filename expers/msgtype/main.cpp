#include <igris/dprint.h>
#include <igris/buffer.h>
#include <igris/protocols/msgtype.h>

int main() 
{
	uint8_t buf[1024];
	uint8_t* ptr = buf;
	uint8_t* eptr = buf + 1024;

	graw_write_f32(&ptr, eptr, 0.1);
	graw_write_f32(&ptr, eptr, 0.2);
	graw_write_f32(&ptr, eptr, 0.3);
	graw_write_i64(&ptr, eptr, 365);

	auto reader = igris::msgtype_read_type("pos3", "/home/mirmik/project/igris/msgtype/nav.msg");

	for (auto a: reader.tstruct) 
	{
		DPRINT(a.second.type);
		DPRINT(a.second.size);
		DPRINT(a.first.data());
	}

	auto msgmap = reader.map(buf, buf + 20);
}