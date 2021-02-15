#!/usr/bin/env python3

import sys
import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.execute("../igris.g.py")

tests = [
	"argvc",
	"event",
	"serialize",
	"dprint",
	"sync",
	"bits",
	"array_view",
	"test_allocator",
	"chunked_vector",
	"util"
]

tests_c = [
]

application("runtests",
	sources = ["main.cpp", "reader.cpp"],
		#+ [t+".cpp" for t in tests] 
		#+ [t+".c" for t in tests_c]),

	#ld_flags = "-L/usr/local/lib/",
	cxx_flags = "-g",
	cc_flags = "-g",

	include_paths = ["."],
	mdepends = [ "igris", ("igris.dprint", "user") ],
)

#application("runtests_installed",
#	sources = (
#		["main.cpp"] 
#		+ [t+".cpp" for t in tests] 
#		+ [t+".c" for t in tests_c]),
#
#	ld_flags = "-L/usr/local/lib/",
#
#	include_paths = ["."],
#	mdepends = [],
#
#	libs = ["gtest", "pthread", "nos", "igris"]
#)

licant.ex("runtests")
