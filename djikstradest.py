import heapq

n, m = map(int, input("Nodes and edges: ").split())
g = [[] for _ in range(n)]
print("Enter u v w")
for _ in range(m):
    u, v, w = map(int, input().split())
    g[u].append((v, w))
    g[v].append((u, w))

s = int(input("Source node: "))
dest = int(input("Destination node: "))

dist = [float('inf')] * n
parent = [-1] * n
dist[s] = 0
pq = [(0, s)]

while pq:
    d, u = heapq.heappop(pq)
    if d > dist[u]:
        continue
    for v, w in g[u]:
        if dist[u] + w < dist[v]:
            dist[v] = dist[u] + w
            parent[v] = u
            heapq.heappush(pq, (dist[v], v))

path = []
cur = dest
while cur != -1:
    path.append(cur)
    cur = parent[cur]
path.reverse()

if dist[dest] == float('inf'):
    print(f"No path from {s} to {dest}")
else:
    print(f"Shortest distance from {s} to {dest} = {dist[dest]}")
    print("Path:", " -> ".join(map(str, path)))
