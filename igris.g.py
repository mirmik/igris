import licant

licant.module("igris.include", include_paths = ["."])
licant.module("igris.sshell", sources=["igris/shell/sshell.cpp"])

licant.module("igris", mdepends=[
	"igris.include",
	"igris.sshell"
])