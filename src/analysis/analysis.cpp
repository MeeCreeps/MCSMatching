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
//                Motif m(query_graph_[i]->edge_label_size_);
//                ((Mgraph *) query_graph_[i])->BuildMotif(m, v1, v2);
            }
        }
    }

    // build data graph
    for (int v1 = 0; v1 < data_graph_->vertex_nums_; ++v1) {
        for (int n = 0; n < data_graph_->neighbors_[v1].size(); ++n) {
            uint32_t v2 = data_graph_->neighbors_[v1][n];
            if (v1 > v2)
                continue;
            Motif m(data_graph_->edge_label_size_);

            ((Mgraph *) data_graph_)->BuildMotif(m, v1, v2);
            ((Mgraph *) data_graph_)->SetMotif(m,v1,n);
        }
    }
}


void Analysis::Analyze() {

    // how many edges are affected in each query graph by single stream uint
    uint32_t size = streaming_.GetSize();
    std::vector<std::vector<uint32_t>> mapped_edge_nums(size,
                                                        std::vector<uint32_t>(query_graph_.size(), 0));

    std::vector<uint32_t> affected_edge_nums(size, 0);

    std::vector<uint32_t> motif_filtered(size, 0);
    for (int i = 0; i < size; ++i) {
        auto uint = streaming_.Front();
        streaming_.Pop();
        std::cout<<"streaming :"<<i<<std::endl;
        ((Mgraph *)data_graph_)->AddEdge(uint.src, uint.dst, uint.edge_label);
        // data motif
        const Motif &m = ((Mgraph *) data_graph_)->GetMotif(uint.src, uint.dst);

        triple t(uint.src_label, uint.dst_label, uint.edge_label);
        auto iter = triple_to_graph_edge_.find(t);
        if (iter != triple_to_graph_edge_.end()) {
            // motif filter
            for (auto p: iter->second) {
                int graph_id = std::get<0>(p), src = std::get<1>(p), dst = std::get<2>(p);
                // get query motif
                Motif m2(query_graph_[graph_id]->GetEdgeLabelNums());
                ((Mgraph *) query_graph_[graph_id])->BuildMotif(m2,src,dst);

                if (!m.IsSatisfied(((Mgraph *) query_graph_[graph_id])->GetMotif(src, dst)))
                    motif_filtered[i]++;
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
        output << i << "," << affected_edge_nums[i] << "," << motif_filtered[i] << "\n";
    }
//    output<<"streaming detail"<<"\n";
//    for(int i=0;i<mapped_edge_nums.size();++i){
//
//
//
//    }


    output.close();
}


