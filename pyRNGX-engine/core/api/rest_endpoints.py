from fastapi import FastAPI, Body, Query
from pydantic import BaseModel
from typing import Dict, List
import time
import math
import os
import random
from concurrent.futures import ProcessPoolExecutor, as_completed

app = FastAPI(title="fastflow-executor API")
START = time.time()

class Job(BaseModel):
    id: str
    payload: str

@app.get("/health")
def health():
    return {"status": "ok", "uptime_seconds": int(time.time() - START)}

@app.post("/submit")
def submit(job: Job):
    # Placeholder queueing endpoint
    return {"accepted": True, "message": f"Job {job.id} queued"}


def _pi_hits(n: int, seed: int) -> int:
    """Worker: count points inside unit quarter-circle."""
    rnd = random.Random(seed)
    hits = 0
    for _ in range(n):
        x = rnd.random()
        y = rnd.random()
        if x*x + y*y <= 1.0:
            hits += 1
    return hits

@app.get("/pi")
def pi(n: int = Query(1_000_00, ge=1, description="Number of random samples")):
    """
    Estimate π using single-process Monte Carlo.
    """
    t0 = time.time()
    hits = _pi_hits(n, seed=42)
    pi_est = 4.0 * hits / n
    return {
        "method": "monte_carlo_single",
        "samples": n,
        "pi_estimate": pi_est,
        "abs_error": abs(math.pi - pi_est),
        "elapsed_sec": round(time.time() - t0, 6),
    }

@app.get("/pi/parallel")
def pi_parallel(
    n: int = Query(2_000_000, ge=1, description="Total samples across all workers"),
    workers: int = Query(max(1, os.cpu_count() or 2), ge=1, le=128,
                         description="Number of processes")
):
    """
    Estimate π using parallel Monte Carlo across `workers` processes.
    """
    t0 = time.time()
    per = n // workers
    remainder = n % workers

    tasks = [per + (1 if i < remainder else 0) for i in range(workers)]
    seeds = [1234 + i for i in range(workers)]

    hits_total = 0
    start_workers = time.time()
    with ProcessPoolExecutor(max_workers=workers) as ex:
        futs = [ex.submit(_pi_hits, tasks[i], seeds[i]) for i in range(workers)]
        for fut in as_completed(futs):
            hits_total += fut.result()
    end_workers = time.time()

    pi_est = 4.0 * hits_total / n
    return {
        "method": "monte_carlo_parallel",
        "samples": n,
        "workers": workers,
        "pi_estimate": pi_est,
        "abs_error": abs(math.pi - pi_est),
        "elapsed_sec_total": round(time.time() - t0, 6),
        "elapsed_sec_compute": round(end_workers - start_workers, 6),
        "per_worker_samples": tasks,
    }

@app.post("/wordcount")
def wordcount(text: str = Body(..., embed=True)):
    """
    Count words in a text blob.
    """
    counts: Dict[str, int] = {}
    for tok in text.strip().split():
        w = tok.lower()
        counts[w] = counts.get(w, 0) + 1
    # Return top 10 by frequency
    top = sorted(counts.items(), key=lambda kv: (-kv[1], kv[0]))[:10]
    return {"unique": len(counts), "top": top}
