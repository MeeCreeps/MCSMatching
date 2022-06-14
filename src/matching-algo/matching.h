

#ifndef MCSMATCHING_MATCHING_H
#define MCSMATCHING_MATCHING_H

#include "graph/graph.h"
#include "graph/streaming.h"

#include <unordered_map>
#include <vector>


class Matching {

public:
    Matching(Graph &data_graph, std::vector<Graph> &query_graph, Streaming &streaming) : data_graph_(
            data_graph), query_graph_(query_graph), streaming_(streaming) {

    }


    virtual void Init();

    virtual void InitialMatching() {}

    virtual void ProcessStreaming() {}


protected:

    Graph &data_graph_;
    std::vector<Graph> &query_graph_;
    Streaming &streaming_;


};


#endif //MCSMATCHING_MATCHING_H
