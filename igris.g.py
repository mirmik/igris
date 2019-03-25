import licant

licant.module("igris.include", include_paths = ["."])

licant.module("igris.sshell", 
	sources=["igris/shell/sshell.cpp"])

licant.module("igris.syslock", impl="atomic", 
	sources=["igris/syslock/syslock_genos_atomic.cpp"])

licant.module("igris.syslock", impl="mutex", 
	sources=["igris/syslock/syslock_mutex.cpp"], default=True)

licant.module("igris", mdepends=[
	"igris.include",
	"igris.sshell",
])