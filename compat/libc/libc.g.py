import licant

licant.module("igris.libc", 
	include_paths = ["include", "include/libc/"],
	sources = ["stdlib/*.c", "string/*.c", "ctype/*.c", "stdio/*.c", "math/*.c"]
)


