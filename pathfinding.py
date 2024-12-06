import heapq

class Nodo:
    def __init__(self, x, y, costo=0, heuristica=0, padre=None):
        self.x = x
        self.y = y
        self.costo = costo
        self.heuristica = heuristica
        self.padre = padre

    def __lt__(self, otro):
        return (self.costo + self.heuristica) < (otro.costo + otro.heuristica)

def heuristica_manhattan(x1, y1, x2, y2):
    return abs(x1 - x2) + abs(y1 - y2)

def a_star(inicio, objetivo, mapa):
    filas = len(mapa)
    columnas = len(mapa[0])
    abiertos = []
    cerrados = set()
    inicio_nodo = Nodo(inicio[0], inicio[1], costo=0, heuristica=heuristica_manhattan(*inicio, *objetivo))
    heapq.heappush(abiertos, inicio_nodo)

    while abiertos:
        nodo_actual = heapq.heappop(abiertos)
        if (nodo_actual.x, nodo_actual.y) == objetivo:
            camino = []
            while nodo_actual:
                camino.append((nodo_actual.x, nodo_actual.y))
                nodo_actual = nodo_actual.padre
            return camino[::-1]
        
        cerrados.add((nodo_actual.x, nodo_actual.y))
        for dx, dy in [(-1, 0), (1, 0), (0, -1), (0, 1)]:
            x_vecino, y_vecino = nodo_actual.x + dx, nodo_actual.y + dy
            if (0 <= x_vecino < filas and 0 <= y_vecino < columnas and (x_vecino, y_vecino) not in cerrados and mapa[x_vecino][y_vecino] == 0):
                nuevo_costo = nodo_actual.costo + 1
                heuristica = heuristica_manhattan(x_vecino, y_vecino, *objetivo)
                vecino_nodo = Nodo(x_vecino, y_vecino, costo=nuevo_costo, heuristica=heuristica, padre=nodo_actual)
                heapq.heappush(abiertos, vecino_nodo)
    return None

if __name__ == "__main__":
    mapa = [
        [0, 1, 0, 0, 0],
        [0, 1, 0, 1, 0],
        [0, 0, 0, 1, 0],
        [0, 1, 1, 1, 0],
        [0, 0, 0, 0, 0],
    ]

    inicio = (0, 0)
    objetivo = (4, 4)
    camino = a_star(inicio, objetivo, mapa)
    if camino:
        print("Camino encontrado:")
        print(camino)
    else:
        print("No se encontró camino")