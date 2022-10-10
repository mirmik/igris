import licant

licant.module("igris.posix",
              include_paths=["include"],
              sources=["signal.c", "select.c"],
              )

licant.module("igris.compat.posix",
              mdepends=["igris.posix"],
              )
