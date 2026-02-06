# Base image
FROM ubuntu:22.04

# Avoid interactive prompts during build
ENV DEBIAN_FRONTEND=noninteractive

# Install system dependencies
RUN apt-get update && apt-get install -y \
    g++ \
    make \
    sqlite3 \
    libsqlite3-dev \
    libasio-dev \
    git \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Set working directory inside container
WORKDIR /app

# Copy entire project into container
COPY . .

# Build the application
RUN g++ \
    src/main.cpp \
    src/db/database.cpp \
    src/routes/student_routes.cpp \
    -I./external/Crow/include \
    -std=c++17 \
    -lsqlite3 \
    -lpthread \
    -o server

# Expose API port
EXPOSE 18081

# Run the server
CMD ["./server"]
