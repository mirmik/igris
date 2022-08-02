set -ex
docker build . -t arm32v7/igris -f tools/dockerfile.arch --build-arg ARCH=arm32v7
docker build . -t arm64v8/igris -f tools/dockerfile.arch --build-arg ARCH=arm64v8
docker build . -t igris -f tools/dockerfile.host

mkdir -p dist
mkdir -p dist/arm32v7
mkdir -p dist/arm64v8

docker run --rm --entrypoint cat igris /root/igris/libigris.so > dist/libigris.so
docker run --rm --entrypoint cat arm32v7/igris /root/igris/libigris.so > dist/arm32v7/libigris.so
docker run --rm --entrypoint cat arm64v8/igris /root/igris/libigris.so > dist/arm64v8/libigris.so

docker run --rm --entrypoint cat igris /root/igris/libigris.a > dist/libigris.a
docker run --rm --entrypoint cat arm32v7/igris /root/igris/libigris.a > dist/arm32v7/libigris.a
docker run --rm --entrypoint cat arm64v8/igris /root/igris/libigris.a > dist/arm64v8/libigris.a