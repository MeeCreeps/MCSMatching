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
        for (int v1 = 0; v1 < query_graph_[i].vertex_nums_; ++v1) {
            for (int n = 0; n < query_graph_[i].neighbors_[v1].size(); ++n) {
                uint32_t v2 = query_graph_[i].neighbors_[v1][n];
                if (v1 > v2)
                    continue;
                triple t{query_graph_[i].vertex_label_[v1], query_graph_[i].vertex_label_[v2],
                         query_graph_[i].edge_label_[v1][n]};


                std::pair<uint32_t, uint32_t> p{i, edge_idx};
                ++edge_idx;
                auto iter = triple_to_graph_edge_.find(t);
                if (iter != triple_to_graph_edge_.end())
                    iter->second.push_back(p);
                else
                    triple_to_graph_edge_.insert({t, {p}});
            }
        }
    }
}


void Analysis::Analyze() {

    // how many edges are affected in each query graph by single stream uint
    uint32_t size=streaming_.GetSize();
    std::vector<std::vector<uint32_t>> mapped_edge_nums(size,
                                                        std::vector<uint32_t>(query_graph_.size(), 0));

    std::vector<uint32_t> affected_edge_nums(size,0);
    for (int i = 0; i < size; ++i) {
        auto uint = streaming_.Front();
        streaming_.Pop();
        triple t{uint.src_label,uint.dst_label,uint.edge_label};
        auto iter = triple_to_graph_edge_.find(t);

        if(iter!=triple_to_graph_edge_.end()){
            affected_edge_nums[i]=iter->second.size();
            for(auto &p:iter->second){
                mapped_edge_nums[i][p.first]+=1;
            }
        }
    }

    std::ofstream output(report_path_);
    output<<"streaming "<<","<<"edge nums"<<"\n";
    for(int i=0;i<affected_edge_nums.size();++i){
        output<<i<<","<<affected_edge_nums[i]<<"\n";
    }
//    output<<"streaming detail"<<"\n";
//    for(int i=0;i<mapped_edge_nums.size();++i){
//
//
//
//    }


    output.close();
}


