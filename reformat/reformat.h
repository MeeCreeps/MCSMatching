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


class Reformat {
public:

    Reformat(std::string original_path, std::string graph_path) : original_path_(original_path),
                                                                  graph_path_(graph_path) {};


    virtual void ShuffleDataGraph();


protected:
    std::string original_path_;
    std::string graph_path_;
    Graph data_graph_;


};


#endif //MULTISUBGRAPHMATCHING_SHUFFLE_H
