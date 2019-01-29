import licant

licant.module("igris.include", include_paths = ["."])


licant.module("igris", mdepends=[
	"igris.include"
])