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

application("runtests",
	sources = ["*.cpp"],

	cxx_flags = "-g",
	cc_flags = "-g",

	include_paths = ["."],
	mdepends = [ "igris", ("igris.dprint", "user") ],
)

licant.ex("runtests")
