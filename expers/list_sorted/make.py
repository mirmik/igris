#!/usr/bin/env python3

import licant

licant.include("igris")
licant.include("nos")

licant.cxx_application("target",
	sources = ["main.cpp"],
	mdepends=["igris", "nos"]
)

licant.ex("target")
