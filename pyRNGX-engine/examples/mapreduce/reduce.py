from collections import defaultdict
import sys

counts = defaultdict(int)
for ln in sys.stdin:
    key, val = ln.strip().split("\t")
    counts[key] += int(val)

for k, v in sorted(counts.items()):
    print(f"{k}\t{v}")
