//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#include "mgraph.h"

void Mgraph::AddEdge(uint32_t src, uint32_t dst, label_type label) {
    auto lower = std::lower_bound(neighbors_[src].begin(), neighbors_[src].end(), dst);
    if (lower != neighbors_[src].end() && *lower == dst) return;
    // ordered by id
    size_t distance = std::distance(neighbors_[src].begin(), lower);
    neighbors_[src].insert(lower, dst);
    edge_label_[src].insert(edge_label_[src].begin() + distance, label);

    // build motif
    // perhaps insert is not efficient
    Motif m;
    BuildMotif(m, src, dst);
    edge_motif_[src].insert(edge_motif_[src].begin() + distance, m);


    lower = std::lower_bound(neighbors_[dst].begin(), neighbors_[dst].end(), src);
    if (lower != neighbors_[dst].end() && *lower == dst) return;
    // ordered by id
    distance = std::distance(neighbors_[dst].begin(), lower);
    neighbors_[dst].insert(lower, src);
    edge_label_[dst].insert(edge_label_[dst].begin() + distance, label);

    // insert motif bidirectionally
    edge_motif_[dst].insert(edge_motif_[dst].begin() + distance, m);


    edge_nums_++;
    // label started from 0
    edge_label_type_nums_ = std::max(edge_label_type_nums_, label + 1);
}

void Mgraph::RemoveEdge(uint32_t src, uint32_t dst) {
    Graph::RemoveEdge(src, dst);
}

void Mgraph::AddVertex(uint32_t vertex, label_type label) {
    if (vertex >= vertex_label_.size())
        edge_motif_.resize(vertex + 1);
    Graph::AddVertex(vertex,label);
}

void Mgraph::RemoveVertex(uint32_t vertex) {
    Graph::RemoveVertex(vertex);
}

void Mgraph::BuildMotif(Motif &m, uint32_t vertex, uint32_t idx) {

}
