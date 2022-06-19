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


void Mgraph::RemoveEdge(uint32_t src, uint32_t dst) {
    Graph::RemoveEdge(src, dst);
}

void Mgraph::RemoveVertex(uint32_t vertex) {
    Graph::RemoveVertex(vertex);
}

const Motif &Mgraph::BuildMotif(uint32_t src, uint32_t dst, label_type label) {


    Motif &m = GetMotif(src, dst);
    const std::vector<uint32_t> &s_neighbor = GetNeighbors(src);
    const std::vector<uint32_t> &d_neighbor = GetNeighbors(dst);
    const auto &s_edge_label = GetEdgeLabels(src);
    const auto &d_edge_label = GetEdgeLabels(dst);
    const auto &s_neighbor_label = GetNeighborLabels(src);
    const auto &d_neighbor_label = GetNeighborLabels(dst);


    m.SetLables(vertex_label_[src], vertex_label_[dst]);
    // triangle
    CountTriangle(m, s_neighbor, d_neighbor, s_edge_label, d_edge_label, s_neighbor_label, d_neighbor_label);
    // update src neighbor distribution
    CountNeighborDis(m, s_edge_label, s_neighbor_label, {vertex_label_[dst], label}, true);

    CountNeighborDis(m, d_edge_label, d_neighbor_label, {vertex_label_[src], label}, false);

    Motif &m2 = GetMotif(dst, src);
    m2 = m;
    return m;

}

const Motif &Mgraph::UpdateMotif(uint32_t src, uint32_t dst, label_type label) {

    const std::vector<uint32_t> &s_neighbor = GetNeighbors(src);
    const std::vector<uint32_t> &d_neighbor = GetNeighbors(dst);
    const auto &s_edge_label = GetEdgeLabels(src);
    const auto &d_edge_label = GetEdgeLabels(dst);
    const auto &s_neighbor_label = GetNeighborLabels(src);
    const auto &d_neighbor_label = GetNeighborLabels(dst);

    // insert motif
    auto lower = std::lower_bound(neighbors_[src].begin(), neighbors_[src].end(), dst);
    size_t distance = std::distance(neighbors_[src].begin(), lower);
    edge_motif_[src].insert(edge_motif_[src].begin() + distance, Motif());
    Motif &m = edge_motif_[src][distance];

    m.SetLables(vertex_label_[src], vertex_label_[dst]);
    // triangle
    CountTriangle(m, s_neighbor, d_neighbor, s_edge_label, d_edge_label, s_neighbor_label, d_neighbor_label);
    // update src neighbor distribution
    CountNeighborDis(m, s_edge_label, s_neighbor_label, {vertex_label_[dst], label}, true);

    CountNeighborDis(m, d_edge_label, d_neighbor_label, {vertex_label_[src], label}, false);

    lower = std::lower_bound(neighbors_[dst].begin(), neighbors_[dst].end(), src);
    distance = std::distance(neighbors_[dst].begin(), lower);
    edge_motif_[dst].insert(edge_motif_[dst].begin() + distance, m);

    return m;
}


// neighbor is ordered by id
void Mgraph::CountTriangle(Motif &m, const std::vector<uint32_t> &neighbor1, const std::vector<uint32_t> &neighbor2,
                           const std::vector<label_type> &edge_label1, const std::vector<label_type> &edge_label2,
                           const std::vector<label_type> &neighbor_label1,
                           const std::vector<label_type> &neighbor_label2) {
    int s1 = neighbor1.size(), s2 = neighbor2.size(), i = 0, j = 0;
    while (i < s1 && j < s2) {
        if (neighbor1[i] < neighbor2[j])
            ++i;
        else if (neighbor1[i] > neighbor2[j])
            ++j;
        else {
            m.UpdateTriangle(edge_label1[i], neighbor_label1[i], edge_label2[j], 1);
            ++j;
            ++i;
        }
    }


}


void Mgraph::CountNeighborDis(Motif &m, const std::vector<label_type> &edge_label,
                              const std::vector<label_type> &neighbor_label,
                              std::pair<label_type, label_type> edge_pair, bool is_src) {

    for (int i = 0; i < edge_label.size(); ++i) {
        label_type edge_l = edge_label[i], vertex_l = neighbor_label[i];
        if (is_src)
            m.UpdateSrc(vertex_l, edge_l, 1);
        else
            m.UpdateDst(vertex_l, edge_l, 1);
    }
    if (is_src)
        // remove duplicate ones
        m.UpdateSrc(edge_pair.first, edge_pair.second, -1);
    else
        m.UpdateDst(edge_pair.first, edge_pair.second, -1);
}


Motif &Mgraph::GetMotif(uint32_t src, uint32_t dst) {
    auto lower = std::lower_bound(neighbors_[src].begin(), neighbors_[src].end(), dst);
    size_t distance = std::distance(neighbors_[src].begin(), lower);
    return *(edge_motif_[src].begin() + distance);
}

void Mgraph::LoadGraphByFile(std::string &graph_path) {
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
    edge_motif_.resize(vertex_nums_);
    for(int i=0;i<vertex_nums_;++i){
        edge_motif_[i].resize(neighbors_[i].size(),Motif());
    }


    infile.close();
}








