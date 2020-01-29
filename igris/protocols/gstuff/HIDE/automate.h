#ifndef GXX_PACKAGER_H
#define GXX_PACKAGER_H

#include <gxx/io/iostorage.h>
#include <gxx/sline.h>
#include <gxx/util/setget.h>
#include <gxx/util/crc.h>
#include <gxx/util/gstuff.h>

namespace gxx {
	namespace gstuff {
		class automate {
			gxx::sline line;
			uint8_t crc;
			uint8_t state = 0;
			gxx::delegate<void, gxx::buffer> dlg;
			bool _debug = false;
	
		private: 
	
			inline void addchar(char c) {
				line.putchar(c);
				strmcrc8(&crc, c);
			}
	
		public:
			ACCESSOR(debug_mode, _debug);
			automate(gxx::buffer buf) : line(buf) {}
			automate(){}
	
			void init() {
				crc = 0xFF;
				line.init();
			}

			void init(gxx::buffer buf) {
				crc = 0xFF;
				line.init(buf);
			}
	
			void invoke_callback() {
				//if(_debug) gxx::println("callback_invoked");

				//Убираем символ контрольной суммы из строки.
				line.back(1);

				//Вызов обработчика. 
				dlg(line);
			}
	
			void set_callback(gxx::delegate<void, gxx::buffer> dlg) {
				this->dlg = dlg;
			}
	
			void setstate(int n) {
				state = n;
				//if (_debug) dprln("packager::setstate", n);
			}
	
			void newchar(char c) {
				//dprhexln(c);

				//if (_debug) {dpr("packager::newchar "); dpr(c); dpr("\t"); dprhexln(c);}
				switch (state) {
					case 0:
						if (c == gxx::gmsg::strt) {
							init();
							setstate(1);
						}
						break;
					case 1:
						switch ((unsigned char)c) {
							case gxx::gmsg::strt:
								if (line.size() == 0) break;
								if (crc != 0) { 
									//dprln("packager::crc_error:", crc);
								}
								else {
									//gxx::println("callback");
									invoke_callback();
								}
								init();
								break;
							case gxx::gmsg::stub:
								setstate(2);
								break;
							default:
								addchar(c);
						}
						break;
	
					case 2:
						switch ((unsigned char)c) {
							case gxx::gmsg::stub_strt:
								addchar(gxx::gmsg::strt);
								setstate(1);
								break;
							case gxx::gmsg::stub_stub:
								addchar(gxx::gmsg::stub);
								setstate(1);
								break;
							default:
								//dpr("packager::stub_error: ");
								//exit(-1);
								//dprhexln(c);
								setstate(0);
						}				
						break;
	
				}
			}
		};
	}
}

#endif