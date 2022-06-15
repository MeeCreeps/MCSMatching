
#ifndef MULTISUBGRAPHMATCHING_ANALYSIS_H
#define MULTISUBGRAPHMATCHING_ANALYSIS_H

#include "graph/graph.h"
#include "graph/streaming.h"


#include <vector>
#include <unordered_map>
#include "matching-algo/motif/motif_matching.h"

class Analysis {

public:
    Analysis(Graph &data_graph, std::vector<Graph> &query_graph, Streaming &streaming, std::string &report_path)
            : data_graph_(
            data_graph), query_graph_(query_graph), streaming_(streaming), report_path_(report_path) {

    }

    void Init();

    void Analyze();

protected:
    struct triple {
        label_type v1_label;
        label_type v2_label;
        label_type edge_label;

        bool operator==(const triple &rhs) const {
            return edge_label == rhs.edge_label && ((v1_label == rhs.v1_label && v2_label == rhs.v2_label) ||
                                                    (v1_label == rhs.v2_label && v2_label == rhs.v1_label));
        }
    };

    struct triple_hash {
        std::size_t operator()(const triple &t) const {
            return 11 * std::hash<label_type>()(t.v1_label) + 7 * std::hash<label_type>()(t.v2_label) +
                   5 * std::hash<label_type>()(t.edge_label);
        }
    };

    struct key_equal{

        bool operator()(const triple& lhs,const triple &rhs) const{
            return lhs==rhs;
        }

    };
    Graph &data_graph_;
    std::vector<Graph> &query_graph_;
    Streaming &streaming_;
    std::string report_path_;
    std::unordered_map<triple, std::vector<std::pair<uint32_t, uint32_t>>, triple_hash,key_equal> triple_to_graph_edge_;

};


#endif //MULTISUBGRAPHMATCHING_ANALYSIS_H
