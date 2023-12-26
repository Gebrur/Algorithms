#ifndef GRAPH_H
#define GRAPH_H

#include "vector.h" // Подключение пользовательского заголовочного файла с определением вектора

template<typename Data>
class Graph {
public:
    // Конструктор графа
    Graph(size_t vertexAmount) {
        vertexes.resize(vertexAmount); // Выделение памяти под вершины
        for (size_t i = 0; i < vertexAmount; i++) {
            vertexes.set(i, Vertex(static_cast<int>(i))); // Инициализация вершин
        }
        edgeMatrix.resize(vertexAmount * vertexAmount); // Выделение памяти под матрицу ребер
        for (size_t i = 0; i < edgeMatrix.size(); i++) {
            edgeMatrix.set(i, nullptr); // Инициализация матрицы ребер значением nullptr
        }
    }

    // Деструктор графа
    ~Graph() {
        for (size_t i = 0; i < edgeMatrix.size(); i++) {
            delete edgeMatrix.get(i); // Освобождение памяти от ребер
        }
    }

    // Конструктор копирования
    Graph(const Graph& a) {
        // Копирование вершин
        vertexes = a.vertexes;

        // Создаем временную копию объекта Vector<Edge*>
        Vector<Edge*> tempEdgeMatrix = a.edgeMatrix;

        // Используем swap для обмена данными между объектами Vector<Edge*>
        edgeMatrix.swap(tempEdgeMatrix);
    }

    // Оператор присваивания
    Graph& operator=(const Graph& other) {
        if (this == &other) {
            return *this;
        }

        // Освобождаем память от существующих объектов Edge
        for (size_t i = 0; i < edgeMatrix.size(); i++) {
            delete edgeMatrix.get(i);
        }

        // Создаем временные копии объектов
        Vector<Vertex> tempVertices = other.vertexes;
        Vector<Edge*> tempEdgeMatrix = other.edgeMatrix;

        // Используем swap для обмена данными между объектами Vector
        vertexes.swap(tempVertices);
        edgeMatrix.swap(tempEdgeMatrix);

        return *this;
    }

    // Структура Vertex, представляющая вершину графа
    struct Vertex {
    private:
        Data vertexData; // Данные вершины
    public:
        // Конструктор по умолчанию
        Vertex() : vertexData(Data()) {}

        // Конструктор с параметром
        Vertex(Data vertexData) {
            this->vertexData = vertexData;
        }

        // Метод для установки данных вершины
        void setVertexData(Data vertexData) {
            this->vertexData = vertexData;
        }

        // Метод для получения данных вершины
        Data getVertexData() {
            return this->vertexData;
        }
    };

    // Структура Edge, представляющая ребро графа
    struct Edge {
    private:
        Data edgeData; // Данные ребра
    public:
        // Конструктор с параметром
        Edge(Data data) {
            this->edgeData = data;
        }

        // Метод для установки данных ребра
        void setEdgeData(Data data) {
            this->edgeData = data;
        }

        // Метод для получения данных ребра
        Data getEdgeData() {
            return edgeData;
        }
    };

    // Метод для получения количества вершин в графе
    size_t getVertexAmount() const {
        return vertexes.size();
    }

    // Вложенная структура Iterator для обхода вершин графа (не используется)
    struct Iterator {
    private:
        Graph* graph;  // Указатель на граф
        size_t start;  // Индекс начальной вершины
        int end = -1;   // Индекс следующей смежной вершины

        // Метод для поиска индекса ближайшей вершины, смежной с текущей
        size_t getNearVertexIndex() {
            for (size_t i = end + 1; i < graph->getVertexAmount(); i++) {
                if (graph->isEdgeExist(start, i)) {
                    return static_cast<int>(i);
                }
            }
            return -1;
        }

    public:
        // Конструктор итератора
        Iterator(Graph* graph, size_t start) {
            this->graph = graph;
            this->start = start;
            this->end = getNearVertexIndex();
        }

        // Оператор разыменования
        bool operator *() {
            if (end != -1) {
                return true;
            } else {
                return false;
            }
        }

        // Префиксный оператор инкремента для перехода к следующей смежной вершине
        void operator ++() {
            end = getNearVertexIndex();
        }

        // Метод для получения индекса текущей смежной вершины
        size_t getIndex() const {
            return end;
        }

