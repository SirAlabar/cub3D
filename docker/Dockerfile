FROM ubuntu:latest

# Avoid prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Install required packages
RUN apt-get update && apt-get install -y \
    build-essential \
    gcc \
    make \
    git \
    xorg \
    libxext-dev \
    libbsd-dev \
    pkg-config \
    libx11-dev \
    valgrind \
    gdb \
    vim \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Handle user creation more gracefully
ARG USER_ID=1000
ARG GROUP_ID=1000
RUN groupadd -g ${GROUP_ID} appuser 2>/dev/null || groupmod -n appuser $(getent group ${GROUP_ID} | cut -d: -f1) \
    && useradd -u ${USER_ID} -g appuser -s /bin/bash appuser \
    && chown -R appuser:appuser /app

USER appuser

# Install MinilibX
RUN git clone https://github.com/42Paris/minilibx-linux.git /tmp/minilibx && \
    cd /tmp/minilibx && \
    make && \
    cp libmlx.a /usr/local/lib/ && \
    cp mlx.h /usr/local/include/ && \
    rm -rf /tmp/minilibx

# Add scripts for convenience
COPY --chown=appuser:appuser scripts/entrypoint.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/entrypoint.sh

ENTRYPOINT ["/usr/local/bin/entrypoint.sh"]
