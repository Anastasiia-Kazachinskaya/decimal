FROM ubuntu:24.04

LABEL authors="artemakulov"

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    check \
    gcovr \
    lcov \
    clang-format \
    && rm -rf /var/lib/apt/lists/*


WORKDIR /work

CMD ["bash"]
