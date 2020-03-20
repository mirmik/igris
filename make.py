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
	target="libigris.so",
	toolchain=toolchain,
	mdepends = modules + ["igris.syslock", ("igris.ctrobj", "linux")],
	cxx_flags = '-fPIC -Wall',
	cc_flags = '-fPIC -Wall',
	shared = True
)

licant.cxx_library("static",
	target="libigris.a",
	toolchain=toolchain,
	mdepends = modules,
	cxx_flags = '-fPIC',
	cc_flags = '-fPIC',
	ccstd = "c11",
	shared = False
)

licant.install.install_library(
	tgt="install",
	uninstall="uninstall",
	libtgt="libigris.so",
	hroot="igris",
	headers="igris")

licant.ex("shared")