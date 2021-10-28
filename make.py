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
		"igris.series",
		"igris.sclonner"
	]

CCFLAGS = '-fPIC -Wall -pedantic-errors -Wreturn-type -Wno-gnu-zero-variadic-macro-arguments'
CXXFLAGS = CCFLAGS

licant.cxx_library("shared",
	target="libigris.so",
	toolchain=toolchain,
	mdepends = modules + ["igris.syslock", ("igris.ctrobj", "linux")],
	cxx_flags = CXXFLAGS,
	cc_flags = CCFLAGS,
	shared = True,
	cxxstd = "c++17 -g",
	ccstd = "c11 -g",
	optimize = "-O3"
)

licant.cxx_library("static",
	target="libigris.a",
	toolchain=toolchain,
	mdepends = modules,
	cxx_flags = CXXFLAGS,
	cc_flags = CCFLAGS,
	cxxstd = "gnu++17",
	ccstd = "gnu11",
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
