#!/usr/bin/env python3

import sys
import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.execute("../igris.g.py")

application("runtests",
	sources = [
		"*.cpp",
		"container/*.cpp",
		"shell/*.cpp",
		"series/*.cpp"
	],

	cxx_flags = "-g",
	cc_flags = "-g",

	include_paths = ["."],
	mdepends = [ 
		"igris", 
		"igris.series", 
		("igris.dprint", "user")
	],
)

licant.ex("runtests")
