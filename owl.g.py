import licant

licant.module("owl.include", include_paths = ["."])
licant.module("owl.sshell", sources=["owl/shell/sshell.cpp"])

licant.module("owl", mdepends=[
	"owl.include",
	"owl.sshell"
])