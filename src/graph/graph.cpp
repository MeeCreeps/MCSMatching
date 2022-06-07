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


void Graph::LoadGraphByFile(std::string &graph_path) {

    std::ifstream infile(graph_path);
    if (!infile.good()) {
        std::cout << "graph file doesn't exist ! " << std::endl;
        return;
    }

    char type;

    infile >> vertex_nums_ >> edge_nums_;
    infile >> vertex_label_type_nums_ >> edge_label_type_nums_;
    vertex_label_.resize(vertex_nums_);
    neighbors_.resize(vertex_nums_);
    edge_label_.resize(vertex_nums_);

    uint32_t vertex1, vertex2;
    label_type label;
    while (infile >> type) {
        if (type == 'v') {
            infile >> vertex1 >> label;
            vertex_label_[vertex1] = label;
        } else if (type == 'e') {
            infile >> vertex1 >> vertex2 >> label;

            neighbors_[vertex1].push_back(vertex2);
            edge_label_[vertex1].push_back(label);
#ifdef UNDIRECTED
            neighbors_[vertex2].push_back(vertex1);
            edge_label_[vertex2].push_back(label);
#endif
        }
    }
    infile.close();

}