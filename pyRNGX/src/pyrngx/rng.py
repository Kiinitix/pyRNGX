import numpy as np

try:
    # Preferred: extension is inside the package
    from . import _pyrngx
except Exception:
    # Fallback: extension is a top-level module
    import _pyrngx


class RNGStream:
    def __init__(self, seed: int, stream_id: int = 0):
        self._s = _pyrngx.Stream(int(seed) & ((1<<64)-1), int(stream_id) & ((1<<64)-1))

    def uniform(self, size: int) -> np.ndarray:
        return np.asarray(self._s.uniform(int(size)))

    def normal(self, size: int) -> np.ndarray:
        return np.asarray(self._s.normal(int(size)))

    def jump_ahead(self, n: int) -> None:
        self._s.jump_ahead(int(n))

    def state(self):
        s0,s1,chi,clo = self._s.state()
        return {"key0": int(s0), "key1": int(s1), "counter_hi": int(chi), "counter_lo": int(clo)}

    @classmethod
    def from_state(cls, state: dict) -> "RNGStream":
        obj = object.__new__(cls)
        obj._s = _pyrngx.Stream.from_state(
            int(state["key0"]), int(state["key1"]),
            int(state["counter_hi"]), int(state["counter_lo"])
        )
        return obj
