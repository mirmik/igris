#include <igris/dprint.h>
#include <igris/buffer.h>
#include <igris/protocols/msgtype.h>

#include <nos/print.h>
#include <nos/fprint.h>

int main(int argc, char*argv[]) 
{
	//std::vector<igris::msgtype_object> m = igris::type_objectize("f32:a,f32,str,i32[]:zarr");
	//nos::println(m);

	igris::msgtype_struct strct;
	int ans = strct.readtype(argc == 1 ? "f32:a,f32,str,i32[]:zarr": argv[1]);

	nos::println(strct);
}