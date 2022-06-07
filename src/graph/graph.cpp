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

void Graph::Dump(std::string &graph_path) {
    std::ofstream output(graph_path);

    // v: vertex  e: edge(dst,src,type)
    //first line   vertex nums   edge nums
    output << vertex_nums_ << " " << edge_nums_ << "\n";
    // second line  the size of vertex label , the size of edge label
    output << vertex_label_type_nums_ << " " << edge_label_type_nums_ << "\n";
    for (uint32_t vertex = 0; vertex < vertex_label_.size(); ++vertex) {
        output << "v" << " " << vertex << " " << vertex_label_[vertex] << "\n";
    }
    for (uint32_t src = 0; src < neighbors_.size(); ++src) {
        for (uint32_t idx = 0; idx < neighbors_[src].size(); ++idx) {
            output << "e" << src << " " << neighbors_[src][idx] << " "
                   << edge_label_[src][idx];
        }
    }
    output.close();
}
