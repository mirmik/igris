#!/usr/bin/env python3

import licant

licant.include("nos")
licant.include("igris")

licant.cxx_application("target",
	sources = ["main.cpp"],
	mdepends = ["igris", "igris.dprint", "igris.protocols.msgtype", "nos"],
	cxxstd = "gnu++17"
)

licant.ex("target")