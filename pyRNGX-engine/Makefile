.PHONY: all build test run fmt

all: build

build:
\tpython -m pip install -r requirements.txt
\tcargo build --quiet || true
\tcmake -S . -B build && cmake --build build --config Release

test:
\tpytest -q

run:
\tuvicorn core.api.rest_endpoints:app --host 0.0.0.0 --port 8000

fmt:
\trustfmt || true
\tclang-format -i $(shell git ls-files '*.cpp' '*.hpp' 2>/dev/null) || true
