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

licant.module("igris.protocols.dstring", sources= ["igris/util/dstring.c"])

licant.module("igris.util", sources=[
	"igris/util/numconvert.c",
	"igris/util/hexascii.cpp",
])

#################################################################

licant.module("igris.sshell", 
	sources=["igris/shell/sshell.cpp"])

licant.module("igris.syslock", impl="genos.atomic", 
	sources=["igris/sync/syslock_genos_atomic.cpp"])

licant.module("igris.syslock", impl="mutex", 
	sources=["igris/sync/syslock_mutex.cpp"], default=True)

licant.module("igris", mdepends=[
	"igris.include",
	"igris.sshell",
])