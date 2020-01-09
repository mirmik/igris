#!/usr/bin/env python3
#coding: utf-8

import licant
import licant.install
import os

licant.execute("igris.g.py")

licant.cli.add_argument("--toolchain")
opts, args = licant.cli.parse()

toolchain = licant.cxx_make.toolchain_gcc(opts.toolchain)

target = lambda suffix: "libigris.{}".format(suffix)

modules = [
		"igris",
		"igris.dprint",
		"igris.protocols.gstuff",
		"igris.protocols.numcmd",
	]

licant.cxx_library("shared",
	target=target("so"),
	toolchain=toolchain,
	mdepends = modules + ["igris.syslock", ("igris.ctrobj", "linux")],
	cxx_flags = '-fPIC -Wall',
	cc_flags = '-fPIC -Wall',
	shared = True
)

licant.cxx_library("static",
	target=target("a"),
	toolchain=toolchain,
	mdepends = modules,
	cxx_flags = '-fPIC',
	cc_flags = '-fPIC',
	ccstd = "c11",
	shared = False
)

licant.install.install_library(tgt="install", libtgt=target("so"), headers="igris", hroot="igris")

@licant.routine
def uninstall():
	os.system("rm -r {}".format(install_include_path))
	os.system("rm {}".format(install_library_path))
	os.system("rm {}".format(install_library_link))

licant.ex("shared")