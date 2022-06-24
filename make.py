#!/usr/bin/env python3
#coding: utf-8

import licant
import licant.install
import os

licant.execute("igris.g.py")

licant.cli.add_argument("--toolchain")
opts, args = licant.cli.parse()

if hasattr(licant, 'gcc_toolchain'): 
	toolchain = licant.cxx_make.gcc_toolchain(opts.toolchain)
else:
	toolchain = licant.cxx_make.toolchain_gcc(opts.toolchain)


target = lambda suffix: "libigris.{}".format(suffix)

modules = [
		"igris",
		"igris.dprint",
		"igris.protocols.gstuff",
		"igris.series",
		"igris.sclonner"
	]

CCFLAGS = '-fPIC -Werror=all -Werror=extra -pedantic-errors -Wreturn-type -g'
CXXFLAGS = CCFLAGS
LDFLAGS = '-g'

licant.cxx_library("shared",
	target="libigris.so",
	toolchain=toolchain,
	mdepends = modules + [
		"igris.syslock", 
		("igris.ctrobj", "linux"), 
		("igris.platform", "host")],
	cxx_flags = CXXFLAGS,
	cc_flags = CCFLAGS,
	ld_flags = LDFLAGS,
	shared = True,
	cxxstd = "c++17",
	ccstd = "c11",
	optimize = "-O3"
)

licant.cxx_library("static",
	target="libigris.a",
	toolchain=toolchain,
	mdepends = modules,
	cxx_flags = CXXFLAGS,
	cc_flags = CCFLAGS,
	cxxstd = "c++17",
	ccstd = "c11",
	optimize = "-O3",
	shared = False
)

licant.install.install_library(
	tgt="install",
	uninstall="uninstall",
	libtgt="libigris.so",
	hroot="igris",
	headers="igris")

licant.ex("shared")
