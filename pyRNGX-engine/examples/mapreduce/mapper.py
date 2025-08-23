def map_line(line: str):
    for word in line.strip().split():
        yield (word.lower(), 1)

if __name__ == "__main__":
    import sys
    for ln in sys.stdin:
        for k, v in map_line(ln):
            print(f"{k}\t{v}")
