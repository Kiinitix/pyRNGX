# pyRNGX-engine

`pyRNGX-engine` is a scaffold for building high-performance simulation services.  

It exposes a simple HTTP API for experiments like Monte Carlo π estimation or word counting, and it is structured to grow into a more capable HPC-style execution framework.

The design goal is to make distributed or parallel simulation accessible without heavy orchestration. It uses Python and FastAPI for the interface, with placeholders for C++ and Rust backends to add speed and concurrency.

Think of this as a web service that runs heavy computations for you.

You start it (either locally or in Docker).

It gives you simple web endpoints (URLs) you can hit, like:
- /pi → it estimates the value of π (pi) by running a simulation.
- /pi/parallel → same thing, but using multiple CPU cores at once, so it’s faster.
- /wordcount → you send it text, it counts the words and gives you the result.

So pyRNGX-engine is like the “user-facing app”, an API service where people can request work and get answers back.

---

## Quickstart

### Run with Python

```bash
python -m venv .venv
source .venv/bin/activate           # Windows: .venv\Scripts\activate
pip install -r requirements.txt
uvicorn core.api.rest_endpoints:app --host 0.0.0.0 --port 8000
```

Open in browser or curl:

- Health: http://localhost:8000/health  
- Single-core π: http://localhost:8000/pi?n=200000  
- Parallel π: http://localhost:8000/pi/parallel?n=2000000&workers=8

### Run with Docker

```bash
docker build -t pyRNGX-engine .
docker run -p 8000:8000 pyRNGX-engine
```

---

## API Reference

Base URL: `http://localhost:8000`

### `GET /health`
Returns liveness information.
```json
{ "status": "ok", "uptime_seconds": 12 }
```

### `GET /pi`
Monte Carlo estimate of π in a single process.

Query:
- `n` – number of random samples (default 200000)

### `GET /pi/parallel`
Monte Carlo estimate of π in parallel.

Query:
- `n` – total samples (default 2,000,000)
- `workers` – number of processes (default = CPU cores)

### `POST /wordcount`
Counts words in text.

Body:
```json
{ "text": "red red blue" }
```

Response:
```json
{ "unique": 2, "top": [["red",2],["blue",1]] }
```

---

## Architecture

<img width="1037" height="807" alt="image" src="https://github.com/user-attachments/assets/fdc845ee-9df7-411f-80b1-92016cead789" />

The project is split into clear layers:

- **core/api** – FastAPI application (entry point).
- **core/executor, core/scheduler** – worker pool and scheduler (currently stubs in C++ and Rust).
- **connectors** – integrations with brokers and storage (Kafka, RabbitMQ, S3 stubs).
- **runtime** – monitoring and fault tolerance.
- **examples** – small demo workloads (map-reduce, streaming).
- **tests** – unit, integration, performance.

### Request Lifecycle

<img width="1432" height="724" alt="image" src="https://github.com/user-attachments/assets/6dae111d-22d5-4f21-8cb7-94e145f7910b" />

Flow:
1. A client calls an endpoint (for example, `/pi/parallel`).
2. The API validates and prepares work.
3. Execution happens in-process (now), or later through schedulers/workers.
4. Results are returned as JSON.

---

## Configuration

- `configs/default.yaml` – worker and monitoring defaults.
- `configs/logging.yaml` – console logging.

Environment variables:
- `PORT` – port number for the server if you override defaults.

---

## Deployment

### docker-compose
```yaml
services:
  api:
    build: .
    ports: ["8000:8000"]
    command: >
      uvicorn core.api.rest_endpoints:app
      --host 0.0.0.0 --port 8000
```

Run:
```bash
docker compose up --build
```

### Kubernetes
A basic manifest is in `scripts/deploy_k8s.yaml`. Replace the image name and adjust resource limits.

---

## Development

### Local setup
```bash
python -m venv .venv
source .venv/bin/activate
pip install -r requirements.txt
pip install -e .
```

Run the server with reload:
```bash
uvicorn core.api.rest_endpoints:app --host 0.0.0.0 --port 8000 --reload
```

### Code layout
- API: `core/api/rest_endpoints.py`
- Scheduler (Rust): `core/scheduler/task_scheduler.rs`
- Worker pool (C++): `core/executor/worker_pool.cpp`

---

## Testing

Run tests with:
```bash
pytest -q
```

Tests include:
- Unit tests for Python API logic
- Integration test hitting `/health`
- Performance smoke test

---

## Observability

- Prometheus: stubbed exporter in `runtime/monitoring/prometheus_exporter.rs`.  
- Logging: `configs/logging.yaml` for console logging.

---

## Roadmap

Short term:
- Use `pyRNGX-engine` for deterministic RNG streams in `/pi` endpoints.
- Add a background job queue.
- Add `/metrics` endpoint.

Medium term:
- Persistent store for checkpoints and replay.
- Stateful scheduler with priorities and deadlines.
- MPI or Ray integration for distributed runs.

Long term:
- SIMD and GPU acceleration backends.
- DSL for simulation graphs.
- Authentication and multi-tenant support.

---

## Contributing

Fork the repository and create a feature branch. Keep changes focused. Add or update tests where relevant. Use clear commit messages. Submit pull requests with a short description and simple steps to verify.

Coding style:
- Short, readable functions.
- Explicit naming.
- Docstrings for public functions and endpoints.

---
## Steps to re-generate Architecutre Diagram and Generation Call Sequence

1. Go to https://mermaid.live
2. To generate Architecture Diagram, use this code
```
flowchart TD
    %% fastflow-executor layered view

    subgraph L1[User Layer]
        A1[HTTP Client]
        A2[CLI or Script]
    end

    subgraph L2[API Layer]
        B1[FastAPI App]
        B2[Request Validation]
        B3[Response JSON]
        B1 --> B2 --> B3
    end

    subgraph L3[Execution Layer]
        C1[In-Process Executors]
        C2[Parallel Workers]
        C3[Task Queue Abstraction]
        C1 --- C2 --- C3
    end

    subgraph L4[Runtime Layer]
        D1[Monitoring]
        D2[Logging]
        D3[Fault Tolerance]
        D1 --- D2 --- D3
    end

    subgraph L5[Connectors]
        E1[Kafka Stub]
        E2[RabbitMQ Stub]
        E3[S3 Storage Stub]
        E1 --- E2 --- E3
    end

    subgraph L6[Examples and Tests]
        F1[Pi Single]
        F2[Pi Parallel]
        F3[Wordcount]
        F4[Unit and Integration Tests]
        F1 --- F2 --- F3 --- F4
    end

    %% data flow
    A1 --> B1
    A2 --> B1
    B3 --> C1
    C1 --> C2
    C2 --> C3
    C1 --> D1
    C1 --> D2
    C2 --> D3
    C1 --> E3
    C2 --> E1
    C2 --> E2
    C1 --> F1
    C2 --> F2
    C1 --> F3
```

3. To generate Request Lifecycle, use this code
```
sequenceDiagram
    participant Client
    participant API as FastAPI
    participant Exec as Executors
    participant RT as Runtime
    participant Conn as Connectors

    Client->>API: GET /pi/parallel?n=4000000&workers=8
    API->>API: Validate query
    API->>Exec: Create tasks and dispatch
    Exec->>Exec: Run parallel workers
    Exec->>RT: Emit metrics and logs
    Exec->>Conn: Optional publish or store
    Exec-->>API: Aggregated result JSON
    API-->>Client: 200 OK with pi_estimate

```

