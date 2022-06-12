
#ifndef MULTISUBGRAPHMATCHING_ANALYSIS_H
#define MULTISUBGRAPHMATCHING_ANALYSIS_H

#include "graph/graph.h"
#include "graph/streaming.h"


#include <vector>
#include <unordered_map>


class Analysis {

public:
    Analysis(Graph &data_graph, std::vector<Graph> &query_graph, Streaming &streaming, std::string &report_path)
            : data_graph_(
            data_graph), query_graph_(query_graph), streaming_(streaming), report_path_(report_path) {

    }

    void Analyze();

protected:
    Graph &data_graph_;
    std::vector<Graph> &query_graph_;
    Streaming &streaming_;
    std::string report_path_;


};


#endif //MULTISUBGRAPHMATCHING_ANALYSIS_H
