#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include "rng_core.hpp"

namespace py = pybind11;

extern "C" {
  StreamState* prx_create(uint64_t seed, uint64_t stream_id);
  void prx_free(StreamState*);
  void prx_jump_ahead(StreamState*, uint64_t n);
  void prx_state(const StreamState*, uint64_t out_state[4]);
  StreamState* prx_from_state(uint64_t s0, uint64_t s1, uint64_t c_hi, uint64_t c_lo);
  void prx_uniform_double(StreamState*, double* out, size_t n);
  void prx_normal_double(StreamState*, double* out, size_t n);
}

struct Stream {
  StreamState* st;
  Stream(uint64_t seed, uint64_t stream_id) { st = prx_create(seed, stream_id); }
  ~Stream() { if (st) prx_free(st); }
  void jump_ahead(uint64_t n) { prx_jump_ahead(st, n); }
  std::array<uint64_t,4> state() const {
    uint64_t s[4]; prx_state(st, s); return {s[0],s[1],s[2],s[3]};
  }
  static Stream* from_state(uint64_t s0, uint64_t s1, uint64_t c_hi, uint64_t c_lo) {
    Stream* obj = (Stream*)::operator new(sizeof(Stream));
    obj->st = prx_from_state(s0,s1,c_hi,c_lo);
    return obj;
  }
  py::array_t<double> uniform(size_t n) {
    py::array_t<double> out(n);
    auto buf = out.mutable_unchecked<1>();
    py::gil_scoped_release rel;
    prx_uniform_double(st, buf.mutable_data(0), n);
    return out;
  }
  py::array_t<double> normal(size_t n) {
    py::array_t<double> out(n);
    auto buf = out.mutable_unchecked<1>();
    py::gil_scoped_release rel;
    prx_normal_double(st, buf.mutable_data(0), n);
    return out;
  }
};

PYBIND11_MODULE(_pyrngx, m) {
  py::class_<Stream>(m, "Stream")
    .def(py::init<uint64_t,uint64_t>(), py::arg("seed"), py::arg("stream_id"))
    .def_static("from_state", &Stream::from_state)
    .def("jump_ahead", &Stream::jump_ahead)
    .def("state", &Stream::state)
    .def("uniform", &Stream::uniform, py::arg("size"))
    .def("normal",  &Stream::normal,  py::arg("size"));
}
