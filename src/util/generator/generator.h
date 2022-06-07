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
#include <queue>

class Generator {


public:
    Generator( std::string graph_path, std::string stream_path,
              std::string query_path_prefix) : graph_path_(graph_path), stream_path_(stream_path),
                                               query_path_prefix_(query_path_prefix) {};

    virtual void ShuffleDataGraph() {};


    virtual void GenerateStreaming(int streaming_size);

    virtual void GenerateQueries(int query_size, QueryLimit limit);


    void StreamDump();


protected:
    Graph data_graph_;
    std::string graph_path_;
    std::string stream_path_;
    std::string query_path_prefix_;
    std::queue<StreamUint> streaming_;

};


#endif //MULTISUBGRAPHMATCHING_SHUFFLE_H
