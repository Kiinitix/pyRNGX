# pyRNGX
A high-performance simulation framework for Python that bridges HPC and everyday utilities. It provides modular components for distributed execution, parallel simulations, and dataflow orchestration, powered by Python with C++/Rust backends for speed.

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
