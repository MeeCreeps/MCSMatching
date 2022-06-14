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


#ifdef UNDIRECTED
    lower = std::lower_bound(neighbors_[dst].begin(), neighbors_[dst].end(), src);
    if (lower != neighbors_[dst].end() && *lower == dst) return;
    // ordered by id

    distance = std::distance(neighbors_[dst].begin(), lower);
    neighbors_[dst].insert(lower, src);
    edge_label_[dst].insert(edge_label_[dst].begin() + distance, label);
#endif
    edge_nums_++;
    // label started from 0
    edge_label_type_nums_ = std::max(edge_label_type_nums_, label + 1);
}

void Graph::AddVertex(uint32_t vertex, label_type label) {
    if (vertex >= vertex_label_.size()) {
        vertex_label_.resize(vertex + 1, NON_EXIST);
        neighbors_.resize(vertex + 1);
        edge_label_.resize(vertex + 1);

    }
    if (vertex_label_[vertex] == NON_EXIST) {
        vertex_label_[vertex] = label;
        vertex_nums_++;
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


//    infile >> vertex_label_type_nums_ >> edge_label_type_nums_;
    //vertex_label_.resize(vertex_nums_,NON_EXIST);
    //neighbors_.resize(vertex_nums_);
    //edge_label_.resize(vertex_nums_);

    uint32_t vertex1, vertex2;
    label_type label;
    while (infile >> type) {
        if (type == 'v') {
            infile >> vertex1 >> label;
            AddVertex(vertex1, label);
            // vertex_label_[vertex1] = label;
        } else if (type == 'e') {
            label=0;
            infile >> vertex1 >> vertex2 >>label;
            AddEdge(vertex1, vertex2, label);

//            neighbors_[vertex1].push_back(vertex2);
//            edge_label_[vertex1].push_back(label);
//            neighbors_[vertex2].push_back(vertex1);
//            edge_label_[vertex2].push_back(label);

        }
    }
    infile.close();

}

void Graph::Dump(std::string &graph_path) {
    std::ofstream output(graph_path);

    // v: vertex  e: edge(dst,src,type)
    //first line   vertex nums   edge nums
    output << vertex_nums_ << " " << edge_nums_ << "\n";
    // second line  the size of vertex label , the size of edge label
    //output << vertex_label_type_nums_ << " " << edge_label_type_nums_ << "\n";
    for (uint32_t vertex = 0; vertex < vertex_label_.size(); ++vertex) {
        if (vertex_label_[vertex] == NON_EXIST)
            continue;
        output << "v" << " " << vertex << " " << vertex_label_[vertex] << "\n";
    }
    for (uint32_t src = 0; src < neighbors_.size(); ++src) {
        for (uint32_t idx = 0; idx < neighbors_[src].size(); ++idx) {
            if (neighbors_[src][idx] == NON_EXIST)
                continue;
            output << "e"<<" " << src << " " << neighbors_[src][idx] << " "
                   << edge_label_[src][idx]<<"\n";
        }
    }
    output.close();
}
