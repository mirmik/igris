mkdir build-android
cd build-android
cmake .. -B . \
	-DCMAKE_CXX_COMPILER=${ACOMPILERS}/aarch64-linux-android29-clang++ \
	-DCMAKE_C_COMPILER=${ACOMPILERS}/aarch64-linux-android29-clang \
	-DWITHOUT_TESTS=1
