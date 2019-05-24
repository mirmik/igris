#include <igris/dprint.h>
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
	DPRINT(ans);

	ptr = buf;

	int64_t i1;
	int32_t i2;
	float32_t f1;

	ans = graw_read_i64(&ptr, endp, &i1);
	ans = graw_read_i32(&ptr, endp, &i2);
	ans = graw_read_f32(&ptr, endp, &f1);
	DPRINT(ans);	

	DPRINT(i1);
	DPRINT(i2);
	DPRINT(f1);

	dprdump(buf, 48);
}