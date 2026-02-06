FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies (IMPORTANT: libasio-dev)
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    sqlite3 \
    libsqlite3-dev \
    libasio-dev \
    git \
    curl

WORKDIR /app

COPY . .

# Build the server
RUN g++ src/main.cpp src/db/database.cpp \
    -I./external/Crow/include \
    -std=c++17 \
    -lsqlite3 \
    -o server \
    -lpthread

EXPOSE 18081

CMD ["./server"]