        // Метод для получения индекса начальной вершины
        int getStart() const {
            return start;
        }
    };

    // Метод для получения итератора, начинающего обход с заданной вершины
    Iterator getIterator(size_t start) {
        return Iterator(this, start);
    }

    // Метод для добавления вершины с данными vertex_data в граф
    size_t addVertex(Data vertex_data) {
        size_t index = vertexes.size();
        vertexes.resize(index + 1);
        vertexes.set(index, Vertex(vertex_data));

        size_t vertex_amount = getVertexAmount();

        Vector<Edge*> buffMatrix;
        buffMatrix.resize(vertex_amount * vertex_amount);
        for (size_t i = 0; i < vertex_amount; i++) {
            for (size_t j = 0; j < vertex_amount; j++) {
                buffMatrix.set((i * vertex_amount) + j, edgeMatrix.get(i * index + j));
            }
        }

        edgeMatrix.swap(buffMatrix);

        return index;
    }

    // Метод для удаления вершины по индексу index
void removeVertex(size_t index) {
    size_t _vertexAmount = getVertexAmount();

    // Проверка на корректность индекса
    if (index >= _vertexAmount) {
        return;
    }

    // Сдвиг вершин после удаленной
    for (size_t i = index; i < _vertexAmount - 1; i++) {
        vertexes.set(i, vertexes.get(i + 1));
    }
    vertexes.resize(_vertexAmount - 1);

    // Освобождение памяти от ребер, связанных с удаляемой вершиной
    for (size_t i = 0; i < _vertexAmount; i++) {
        Edge* edge = edgeMatrix.get(index * _vertexAmount + i);
        if (edge) {
            delete edge;
        }
        edge = edgeMatrix.get(i * _vertexAmount + index);
        if (edge) {
            delete edge;
        }
    }

    size_t vertex_amount = getVertexAmount();

    // Создание временной матрицы ребер
    Vector<Edge*> buffMatrix;
    buffMatrix.resize(vertex_amount * vertex_amount);

    // Заполнение временной матрицы ребер
    for (size_t i = 0; i < vertex_amount; i++) {
        for (size_t j = 0; j < vertex_amount; j++) {
            Edge* edge = edgeMatrix.get(((i + (i >= index)) * _vertexAmount) + (j + (j >= index)));
            buffMatrix.set((i * vertex_amount) + j, edge);
        }
    }

    // Замена текущей матрицы ребер временной
    edgeMatrix.swap(buffMatrix);
}

    // Метод для получения вершины по индексу index
    Vertex getVertex(size_t index) {
        return vertexes.get(index);
    }

    // Метод для добавления ребра между вершинами start_vertex_index и end_vertex_index с данными edge_data
    void addEdge(size_t start_vertex_index, size_t end_vertex_index, Data edge_data) {
        size_t vertex_amount = getVertexAmount();

        Edge* existingEdge = edgeMatrix.get(start_vertex_index * vertex_amount + end_vertex_index);

        if (existingEdge == nullptr) {
            Edge* newEdge = new Edge(edge_data);
            edgeMatrix.set(start_vertex_index * vertex_amount + end_vertex_index, newEdge);
        } else {
            existingEdge->setEdgeData(edge_data);
        }
    }

    // Метод для удаления ребра между вершинами start_vertex_index и end_vertex_index
    void removeEdge(size_t start_vertex_index, size_t end_vertex_index) {
        size_t vertex_amount = getVertexAmount();
        Edge* edge = edgeMatrix.get(start_vertex_index * vertex_amount + end_vertex_index);
        delete edge;
        edgeMatrix.set(start_vertex_index * vertex_amount + end_vertex_index, nullptr);
    }

    // Метод для получения ребра между вершинами start_vertex_index и end_vertex_index
    Edge* getEdge(size_t start_vertex_index, size_t end_vertex_index) {
        size_t vertex_amount = getVertexAmount();
        return edgeMatrix.get(start_vertex_index * vertex_amount + end_vertex_index);
    }

    // Метод для проверки существования ребра между вершинами start_vertex_index и end_vertex_index
    bool isEdgeExist(size_t start_vertex_index, size_t end_vertex_index) {
        return getEdge(start_vertex_index, end_vertex_index) != nullptr;
    }

private:
    Vector<Vertex> vertexes; 
    Vector<Edge*> edgeMatrix; 
};

#endif 
