docker build . -t arm32v7/igris -f tools/dockerfile.arch --build-arg ARCH=arm32v7
docker build . -t arm64v8/igris -f tools/dockerfile.arch --build-arg ARCH=arm64v8
docker build . -t igris -f tools/dockerfile.host