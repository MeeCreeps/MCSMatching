

#ifndef MCSMATCHING_MATCHING_H
#define MCSMATCHING_MATCHING_H

#include "graph/graph.h"
#include <queue>
#include <vector>

class Matching {

public:
    Matching(Graph &data_graph, std::vector<Graph> &query_graph, std::queue<StreamUint> &streaming) : data_graph_(
            data_graph), query_graph_(query_graph), streaming_(streaming) {

    }


    virtual void Init() {}

    virtual void InitialMatching() {}

    virtual void ProcessStreaming() {}


protected:
    Graph &data_graph_;
    std::vector<Graph> &query_graph_;
    std::queue<StreamUint> &streaming_;

};


#endif //MCSMATCHING_MATCHING_H
