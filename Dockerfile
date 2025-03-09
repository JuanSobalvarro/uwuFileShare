# Use a lightweight Python + Debian base image
FROM python:3.13-slim

# Install system dependencies
RUN apt update && apt install -y \
    cmake \
    g++ \
    git \
    libboost-all-dev \
    build-essential \
    && rm -rf /var/lib/apt/lists/*

# Set up a working directory
WORKDIR /app

# Copy dependency list first (to leverage Docker caching)
COPY requirements.txt .  

# Install Python dependencies
RUN pip install --no-cache-dir --upgrade pip && pip install --no-cache-dir -r requirements.txt

# Copy the source code
COPY . .

# Set default command
CMD ["/bin/bash"]
