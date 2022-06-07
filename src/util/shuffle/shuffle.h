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

class Shuffle {


public:
    Shuffle(std::string graph_path) : graph_path_(graph_path) {}

    virtual void BuildGraph() {};


    void GraphDump() {

        std::ofstream output(graph_path_);

        // v: vertex  e: edge(dst,src,type)
        //first line   vertex nums   edge nums
        output << data_graph_.vertex_nums_ << " " << data_graph_.edge_nums_ << "\n";
        // second line  the size of vertex label , the size of edge label
        output << data_graph_.vertex_label_type_nums_ << " " << data_graph_.edge_label_type_nums_ << "\n";
        for (uint32_t vertex = 0; vertex < data_graph_.vertex_label_.size(); ++vertex) {
            output << "v" << " " << vertex << " " << data_graph_.vertex_label_[vertex] << "\n";
        }
        for (uint32_t src = 0; src < data_graph_.neighbors_.size(); ++src) {
            for (uint32_t idx = 0; idx < data_graph_.neighbors_[src].size(); ++idx) {
                output << "e" << src << " " << data_graph_.neighbors_[src][idx] << " "
                       << data_graph_.edge_label_[src][idx];
            }
        }
        output.close();

    };


    void BuildStream();

    void StreamDump() {

        std::ofstream output(stream_path_);

        output << streaming_.size() << "\n";
        // format :  i/d  src  src_label  dst dst_label edge_label
        while (!streaming_.empty()) {
            StreamUint operate = streaming_.front();
            streaming_.pop();
            if (operate.is_insert)
                output << "i" << " ";
            else
                output << "d" << " ";
            output << operate.src << " " << operate.src_label << " " << operate.dst << " " << operate.dst_label << " "
                   << operate.edge_label << "\n";
        }
        output.close();
    }

protected:
    Graph data_graph_;
    std::string graph_path_;
    std::string stream_path_;
    std::queue<StreamUint> streaming_;
};


#endif //MULTISUBGRAPHMATCHING_SHUFFLE_H
