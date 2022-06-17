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


    lower = std::lower_bound(neighbors_[dst].begin(), neighbors_[dst].end(), src);
    if (lower != neighbors_[dst].end() && *lower == dst) return;
    // ordered by id
    distance = std::distance(neighbors_[dst].begin(), lower);
    neighbors_[dst].insert(lower, src);
    edge_label_[dst].insert(edge_label_[dst].begin() + distance, label);


    edge_nums_++;
    // label started from 0
    edge_label_size_ = std::max(edge_label_size_, label + 1);
    if (neighbor_label_distribution_[src].size() < edge_label_size_)
        neighbor_label_distribution_[src].resize(edge_label_size_, 0);
    if (neighbor_label_distribution_[dst].size() < edge_label_size_)
        neighbor_label_distribution_[dst].resize(edge_label_size_, 0);
    neighbor_label_distribution_[src][label] += 1;
    neighbor_label_distribution_[dst][label] += 1;
}

void Mgraph::RemoveEdge(uint32_t src, uint32_t dst) {
    Graph::RemoveEdge(src, dst);
}

void Mgraph::AddVertex(uint32_t vertex, label_type label) {
    if (vertex >= vertex_label_.size())
        edge_motif_.resize(vertex + 1);
    Graph::AddVertex(vertex, label);
}

void Mgraph::RemoveVertex(uint32_t vertex) {
    Graph::RemoveVertex(vertex);
}

const Motif &Mgraph::BuildMotif(uint32_t src, uint32_t dst, label_type label) {

    if (edge_motif_[src].size() < neighbors_[src].size())
        edge_motif_[src].resize(neighbors_[src].size(), Motif(edge_label_size_));
    Motif &m = GetMotif(src, dst);
    const std::vector<uint32_t> &s_neighbor = GetNeighbors(src);
    const std::vector<uint32_t> &d_neighbor = GetNeighbors(dst);
    const auto &s_neighbor_label = GetNeighborLabels(src);
    const auto &d_neighbor_label = GetNeighborLabels(dst);
    const auto &s_neighbor_dis = GetNeighborLabelDistribution(src);
    const auto &d_neighbor_dis = GetNeighborLabelDistribution(dst);
    // triangle
    CountTriangle(m, s_neighbor, d_neighbor, s_neighbor_label, d_neighbor_label);
    CountLine(m, s_neighbor, d_neighbor, s_neighbor_dis, d_neighbor_dis, label);
    CountStar(m, s_neighbor, d_neighbor, s_neighbor_dis, d_neighbor_dis, label);

    if (edge_motif_[dst].size() < neighbors_[dst].size())
        edge_motif_[dst].resize(neighbors_[dst].size(), Motif(edge_label_size_));

    Motif &m2 = GetMotif(dst, src);
    m2 = m;
    return m;
}

// neighbor is ordered by id
void
Mgraph::CountTriangle(Motif &m, const std::vector<uint32_t> &neighbor1, const std::vector<uint32_t> &neighbor2,
                      const std::vector<label_type> &neighbor1_label, const std::vector<label_type> &neighbor2_label) {
    int s1 = neighbor1.size(), s2 = neighbor2.size(), i = 0, j = 0;
    while (i < s1 && j < s2) {
        if (neighbor1[i] < neighbor2[j])
            ++i;
        else if (neighbor1[i] > neighbor2[j])
            ++j;
        else {
            m.AddTriangle(neighbor1_label[i], neighbor2_label[j], 1);
            ++j;
            ++i;
        }
    }

}

void Mgraph::CountStar(Motif &m, const std::vector<uint32_t> &neighbor1, const std::vector<uint32_t> &neighbor2,
                       const std::vector<size_t> &neighbor1_label_dis, const std::vector<size_t> &neighbor2_label_dis,
                       label_type label) {

    // contain triangles , duplicate
    // label distribution
    int nums, i_label_num1, i_label_num2;
    for (int i = 0; i < edge_label_size_; ++i) {

        if (i == label) {
            // neighbor contains the edge
            i_label_num1 = neighbor1_label_dis[i] - 1;
            i_label_num2 = neighbor2_label_dis[i] - 1;
        } else {
            i_label_num1 = neighbor1_label_dis[i];
            i_label_num2 = neighbor2_label_dis[i];
        }
        nums = i_label_num1 * i_label_num2;


        int nums = i_label_num1 * (i_label_num1 - 1) / 2 +
                   i_label_num2 * (i_label_num2 - 1) / 2;
        m.AddStar(i, i, nums);
        for (int j = i + 1; j < edge_label_size_; ++j) {
            nums = i_label_num1 * neighbor1_label_dis[j] + i_label_num1 * neighbor2_label_dis[j];
            m.AddStar(i, j, nums);
        }
    }
}

void Mgraph::CountLine(Motif &m, const std::vector<uint32_t> &neighbor1, const std::vector<uint32_t> &neighbor2,
                       const std::vector<size_t> &neighbor1_label_dis, const std::vector<size_t> &neighbor2_label_dis,
                       label_type label) {
    int nums, i_label_num1, i_label_num2;
    for (int i = 0; i < edge_label_size_; ++i) {
        // when label is same
        if (i == label) {
            // neighbor contains the edge
            i_label_num1 = neighbor1_label_dis[i] - 1;
            i_label_num2 = neighbor2_label_dis[i] - 1;
        } else {
            i_label_num1 = neighbor1_label_dis[i];
            i_label_num2 = neighbor2_label_dis[i];
        }
        nums = i_label_num1 * i_label_num2;
        m.AddLine(i, i, nums);
        for (int j = i + 1; j < edge_label_size_; ++j) {
            nums = i_label_num1 * neighbor2_label_dis[j] + i_label_num2 * neighbor1_label_dis[j];
            m.AddLine(i, j, nums);
        }
    }

}


Motif &Mgraph::GetMotif(uint32_t src, uint32_t dst) {

    auto lower = std::lower_bound(neighbors_[src].begin(), neighbors_[src].end(), dst);
    size_t distance = std::distance(neighbors_[src].begin(), lower);
//    if (*lower != dst || lower == neighbors_[src].end())
//        edge_motif_[src].insert(edge_motif_[src].begin() + distance, Motif(edge_label_size_));
    return *(edge_motif_[src].begin() + distance);
}




