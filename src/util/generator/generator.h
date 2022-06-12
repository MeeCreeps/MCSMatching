//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef MULTISUBGRAPHMATCHING_SHUFFLE_H
#define MULTISUBGRAPHMATCHING_SHUFFLE_H

#include "graph/graph.h"
#include "graph/streaming.h"

#include <queue>
#include <random>
#include <time.h>
#include <algorithm>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
class Generator {


public:
    Generator(std::string graph_path_before, std::string data_graph_path, std::string stream_path,
              std::string query_path_prefix) : graph_path_before_(graph_path_before), data_graph_path_(data_graph_path),
                                               stream_path_(stream_path),
                                               query_path_prefix_(query_path_prefix) {
        data_graph_.LoadGraphByFile(graph_path_before_);
    };




    virtual void ShuffleDataGraph() {};


    virtual void GenerateStreaming(int streaming_size);

    virtual void GenerateQueries(int query_nums, QueryLimit limit);





protected:
    Graph data_graph_;
    Streaming stream_;
    // the origin data graph
    std::string graph_path_before_;
    // divide the origin data graph into streaming and data graph
    std::string data_graph_path_;
    std::string stream_path_;
    std::string query_path_prefix_;


};


#endif //MULTISUBGRAPHMATCHING_SHUFFLE_H
