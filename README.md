# pyRNGX

pyRNGX is a high performance simulation framework designed to model and execute complex workloads on distributed systems. It provides a lightweight execution engine with stream-based task scheduling, adaptive resource management, and support for multi-language integration. The framework is optimized for scenarios where speed, scalability, and fine-grained control over execution patterns are essential.

## Overview

At its core, pyRNGX decomposes workloads into independent streams of execution. These streams are managed by a scheduling layer that dynamically assigns them to compute resources. The system supports heterogeneous execution environments, allowing developers to implement simulation logic in Python for flexibility, C++ for raw performance, and Rust for safety and concurrency guarantees.  

The architecture is modular. A stream manager directs tasks into substreams, an execution layer manages memory and threading, and an adaptive orchestrator monitors performance metrics to reallocate resources in real time. Data movement is handled through a high-speed buffer system that minimizes latency and avoids unnecessary copying between processes. This approach creates a system capable of scaling across multi-core CPUs, distributed clusters, or even hybrid HPC cloud setups.

## Core Technical Details

pyRNGX is designed around the idea of stateless execution. Each substream is executed independently, with global coordination handled by the orchestration layer only when absolutely necessary. This eliminates the overhead of tightly coupled orchestration and enables linear scaling in many workloads.

The runtime engine uses lock-free queues and SIMD enabled operators for maximum throughput on modern CPUs. Memory management is cache-aware, reducing stalls from non-local access. For distributed execution, the system leverages MPI and gRPC style communication to provide both low-level speed and high-level flexibility. Developers can extend the core with plugins written in Rust or C++, which are loaded at runtime and integrated into the task execution flow without requiring recompilation.

The framework also integrates a monitoring layer. Metrics such as task completion time, queue backlog, and CPU utilization are continuously measured, and this feedback drives adaptive scheduling. The goal is to keep resources saturated while avoiding bottlenecks in communication or memory transfer.

## Utility

pyRNGX is designed for problems that require massive computational throughput and flexible execution models. Scientific simulations, agent-based modeling, and Monte Carlo methods can all be accelerated using its parallel execution capabilities. Financial modeling and risk simulations benefit from its ability to handle millions of independent trials across distributed clusters. In high performance computing research, the framework provides a testbed for experimenting with new scheduling algorithms or hybrid execution strategies.

Because it supports multiple languages, it is also well suited for mixed workloads where part of the logic is written in Python for rapid prototyping and another part in C++ or Rust for production-grade performance. This makes pyRNGX a versatile choice for both research and real-world applications.

## Problems It Can Solve

pyRNGX can simulate and accelerate large scale scientific experiments such as climate models, physics-based simulations, or epidemiological spread analysis. It can be applied to real-time systems like trading infrastructure, where microsecond-level latency matters. It is also a natural fit for enterprise-scale data processing pipelines that require both elasticity and deterministic execution guarantees.

The framework aims to make HPC simulation accessible without compromising on performance, bridging the gap between academic research prototypes and industrial grade execution engines.

### Architecture Diagram
<img width="1297" height="740" alt="image" src="https://github.com/user-attachments/assets/d2bd96ee-0881-4936-a21a-3d4a6889a02e" />

### Generation Call Sequence
<img width="1887" height="813" alt="image" src="https://github.com/user-attachments/assets/f0469deb-9c44-43ae-b35d-00e887af5fe7" />


### Steps to re-generate Architecutre Diagram and Generation Call Sequence
1. Go to https://mermaid.live
2. To generate Architecture Diagram, use this code
```
graph TD

subgraph Layer1 [User Simulation Layer]
    A1[Scientific Simulation Code]
    A2[Finance Monte Carlo Models]
    A3[AI/ML Workloads]
end

subgraph Layer2 [RNG Abstraction Layer]
    B1[pyRNGX API]
    B2[Language Bindings: Python, C++, Rust]
    B3[NumPy-Style Interface]
end

subgraph Layer3 [Core RNG Engine]
    C1[Counter-Based RNG Kernel]
    C2[Stream Manager - Independent Substreams]
    C3[Reproducibility Manager]
end

subgraph Layer4 [Backends & Accelerators]
    D1[Philox / Threefry]
    D2[Intel MKL Random123]
    D3[NVIDIA cuRAND]
    D4[Custom SIMD CPU Backend]
end

subgraph Layer5 [HPC Runtime Integration]
    E1[Slurm Scheduler]
    E2[MPI / OpenMP]
    E3[CUDA / ROCm]
    E4[Checkpoint/Restart System]
end

%% connections
A1 --> B1
A2 --> B1
A3 --> B1

B1 --> B2
B1 --> B3

B2 --> C1
B3 --> C1

C1 --> D1
C1 --> D2
C1 --> D3
C1 --> D4

C2 --> E2
C3 --> E4

D1 --> E3
D2 --> E1
D3 --> E3
D4 --> E2

```

3. To generate Generation Call Sequence, use this code
   
```
sequenceDiagram
    actor User
    participant Py as Python RNGStream
    participant FFI as pybind11/C ABI
    participant RT as Runtime Dispatch
    participant CPU as CPU Backend
    participant GPU as GPU Backend
    participant ALG as Philox/Threefry
    participant DST as Distributions
    participant BUF as Output Buffer

    User->>Py: rng.normal(size=N, mean, std, backend="cpu")
    Py->>FFI: normal(N, mean, std, seed, stream_id, state)
    FFI->>RT: select backend (CPU/GPU), ISA
    alt CPU path
        RT->>CPU: allocate counter range, threads = p
        CPU->>ALG: generate raw integers (vectorized)
        CPU->>DST: transform to Normal via Ziggurat
        DST-->>CPU: float64 block
        CPU-->>FFI: pointer to contiguous block
    else GPU path
        RT->>GPU: launch kernel (grid-stride)
        GPU->>ALG: per-thread counters → integers
        GPU->>DST: Normal transform on device
        DST-->>GPU: device buffer
        GPU-->>FFI: device ptr / DLPack handle
    end
    FFI-->>Py: wrap as ndarray / device tensor
    Py-->>User: return array (bitwise reproducible)
```
