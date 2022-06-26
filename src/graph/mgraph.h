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

    Motif &GetMotif(uint32_t vertex) { return motif_[vertex]; };

    void BuildN1Motif(uint32_t vertex);

    void BuildN2Motif(uint32_t vertex);

    // edge update first
    void UpdateMotif(uint32_t src, uint32_t dst, label_type label);

    //const Motif &UpdateAndGetMotif(uint32_t src, uint32_t dst);

    //const Motif& UpdateStreaming(const Streaming::StreamUint & uint);


protected:

    std::vector<Motif> motif_;


};


#endif //MULTISUBGRAPHMATCHING_MGRAPH_H
