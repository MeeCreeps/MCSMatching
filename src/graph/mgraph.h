//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef MULTISUBGRAPHMATCHING_MGRAPH_H
#define MULTISUBGRAPHMATCHING_MGRAPH_H

#include "graph.h"
#include "matching-algo/motif/motif.h"

class Mgraph : public Graph {

public:
    void AddEdge(uint32_t src, uint32_t dst, label_type label) override;

    void RemoveEdge(uint32_t src, uint32_t dst) override;

    void AddVertex(uint32_t vertex, label_type label) override;

    void RemoveVertex(uint32_t vertex) override;


    void BuildMotif(Motif &m, uint32_t src, uint32_t dst);

    void SetMotif(Motif &m,uint32_t src,uint32_t idx){
        if(edge_motif_[src].size()<neighbors_[src].size())
            edge_motif_[src].resize(neighbors_[src].size());
        edge_motif_[src][idx]=std::move(m);
    }

    const std::vector<Motif> &GetNeighborMotif(uint32_t src) { return edge_motif_[src]; }

    const Motif &GetMotif(uint32_t src, uint32_t dst) { return edge_motif_[src][dst]; }

    void CountTriangle(Motif &m, const std::vector<uint32_t> &neighbor1, const std::vector<uint32_t> &neighbor2,
                       const std::vector<label_type> &neighbor1_label, const std::vector<label_type> &neighbor2_label);

    void CountStar(Motif &m, const std::vector<uint32_t> &neighbor1, const std::vector<uint32_t> &neighbor2,
                   const std::vector<size_t> &neighbor1_label_dis, const std::vector<size_t> &neighbor2_label_dis);

    void CountLine(Motif &m, const std::vector<uint32_t> &neighbor1, const std::vector<uint32_t> &neighbor2,
                   const std::vector<size_t> &neighbor1_label_dis, const std::vector<size_t> &neighbor2_label_dis);

protected:

    std::vector<std::vector<Motif>> edge_motif_;


};


#endif //MULTISUBGRAPHMATCHING_MGRAPH_H
