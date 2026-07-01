FROM ubuntu:24.04 AS build

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    git cmake g++ make \
    libjsoncpp-dev uuid-dev zlib1g-dev libssl-dev libpq-dev \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /opt
RUN git clone --depth 1 --branch v1.9.10 --recurse-submodules \
        https://github.com/drogonframework/drogon.git \
 && cmake -S drogon -B drogon/build -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_EXAMPLES=OFF -DBUILD_CTL=OFF \
 && cmake --build drogon/build --target install -j"$(nproc)"

WORKDIR /app
COPY CMakeLists.txt ./
COPY src ./src
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release \
 && cmake --build build -j"$(nproc)"


FROM ubuntu:24.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    libjsoncpp25 libuuid1 zlib1g libssl3 libpq5 ca-certificates \
 && rm -rf /var/lib/apt/lists/*

COPY --from=build /app/build/url_shortener /usr/local/bin/url_shortener

RUN useradd --system --no-create-home app
USER app

EXPOSE 8080
ENV PORT=8080
CMD ["url_shortener"]
