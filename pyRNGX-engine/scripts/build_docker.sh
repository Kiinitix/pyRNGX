#!/usr/bin/env bash
set -euo pipefail
IMAGE=${1:-fastflow-executor:local}
docker build -t "$IMAGE" -f - . <<'DOCKER'
FROM python:3.11-slim
WORKDIR /app
COPY . /app
RUN pip install -r requirements.txt
EXPOSE 8000
CMD ["uvicorn", "core.api.rest_endpoints:app", "--host", "0.0.0.0", "--port", "8000"]
DOCKER
echo "Built $IMAGE"
