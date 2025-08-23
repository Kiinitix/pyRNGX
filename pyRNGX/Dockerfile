FROM debian:bookworm-slim AS builder
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake git curl ca-certificates \
    python3-full python3-venv python3-pip python3-dev \
 && rm -rf /var/lib/apt/lists/*

RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:${PATH}"

WORKDIR /src
COPY . /src

# Optional debug: uncomment to verify files exist
# RUN ls -R .

RUN python -m pip install --upgrade pip \
 && pip install scikit-build-core pybind11 numpy build
RUN python -m build --wheel

FROM debian:bookworm-slim
RUN apt-get update && apt-get install -y --no-install-recommends \
    python3-full python3-venv \
 && rm -rf /var/lib/apt/lists/*

RUN python3 -m venv /opt/venv
ENV PATH="/opt/venv/bin:${PATH}"
WORKDIR /app
COPY --from=builder /src/dist/*.whl /app/
RUN pip install --no-cache-dir ./*.whl
CMD ["python", "-c", "from pyrngx import RNGStream; import numpy as np; print(RNGStream(42,0).uniform(4))"]
