#!/usr/bin/env python3

import licant

licant.include("igris")

licant.cxx_application("target",
	sources = ["main.cpp"],
	mdepends = ["igris", "igris.dprint"],
)

licant.ex("target")