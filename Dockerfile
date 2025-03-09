# Use an official image with Python & C++ tools
FROM ubuntu:24.04

# Install system dependencies
RUN apt update && apt install -y \
    cmake \
    g++ \
    python3 \
    python3-pip \
    python3-dev \
    git \
    libboost-all-dev \
    && rm -rf /var/lib/apt/lists/*

# Set up a working directory
WORKDIR /app

# Copy the source code
COPY . /app

RUN pip install --upgrade pip && pip install -r requirements.txt

CMD ["/bin/bash"]