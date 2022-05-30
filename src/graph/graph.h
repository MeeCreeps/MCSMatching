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
#include "include/config.h"

class Graph {

public:
    Graph();

    void AddEdge(vertex_type src, vertex_type dst, label_type label);

    void RemoveEdge(vertex_type src, vertex_type dst);

    void AddVertex(vertex_type vertex, label_type label);

    void Remove(vertex_type vertex);

    uint32_t GetVertexNum() { return vertex_nums_; };

    uint32_t GetEdgeNum() { return edge_nums_; };

    uint32_t GetEdgeLabelNums() { return edge_label_type_nums_; }

    uint32_t GeVertexLabelNums() { return vertex_label_type_nums_; }

    label_type GetVertexLabel(vertex_type vertex);

    label_type GetEdgeLabel(vertex_type src,vertex_type dst);

    std::vector<vertex_type> GetNeighbors(vertex_type vertex);

    std::vector<label_type> GetNeighborLabels(vertex_type vertex);

    

    void LoadGraphByFile(std::string &graph_path);

    void LoadStreamByFile(std::string &stream_path);


protected:
    std::vector<label_type> vertex_label_;
    std::vector<std::vector<vertex_type>> neighbors_;
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
