import licant

licant.module("igris.include", include_paths = ["."])

######################STDLIBS#COMPAT############################

licant.execute("compat/libc/libc.g.py")
licant.execute("compat/std/std.g.py")
licant.execute("compat/posix/posix.g.py")

licant.module("igris.cxx_support", sources=["compat/cxx/__cxa_pure_virtual.c"])
licant.module("igris.stdlibs", 
	mdepends = ["igris.libc", "igris.std", "igris.posix"])

################################################################
######################DPRINT####################################

licant.module("igris.dprint.common", "impl", srcdir="igris/dprint",
	sources=["dprint_func_impl.c", "dprintxx.cpp"],
)

licant.module("igris.dprint.common", "stub", srcdir="igris/dprint",
	sources = "dprint_func_stub.c dprint_stub.c dprintxx.cpp".split(" ")
)

licant.module("igris.dprint", "manually", srcdir="igris/dprint",
	sources = ["dprint_manually.c"],
	mdepends = [("igris.dprint.common","impl")],
)

licant.module("igris.dprint", "stdout", srcdir="igris/dprint",
	sources = ["dprint_stdout.c"],
	mdepends = [("igris.dprint.common","impl")],
	default = True
)

licant.module("igris.dprint", "stub", srcdir="igris/dprint",
	mdepends = [("igris.dprint.common","stub")]
)

#################################################################
########################BUG######################################

licant.module("igris.bug", impl="abort", sources=["igris/util/bug_abort.c"], default=True)

#################################################################
########################PROTOCOLS################################

licant.module("igris.protocols.gstuff", 
	sources= [
		"igris/protocols/gstuff/autorecv.c",
		"igris/protocols/gstuff/gstuff.c"
	]
)

licant.module("igris.protocols.numcmd", 
	sources= [
		"igris/protocols/numcmd/numcmd.c"
	]
)

#################################################################
#########################UTILS###################################

licant.module("igris.util", sources=[
	"igris/util/numconvert.c",
	"igris/util/hexascii.cpp",
	"igris/util/string.cpp",
	"igris/util/dstring.c",
	"igris/util/stub.c",
	"igris/util/printf_impl.c",
])

#################################################################

licant.module("igris.sshell", 
	sources=["igris/shell/sshell.cpp"])

licant.module("igris.syslock", impl="genos.atomic", 
	sources=["igris/sync/syslock_genos_atomic.cpp"])

licant.module("igris.syslock", impl="mutex", 
	sources=["igris/sync/syslock_mutex.cpp"], default=True)

licant.module("igris.protocols.msgtype", 
	sources=["igris/protocols/msgtype.cpp"])

licant.module("igris", mdepends=[
	"igris.include",
	"igris.sshell",
	 "igris.bug"
])

licant.module("igris.ctrobj.common",
	sources = ["igris/ctrobj/wait.c"]
)

licant.module("igris.ctrobj", "linux",
	sources = ["igris/ctrobj/wait-linux.cpp"],
	mdepends= ["igris.ctrobj.common"]
)