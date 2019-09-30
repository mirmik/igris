#!/usr/bin/env python3

import licant

licant.include("igris")
licant.include("genos")

licant.glbfunc.genos_firmware(
	sources=["main.cpp"],
	mdepends=["igris.stdlibs"],
)
