#ifndef GXX_OSUTIL_GETCWD_H
#define GXX_OSUTIL_GETCWD_H

#include <unistd.h>
#include <string>

namespace igris {
	namespace osutil {
                static inline std::string getcwd() {
                        char cwd[1024];
                        if (::getcwd(cwd, sizeof(cwd)) != NULL) {
                            return std::string(cwd, strlen(cwd));
                        }
                        else {
                              perror("getcwd() error");
                              return std::string();
                        }
	  	}
	}
}

#endif
