import licant

MODULES = [
	"igris.include",
	"igris.util",
	"igris.bug",
	"igris.dprint",
	"igris.path",
	"igris.time",
	"igris.trent",
	"igris.semaphore",

	"igris.syslock",
	"igris.ctrobj",

	"igris.protocols.gstuff",
	"igris.crypt.aes",

#	"igris.os_extension"
]

if sys.platform == "linux":
		MODULES.append("igris.os_extension")

licant.module("igris", 
sources = [
	"igris/datastruct/stimer.c"
],
mdepends=MODULES)

licant.module("igris.include", include_paths = ["."])

######################STDLIBS#COMPAT############################

licant.execute("compat/libc/libc.g.py")
licant.execute("compat/std/std.g.py")
licant.execute("compat/posix/posix.g.py")
licant.execute("compat/newlib-stub/newlib-stub.g.py")

licant.module("igris.cxx_support", sources=["compat/cxx/__cxa_pure_virtual.c", "compat/cxx/__dso_handle.c"])
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

licant.module("igris.dprint", "user", srcdir="igris/dprint",
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
		"igris/protocols/gstuff.c",
		"igris/protocols/gstuff_v1/gstuff.c",
		"igris/protocols/gstuff_v1/autorecv.c"
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
	"igris/util/crc.c",
	"igris/util/emergency_stop.c",
	"igris/util/printf_impl.c",
])

#################################################################

#licant.module("igris.sshell", 
#	sources=["igris/shell/sshell.cpp"])

licant.module("igris.syslock", impl="genos.atomic", 
	sources=["igris/sync/syslock_genos_atomic.cpp"])

licant.module("igris.syslock", impl="mutex", 
	sources=["igris/sync/syslock_mutex.cpp"], default=True)

licant.module("igris.semaphore", sources=["igris/sync/semaphore.c"])

licant.module("igris.protocols.msgtype", 
	sources=["igris/protocols/msgtype.cpp"])

licant.module("igris.ctrobj.common",
	sources = ["igris/osinter/wait.c"]
)

licant.module("igris.ctrobj", "linux",
	sources = ["igris/osinter/wait-linux.cpp"],
	mdepends= ["igris.ctrobj.common"],
	default=True
)

licant.module("igris.trent",
	sources = ["igris/trent/trent.cpp"]
)

licant.module("igris.path",
	sources = ["igris/path/path.cpp"]
)

#################################################################
licant.module("igris.os_extension", "unix",
	sources=["igris/osutil/src/posix.cpp"],
	default = True
)


licant.module("igris.time", "posix",
	sources = ["igris/time/time_posix.cpp"],
	default = True
)

licant.module("igris.crypt.aes", 
	sources = ["igris/crypt/aes.c"])
