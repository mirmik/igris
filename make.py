#!/usr/bin/env python3
#coding: utf-8

import licant
import shutil
import os

version = "1.0.0"

licant.execute("igris.g.py")

licant.cli.add_argument("--toolchain")
opts, args = licant.cli.parse()

toolchain = licant.cxx_make.toolchain_gcc(opts.toolchain)

target = lambda suffix: "libigris.{}.{}".format(version, suffix)
install_include_path = '/usr/local/include/igris' 
install_directory_path = '/usr/lib/'
install_library_path = os.path.join(install_directory_path, target("so")) 
install_library_link = os.path.join(install_directory_path, 'libigris.so')

modules = [
		"igris",
		#("igris.ctrobj", "linux"),
		"igris.dprint",
		#"igris.syslock",
		"igris.protocols.gstuff",
		"igris.protocols.numcmd",
		#"igris.protocols.msgtype"
	]

licant.cxx_library("shared",
	target=target("so"),
	toolchain=toolchain,
	mdepends = modules,
	cxx_flags = '-fPIC',
	cc_flags = '-fPIC',
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

@licant.routine(deps=[target("so")])
def install():
	os.system("cp {0} {1}".format(target, install_directory_path))
	os.system("rm {}".format(install_library_link))
	os.system("ln -s {0} {1}".format(install_library_path, install_library_link))

	shutil.rmtree(install_include_path, True)
	shutil.copytree("igris", install_include_path, 
		symlinks=False, ignore=shutil.ignore_patterns('*.cpp', '*.c'))
	
	print("successfully installed")

@licant.routine(deps=[target("so")])
def install_headers():
	shutil.rmtree(install_include_path, True)
	shutil.copytree("igris", install_include_path, 
		symlinks=False, ignore=shutil.ignore_patterns('*.cpp', '*.c'))
	
	print("successfully installed")

licant.ex("shared")