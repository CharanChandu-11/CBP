import heapq

# Input
n, m = map(int, input("Enter number of nodes and edges: ").split())
g = [[] for _ in range(n)]
print("Enter edges (u v w):")
for _ in range(m):
    u, v, w = map(int, input().split())
    g[u].append((v, w))

s = int(input("Enter source node: "))

# Dijkstra's algorithm
d = {s: 0}
q = [(0, s)]

while q:
    du, u = heapq.heappop(q)
    if du > d[u]:
        continue
    for v, w in g[u]:
        nd = du + w
        if v not in d or nd < d[v]:
            d[v] = nd
            heapq.heappush(q, (nd, v))

# Neat output
print("\nShortest distances from node", s)
print("-" * 30)
print(f"{'Node':<10}{'Distance':<10}")
print("-" * 30)
for i in range(n):
    dist = d[i] if i in d else "∞"
    print(f"{i:<10}{dist:<10}")
# Enter number of nodes and edges: 5 6
# Enter edges (u v w):
# 0 1 2
# 0 2 4
# 1 2 1
# 1 3 7
# 2 4 3
# 3 4 1
# Enter source node: 0