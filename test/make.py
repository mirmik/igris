#!/usr/bin/env python3

import sys
import licant
from licant.cxx_modules import application
from licant.modules import submodule, module
from licant.libs import include

licant.execute("../igris.g.py")

tests = [
	"argvc",
	"bits",
	"event",
	"serialize",
	"sync"
]

tests_c = [
]

application("runtests",
	sources = (
		["main.cpp"] 
		+ [t+".cpp" for t in tests] 
		+ [t+".c" for t in tests_c]),

	ld_flags = "-L/usr/local/lib/",

	include_paths = ["."],
	mdepends = [ "igris" ],

	libs = ["gtest", "pthread", "nos"]
)

application("runtests_installed",
	sources = (
		["main.cpp"] 
		+ [t+".cpp" for t in tests] 
		+ [t+".c" for t in tests_c]),

	ld_flags = "-L/usr/local/lib/",

	include_paths = ["."],
	mdepends = [],

	libs = ["gtest", "pthread", "nos", "igris"]
)

licant.ex("runtests")