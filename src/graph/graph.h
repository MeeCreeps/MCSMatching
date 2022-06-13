//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef PROJECT_GRAPH_H
#define PROJECT_GRAPH_H

#include <string>
#include <vector>
#include <map>

#include <fstream>
#include <iostream>

#include "include/config.h"
#include "util/type.h"


class Graph {

public:
    Graph() : edge_nums_(0), vertex_nums_(0) {};

    void AddEdge(uint32_t src, uint32_t dst, label_type label);

    void RemoveEdge(uint32_t src, uint32_t dst);

    void AddVertex(uint32_t vertex, label_type label);

    void RemoveVertex(uint32_t vertex);

    uint32_t GetVertexNum() { return vertex_nums_; };

    uint32_t GetEdgeNum() { return edge_nums_; };

    uint32_t GetEdgeLabelNums() { return edge_label_type_nums_; }

    uint32_t GeVertexLabelNums() { return vertex_label_type_nums_; }

    uint32_t GetDegree(uint32_t vertex) {
        return neighbors_[vertex].size();
    }


    label_type GetVertexLabel(uint32_t vertex) { return vertex_label_[vertex]; }

    label_type GetEdgeLabel(uint32_t src, uint32_t dst) {
        return edge_label_[src][dst];
    };

    std::vector<uint32_t> GetNeighbors(uint32_t vertex) {
        return neighbors_[vertex];
    };

    std::vector<label_type> GetNeighborLabels(uint32_t vertex) {
        return std::vector<label_type >();
    };


    void LoadGraphByFile(std::string &graph_path);


    void Dump(std::string &graph_path);

    friend class Generator;
    friend class Analysis;

protected:
    std::vector<label_type> vertex_label_;
    // sorted by id
    std::vector<std::vector<uint32_t>> neighbors_;
    std::vector<std::vector<label_type>> edge_label_;
    uint32_t vertex_nums_;
    uint32_t edge_nums_;
    // the total number  of types
    uint32_t edge_label_type_nums_;
    uint32_t vertex_label_type_nums_;
    // the number of instance of each type
    std::map<label_type, size_t> each_edge_label_type_num_;
    std::map<label_type, size_t> each_vertex_label_type_num_;


};


#endif //PROJECT_GRAPH_H
