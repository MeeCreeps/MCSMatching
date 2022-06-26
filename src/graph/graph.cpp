//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#include "graph.h"

void Graph::AddEdge(uint32_t src, uint32_t dst, label_type label) {
    // make sure that the edge is not duplicate
    auto lower = std::lower_bound(neighbors_[src].begin(), neighbors_[src].end(), dst);
    if (lower != neighbors_[src].end() && *lower == dst) return;
    // ordered by id

    size_t distance = std::distance(neighbors_[src].begin(), lower);
    neighbors_[src].insert(lower, dst);
    edge_label_[src].insert(edge_label_[src].begin() + distance, label);
    neighbors_label_[src].insert(neighbors_label_[src].begin() + distance, vertex_label_[dst]);

    lower = std::lower_bound(neighbors_[dst].begin(), neighbors_[dst].end(), src);
    if (lower != neighbors_[dst].end() && *lower == dst) return;
    // ordered by id
    distance = std::distance(neighbors_[dst].begin(), lower);
    neighbors_[dst].insert(lower, src);
    edge_label_[dst].insert(edge_label_[dst].begin() + distance, label);
    neighbors_label_[dst].insert(neighbors_label_[dst].begin() + distance, vertex_label_[src]);

    edge_nums_++;
    // label started from 0
    edge_label_size_ = std::max(edge_label_size_, label + 1);


}

void Graph::AddVertex(uint32_t vertex, label_type label) {
    if (vertex >= vertex_label_.size()) {
        vertex_label_.resize(vertex + 1, NON_EXIST);
        neighbors_.resize(vertex + 1);
        neighbors_label_.resize(vertex + 1);
        edge_label_.resize(vertex + 1);
        vertex_nums_=vertex+1;
        vertex_label_size_=std::max(vertex_label_size_,label+1);
    }
    if (vertex_label_[vertex] == NON_EXIST) {
        vertex_label_[vertex] = label;
    }

}

void Graph::RemoveVertex(uint32_t vertex) {

}

void Graph::RemoveEdge(uint32_t src, uint32_t dst) {

}


 void Graph::LoadGraphByFile(std::string &graph_path) {

    std::ifstream infile(graph_path);
    if (!infile.good()) {
        std::cout << "graph file doesn't exist ! " << std::endl;
        return;
    }

    char type;
    uint32_t vertex1, vertex2;
    label_type label;
    while (infile >> type) {
        if (type == 'v') {
            infile >> vertex1 >> label;
            AddVertex(vertex1, label);
        } else if (type == 'e') {
            infile >> vertex1 >> vertex2 >> label;
            AddEdge(vertex1, vertex2, label);

        }
    }
    // for sake of coding easily , needed to be changed later .

    infile.close();

}

void Graph::Dump(std::string &graph_path) {
    std::ofstream output(graph_path);

    // v: vertex label  e: edge(dst,src,label)
    for (uint32_t vertex = 0; vertex < vertex_label_.size(); ++vertex) {
        if (vertex_label_[vertex] == NON_EXIST)
            continue;
        output << "v" << " " << vertex << " " << vertex_label_[vertex] << "\n";
    }
    for (uint32_t src = 0; src < neighbors_.size(); ++src) {
        for (uint32_t idx = 0; idx < neighbors_[src].size(); ++idx) {
            if (neighbors_[src][idx] == NON_EXIST)
                continue;
#ifdef UNDIRECTED
            if (src <= neighbors_[src][idx])
                continue;
#endif


            output << "e" << " " << src << " " << neighbors_[src][idx] << " "
                   << edge_label_[src][idx] << "\n";
        }
    }
    output.close();
}
