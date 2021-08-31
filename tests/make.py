#!/usr/bin/env python3

import sys
import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.execute("../igris.g.py")
licant.include("nos")

application("runtests",
	sources = [
		"*.cpp",
		"container/*.cpp"
	],

	cxx_flags = "-g",
	cc_flags = "-g",

	include_paths = ["."],
	mdepends = [ 
		"igris", 
		("igris.dprint", "user"),
		"nos"
	],
)

licant.ex("runtests")
