#include <igris/dprint.h>
#include <igris/buffer.h>
#include <igris/protocols/msgtype.h>

int main() 
{
	int ans;

	uint8_t buf[1024];
	uint8_t * ptr = buf;
	uint8_t * endp = buf + 1024;	

	ans = graw_write_i64(&ptr, endp, 1);
	ans = graw_write_i32(&ptr, endp, 2);
	ans = graw_write_f32(&ptr, endp, 1);
	ans = graw_write_str(&ptr, endp, "HelloWorld");
	DPRINT(ans);

	ptr = buf;

	int64_t i1;
	int32_t i2;
	float32_t f1;

	uint8_t mapptr[1024];
	uint16_t mapsize;

	ans = graw_read_i64(&ptr, endp, &i1);
	ans = graw_read_i32(&ptr, endp, &i2);
	ans = graw_read_f32(&ptr, endp, &f1);
	ans = graw_read_buf(&ptr, endp, mapptr, 1024, &mapsize);
	DPRINT(ans);	

	DPRINT(i1);
	DPRINT(i2);
	DPRINT(f1);
	DPRINTPTR(mapptr);
	DPRINT(mapsize);
	debug_write((char*)mapptr, mapsize);
	dprln();

	dprdump(buf, 48);
}