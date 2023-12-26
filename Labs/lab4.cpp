#include <iostream> // Подключение заголовочного файла для ввода/вывода
#include <climits> // Подключение заголовочного файла с константами для типов данных
#include "graph.h" // Подключение пользовательского заголовочного файла с определением графа
#include <cassert> // Подключение заголовочного файла для проверки утверждений

const int INF = INT_MAX; // Константа, представляющая бесконечность для весов ребер

template<typename Data>
void modifiedFloydWarshall(const Graph<Data>& graph, size_t vertex_amount) {
    Graph<Data> mutableGraph = graph; // Создание копии графа для модификации

    // Применение алгоритма Флойда-Уоршелла для поиска самого длинного пути
    for (size_t k = 0; k < vertex_amount; ++k) {
        for (size_t i = 0; i < vertex_amount; ++i) {
            for (size_t j = 0; j < vertex_amount; ++j) {
                typename Graph<Data>::Edge* edgeIK = mutableGraph.getEdge(i, k); // Получение ребра от i к k
                typename Graph<Data>::Edge* edgeKJ = mutableGraph.getEdge(k, j); // Получение ребра от k к j

                if (edgeIK && edgeKJ) {
                    int weightIK = edgeIK->getEdgeData(); // Вес ребра от i к k
                    int weightKJ = edgeKJ->getEdgeData(); // Вес ребра от k к j

                    // Проверка условия: если нет ребра от i к j или условие обновления веса пути выполнено
                    if (mutableGraph.getEdge(i, j) == nullptr ||
                        (weightIK != INF && weightKJ != INF &&
                        mutableGraph.getVertex(i).getVertexData() != INF &&
                        mutableGraph.getVertex(k).getVertexData() != INF &&
                        mutableGraph.getVertex(j).getVertexData() != INF &&
                        mutableGraph.getEdge(i, j)->getEdgeData() != INF &&
                        mutableGraph.getVertex(i).getVertexData() + weightIK + weightKJ >
                        mutableGraph.getVertex(j).getVertexData())) {

                        // Если нет ребра от i к j, то добавляем новое ребро с весом равным сумме весов ребер от i к k и от k к j
                        if (mutableGraph.getEdge(i, j) == nullptr) {
                            mutableGraph.addEdge(i, j, mutableGraph.getVertex(i).getVertexData() + weightIK + weightKJ);
                        } else { // Иначе обновляем вес существующего ребра
                            mutableGraph.getEdge(i, j)->setEdgeData(mutableGraph.getVertex(i).getVertexData() + weightIK + weightKJ);
                        }
                    }
                }
            }
        }
    }

    std::cout << "Adjacency Matrix:\n"; // Вывод матрицы смежности
    for (size_t i = 0; i < vertex_amount; ++i) {
        for (size_t j = 0; j < vertex_amount; ++j) {
            if (mutableGraph.getEdge(i, j)) {
                std::cout << "1 "; // Вывод "1" если есть ребро между вершинами i и j
            } else {
                std::cout << "0 "; // Вывод "0" если нет ребра между вершинами i и j
            }
        }
        std::cout << "\n";
    }

    size_t start_vertex = 0; // Начальная вершина пути
    size_t end_vertex = vertex_amount - 1; // Конечная вершина пути
    if (mutableGraph.getEdge(start_vertex, end_vertex) != nullptr) {
        std::cout << "Longest path from vertex " << start_vertex << " to vertex " << end_vertex << " is "
                  << mutableGraph.getEdge(start_vertex, end_vertex)->getEdgeData() << "\n"; // Вывод самого длинного пути
    } else {
        std::cout << "Longest path from vertex " << start_vertex << " to vertex " << end_vertex << " is NULL\n"; // Вывод, если путь отсутствует
    }
}

int main() {
    size_t vertex_amount = 0; // Количество вершин в графе
    int N = 0; // Количество ребер в графе

    std::cout << "Input vertex_amount> ";
    std::cin >> vertex_amount; // Ввод количества вершин

    Graph<int> graph(vertex_amount); // Создание графа с заданным количеством вершин

    std::cout << "Input size read data> ";
    std::cin >> N; // Ввод количества ребер

    for (int i = 0; i < N; i++) {
        size_t start_vertex_index, end_vertex_index;
        int edge_data;
        std::cout << "input " << i << " data (size_t start_vertex_index, size_t end_vertex_index, Data edge_data)> ";
        std::cin >> start_vertex_index >> end_vertex_index >> edge_data; // Ввод данных о ребре
        graph.addEdge(start_vertex_index, end_vertex_index, edge_data); // Добавление ребра в граф
    }

    modifiedFloydWarshall(graph, vertex_amount); // Вызов функции для поиска самого длинного пути

    return 0; // Возвращение нуля, указывающего на успешное завершение программы
}
