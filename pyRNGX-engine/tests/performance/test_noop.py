import time

def test_noop_perf():
    t0 = time.time()
    for _ in range(100_000):
        pass
    assert time.time() - t0 < 1.0
