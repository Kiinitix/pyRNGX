FROM debian:bookworm-slim AS builder

ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake git curl ca-certificates pkg-config \
    && rm -rf /var/lib/apt/lists/*

RUN curl https://sh.rustup.rs -sSf | sh -s -- -y
ENV PATH="/root/.cargo/bin:${PATH}"

WORKDIR /src
COPY . /src

RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build --config Release
RUN cargo build --release || true

FROM python:3.11-slim AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
    libstdc++6 curl ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Install Python deps first
COPY requirements.txt /app/requirements.txt
RUN pip install --no-cache-dir -r requirements.txt

# Copy only runtime code
COPY core /app/core
COPY configs /app/configs

ENV PYTHONUNBUFFERED=1 \
    PORT=8000

EXPOSE 8000

CMD ["uvicorn", "core.api.rest_endpoints:app", "--host", "0.0.0.0", "--port", "8000"]

# docker build -t fastflow-executor . && docker run -p 8000:8000 fastflow-executor
