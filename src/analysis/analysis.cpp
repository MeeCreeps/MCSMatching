//===----------------------------------------------------------------------===//
//
//                         
//
//     
//
// 
//
//===----------------------------------------------------------------------===//

#include "analysis.h"

void Analysis::Init() {
    // build the index that mapping triple to query edge
    uint32_t edge_idx = 0;
    for (int i = 0; i < query_graph_.size(); ++i) {
        for (int v1 = 0; v1 < query_graph_[i]->vertex_nums_; ++v1) {
            for (int n = 0; n < query_graph_[i]->neighbors_[v1].size(); ++n) {
                uint32_t v2 = query_graph_[i]->neighbors_[v1][n];
                if (v1 > v2)
                    continue;
                // update index
                triple t(query_graph_[i]->vertex_label_[v1], query_graph_[i]->vertex_label_[v2],
                         query_graph_[i]->edge_label_[v1][n]);
                std::tuple<uint32_t, uint32_t, uint32_t> p{i, v1, v2};
                ++edge_idx;
                auto iter = triple_to_graph_edge_.find(t);
                if (iter != triple_to_graph_edge_.end())
                    iter->second.push_back(p);
                else
                    triple_to_graph_edge_.insert({t, {p}});

                // build motif of query graph
                ((Mgraph *) query_graph_[i])->BuildMotif(v1, v2, query_graph_[i]->edge_label_[v1][n]);
            }
        }
    }

    // build motif of  data graph
    for (int v1 = 0; v1 < data_graph_->vertex_nums_; ++v1) {
        for (int n = 0; n < data_graph_->neighbors_[v1].size(); ++n) {
            uint32_t v2 = data_graph_->neighbors_[v1][n];
            if (v1 > v2)
                continue;
            ((Mgraph *) data_graph_)->BuildMotif(v1, v2, data_graph_->edge_label_[v1][n]);
        }
    }
}


void Analysis::Analyze() {

    // how many edges are affected in each query graph by single stream uint
    uint32_t size = streaming_.GetSize();
    std::vector<std::vector<uint32_t>> mapped_edge_nums(size,
                                                        std::vector<uint32_t>(query_graph_.size(), 0));

    std::vector<uint32_t> affected_edge_nums(size, 0);
    std::vector<std::map<MFilter, int>> motif_filtered(size, {{MFilter::False,    0},
                                                              {MFilter::Star,     0},
                                                              {MFilter::Triangle, 0}});

    for (int i = 0; i < size; ++i) {
        auto uint = streaming_.Front();
        streaming_.Pop();

        // data motif
        if(uint.src>uint.dst)
            std::swap(uint.dst,uint.src);
        // must add edge first
        ((Mgraph *) data_graph_)->AddEdge(uint.src, uint.dst, uint.edge_label);
        const Motif &m1 = ((Mgraph *) data_graph_)->BuildMotif(uint.src, uint.dst, uint.edge_label);


        // query graph motif contains

        triple t(uint.src_label, uint.dst_label, uint.edge_label);
        auto iter = triple_to_graph_edge_.find(t);
        if (iter != triple_to_graph_edge_.end()) {
            // motif filter
            for (auto p: iter->second) {
                int graph_id = std::get<0>(p), src = std::get<1>(p), dst = std::get<2>(p);

                // get query motif

                const Motif &m2 = ((Mgraph *) query_graph_[graph_id])->GetMotif(src, dst);
                motif_filtered[i][m1.IsSatisfied(((Mgraph *) query_graph_[graph_id])->GetMotif(src, dst))]++;
            }
            // edge mapping to graph
            affected_edge_nums[i] = iter->second.size();
            for (auto &p: iter->second) {
                mapped_edge_nums[i][std::get<0>(p)] += 1;
            }
        }
    }
    std::ofstream output(report_path_);
    output << "streaming " << "," << "edge nums" << "\n";
    for (int i = 0; i < affected_edge_nums.size(); ++i) {
        std::cout << "streaming:" << i << "," << affected_edge_nums[i] << "," << "triangle:" << ","
                  << motif_filtered[i][MFilter::Triangle] << "," << "Star:" << "," << motif_filtered[i][MFilter::Star]
                  << "," << "False:" << "," << motif_filtered[i][MFilter::False] << "\n";
        output << i << "," << affected_edge_nums[i] << "," << "triangle:" << ","
               << motif_filtered[i][MFilter::Triangle] << "," << "Star:" << "," << motif_filtered[i][MFilter::Star]
               << "," << "False:" << "," << motif_filtered[i][MFilter::False] << "\n";
    }
    output.close();
}


