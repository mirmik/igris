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
		"series/*.cpp",
		"archive/*.cpp",
	],

	cxxstd="c++20",
	ccstd="c11",
	cxx_flags = "-g -Werror=all -Wno-gnu-zero-variadic-macro-arguments",
	cc_flags = "-g -Werror=all -Wno-gnu-zero-variadic-macro-arguments",

	include_paths = ["."],
	mdepends = [ 
		"igris",
		"igris.printf_impl",
		"igris.series", 
		("igris.dprint", "user")
	],

	libs = ["rt", "pthread"]
)

licant.ex("runtests")
