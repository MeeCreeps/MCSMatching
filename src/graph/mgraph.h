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




    void RemoveEdge(uint32_t src, uint32_t dst) override;


    void RemoveVertex(uint32_t vertex) override;

    void LoadGraphByFile(std::string &graph_path) override;
    const Motif &BuildMotif(uint32_t src, uint32_t dst, label_type label);
    // edge update first
    const Motif &UpdateMotif(uint32_t src, uint32_t dst, label_type label);

    //const Motif &UpdateAndGetMotif(uint32_t src, uint32_t dst);

    const std::vector<Motif> &GetNeighborMotif(uint32_t src) { return edge_motif_[src]; }


    //const Motif& UpdateStreaming(const Streaming::StreamUint & uint);

    Motif &GetMotif(uint32_t src, uint32_t dst);

    void CountTriangle(Motif &m, const std::vector<uint32_t> &neighbor1, const std::vector<uint32_t> &neighbor2,
                       const std::vector<label_type> &edge_label1, const std::vector<label_type> &edge_label2,
                       const std::vector<label_type> &neighbor_label1, const std::vector<label_type> &neighbor_label2);

    void CountNeighborDis(Motif &m, const std::vector<label_type> &edge_label,
                   const std::vector<label_type> &neighbor_label,std::pair<label_type,label_type> edge_pair,bool is_src);

protected:

    std::vector<std::vector<Motif>> edge_motif_;


};


#endif //MULTISUBGRAPHMATCHING_MGRAPH_H
