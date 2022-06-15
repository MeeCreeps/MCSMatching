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

    const std::vector<Motif> &GetNeighborMotif(uint32_t src) { return edge_motif_[src]; }


protected:

    std::vector<std::vector<Motif>> edge_motif_;


};


#endif //MULTISUBGRAPHMATCHING_MGRAPH_H
