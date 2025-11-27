import heapq

n,m = map(int, input("Enter number of nodes and edges: ").split())
g = [[] for _ in range(n)]
print("Enter edges (u v w):")
for _ in range(m):
    u, v, w = map(int, input().split())
    g[u].append((v, w))
s = int(input("Enter source node: "))
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


def get_path(v):
    path = []
    while v != -1:
        path.append(v)
        v = parent[v]
    return path[::-1]

print("Node   Distance   Path")
for i in range(n):
    if dist[i] == float('inf'):
        print(i, "   ", "∞", "        ", "-")
    else:
        path = get_path(i)
        print(i, "   ", dist[i], "        ", " -> ".join(map(str, path)))

