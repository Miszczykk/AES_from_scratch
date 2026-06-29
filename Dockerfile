FROM ubuntu:latest
LABEL authors="miszczykk"

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    python3 \
    python3-pip \
    && rm -rf /var/lib/apt/lists/*

RUN pip3 install pycryptodome --break-system-packages
WORKDIR /app

COPY . .

RUN mkdir cmake-build-debug && cd cmake-build-debug && cmake .. && make


CMD ["./build/AES_from_scratch"]
ENTRYPOINT ["sh", "-c", "./cmake-build-debug/AES_from_scratch \"$1\" \"$2\" && echo '\n--- TESTS ---' && python3 tests/test.py", "--"]