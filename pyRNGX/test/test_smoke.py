import numpy as np
from pyrngx import RNGStream

def test_uniform_det():
    a = RNGStream(seed=42, stream_id=0).uniform(4)
    b = RNGStream(seed=42, stream_id=0).uniform(4)
    assert np.allclose(a, b)

def test_jump_and_state():
    r = RNGStream(123, 7)
    a = r.uniform(5)
    st = r.state()
    r2 = RNGStream.from_state(st)
    b = r2.uniform(5)
    assert np.allclose(a, b)
