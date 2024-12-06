#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct Nodo {
    int x, y;
    int costo;
    int heuristica;
    struct Nodo* padre;
} Nodo;

typedef struct {
    Nodo** data;
    int size;
    int capacity;
} MinHeap;

MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->data = (Nodo**)malloc(sizeof(Nodo*) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(Nodo** a, Nodo** b) {
    Nodo* temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap* heap, int index) {
    while (index > 0 && (heap->data[index]->costo + heap->data[index]->heuristica) < (heap->data[(index - 1) / 2]->costo + heap->data[(index - 1) / 2]->heuristica)) {
        swap(&heap->data[index], &heap->data[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void heapifyDown(MinHeap* heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && (heap->data[left]->costo + heap->data[left]->heuristica) < (heap->data[smallest]->costo + heap->data[smallest]->heuristica)) {
        smallest = left;
    }

    if (right < heap->size && (heap->data[right]->costo + heap->data[right]->heuristica) < (heap->data[smallest]->costo + heap->data[smallest]->heuristica)) {
        smallest = right;
    }

    if (smallest != index) {
        swap(&heap->data[index], &heap->data[smallest]);
        heapifyDown(heap, smallest);
    }
}

void push(MinHeap* heap, Nodo* node) {
    if (heap->size == heap->capacity) {
        heap->capacity *= 2;
        heap->data = (Nodo**)realloc(heap->data, sizeof(Nodo*) * heap->capacity);
    }
    heap->data[heap->size] = node;
    heapifyUp(heap, heap->size);
    heap->size++;
}

Nodo* pop(MinHeap* heap) {
    if (heap->size == 0) return NULL;
    Nodo* root = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapifyDown(heap, 0);
    return root;
}

int heuristica_manhattan(int x1, int y1, int x2, int y2) {
    return abs(x1 - x2) + abs(y1 - y2);
}

Nodo* createNodo(int x, int y, int costo, int heuristica, Nodo* padre) {
    Nodo* node = (Nodo*)malloc(sizeof(Nodo));
    node->x = x;
    node->y = y;
    node->costo = costo;
    node->heuristica = heuristica;
    node->padre = padre;
    return node;
}

int** createMap(int filas, int columnas) {
    int** mapa = (int**)malloc(filas * sizeof(int*));
    for (int i = 0; i < filas; i++) {
        mapa[i] = (int*)malloc(columnas * sizeof(int));
    }
    return mapa;
}

void freeMap(int** mapa, int filas) {
    for (int i = 0; i < filas; i++) {
        free(mapa[i]);
    }
    free(mapa);
}

void printPath(Nodo* nodo) {
    if (nodo == NULL) return;
    printPath(nodo->padre);
    printf("(%d, %d) ", nodo->x, nodo->y);
}

Nodo* a_star(int inicio[2], int objetivo[2], int** mapa, int filas, int columnas) {
    MinHeap* abiertos = createMinHeap(100);
    int** cerrados = createMap(filas, columnas);
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            cerrados[i][j] = 0;
        }
    }

    Nodo* inicio_nodo = createNodo(inicio[0], inicio[1], 0, heuristica_manhattan(inicio[0], inicio[1], objetivo[0], objetivo[1]), NULL);
    push(abiertos, inicio_nodo);

    while (abiertos->size > 0) {
        Nodo* nodo_actual = pop(abiertos);
        if (nodo_actual->x == objetivo[0] && nodo_actual->y == objetivo[1]) {
            freeMap(cerrados, filas);
            return nodo_actual;
        }

        cerrados[nodo_actual->x][nodo_actual->y] = 1;

        int direcciones[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
        for (int i = 0; i < 4; i++) {
            int x_vecino = nodo_actual->x + direcciones[i][0];
            int y_vecino = nodo_actual->y + direcciones[i][1];

            if (x_vecino >= 0 && x_vecino < filas && y_vecino >= 0 && y_vecino < columnas && cerrados[x_vecino][y_vecino] == 0 && mapa[x_vecino][y_vecino] == 0) {
                int nuevo_costo = nodo_actual->costo + 1;
                int heuristica = heuristica_manhattan(x_vecino, y_vecino, objetivo[0], objetivo[1]);
                Nodo* vecino_nodo = createNodo(x_vecino, y_vecino, nuevo_costo, heuristica, nodo_actual);
                push(abiertos, vecino_nodo);
            }
        }
    }

    freeMap(cerrados, filas);
    return NULL;
}

int main() {
    int filas = 5;
    int columnas = 5;
    int** mapa = createMap(filas, columnas);

    int mapa_data[5][5] = {
        {0, 1, 0, 0, 0},
        {0, 1, 0, 1, 0},
        {0, 0, 0, 1, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };

    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < columnas; j++) {
            mapa[i][j] = mapa_data[i][j];
        }
    }

    int inicio[2] = {0, 0};
    int objetivo[2] = {4, 4};
    Nodo* camino = a_star(inicio, objetivo, mapa, filas, columnas);

    if (camino) {
        printf("Camino encontrado:\n");
        printPath(camino);
        printf("\n");
    } else {
        printf("No se encontró camino\n");
    }

    freeMap(mapa, filas);
    return 0;
}