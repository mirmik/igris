from licant import *

implementation("nos.dprint", "stub", 
	sources = "dprint_func_stub.c dprint_stub.c dprintxx.cpp".split(" ")
)

implementation("nos.dprint", "diag", 
	sources = "dprint_func_impl.c dprint_diag.c dprintxx.cpp".split(" "),
	cc_flags = "-Wno-pointer-to-int-cast",
	depends = "nos.diag",
)

implementation("nos.dprint", "manually", 
	sources = "dprint_func_impl.c dprint_manually.c dprintxx.cpp".split(" ")
)

implementation("nos.dprint", "cout",
	sources = "dprint_func_impl.c dprint_stdout.c dprintxx.cpp".split(" ")
)

module_defimpl("nos.dprint", "cout")