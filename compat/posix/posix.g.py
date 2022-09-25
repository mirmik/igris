import licant

licant.module("igris.posix",
              include_paths=["include"],
              sources=["signal.c"],
              )

licant.module("igris.compat.posix",
              mdepends=["igris.posix"],
              )
