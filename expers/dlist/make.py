#!/usr/bin/env python3

import licant

licant.include("owl")
licant.include("nos")

licant.cxx_application("target",
	sources=["main.cpp"],
	mdepends=["owl", "nos"],
)

licant.ex("target")