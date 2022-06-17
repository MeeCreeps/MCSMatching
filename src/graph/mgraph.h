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
#include "algorithm"
#include "streaming.h"

class Mgraph : public Graph {

public:
    void AddEdge(uint32_t src, uint32_t dst, label_type label) override;

    void RemoveEdge(uint32_t src, uint32_t dst) override;

    void AddVertex(uint32_t vertex, label_type label) override;

    void RemoveVertex(uint32_t vertex) override;


    const Motif &BuildMotif(uint32_t src, uint32_t dst,label_type label);

    //const Motif &UpdateAndGetMotif(uint32_t src, uint32_t dst);

    const std::vector<Motif> &GetNeighborMotif(uint32_t src) { return edge_motif_[src]; }


    //const Motif& UpdateStreaming(const Streaming::StreamUint & uint);

    Motif &GetMotif(uint32_t src, uint32_t dst);

    void CountTriangle(Motif &m, const std::vector<uint32_t> &neighbor1, const std::vector<uint32_t> &neighbor2,
                       const std::vector<label_type> &neighbor1_label, const std::vector<label_type> &neighbor2_label);

    void CountStar(Motif &m, const std::vector<uint32_t> &neighbor1, const std::vector<uint32_t> &neighbor2,
                   const std::vector<size_t> &neighbor1_label_dis, const std::vector<size_t> &neighbor2_label_dis,label_type label);

    void CountLine(Motif &m, const std::vector<uint32_t> &neighbor1, const std::vector<uint32_t> &neighbor2,
                   const std::vector<size_t> &neighbor1_label_dis, const std::vector<size_t> &neighbor2_label_dis,label_type label);

protected:

    std::vector<std::vector<Motif>> edge_motif_;


};


#endif //MULTISUBGRAPHMATCHING_MGRAPH_H
