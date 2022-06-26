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
    BuildQueryMotif();
#ifdef LOGGING
    LOG(INFO) << " build query motif finished \n";
#endif
    //CountMaximalMotif();

    BuildDataMotif();


#ifdef LOGGING
    LOG(INFO) << " build data graph motif finished \n";
#endif

}

void Analysis::BuildDataMotif() {
    // build motif of  data graph
    for (int v1 = 0; v1 < data_graph_->vertex_nums_; ++v1) {

        LOG(INFO) << " Build query motif( 1-hop ) : " << v1 << "  neighbor size: " << data_graph_->neighbors_[v1].size()
                  << "\n";
        ((Mgraph *) data_graph_)->BuildN1Motif(v1);
#ifdef LOGGING
#endif
    }

    // build motif of two hop
//    for (int v1 = 0; v1 < data_graph_->vertex_nums_; ++v1) {
//
//        LOG(INFO) << " Build query motif( 2-hop ) : " << v1 << "  neighbor size: " << data_graph_->neighbors_[v1].size()
//                  << "\n";
//        ((Mgraph *) data_graph_)->BuildN2Motif(v1);
//    }
#ifdef LOGGING
#endif

}

void Analysis::BuildQueryMotif() {
    // build the index that mapping triple to query edge

    // build motif of 1-hop
    for (int i = 0; i < query_graph_.size(); ++i) {
        for (int v1 = 0; v1 < query_graph_[i]->vertex_nums_; ++v1) {
            // build motif of query graph
            ((Mgraph *) query_graph_[i])->BuildN1Motif(v1);
            for (int n = 0; n < query_graph_[i]->neighbors_[v1].size(); ++n) {
                uint32_t v2 = query_graph_[i]->neighbors_[v1][n];
                if (v1 > v2)
                    continue;
                // update index
                triple t(query_graph_[i]->vertex_label_[v1], query_graph_[i]->vertex_label_[v2],
                         query_graph_[i]->edge_label_[v1][n]);
                std::tuple<uint32_t, uint32_t, uint32_t> p{i, v1, v2};

                auto iter = triple_to_graph_edge_.find(t);
                if (iter != triple_to_graph_edge_.end())
                    iter->second.push_back(p);
                else
                    triple_to_graph_edge_.insert({t, {p}});
            }
        }
    }
    // build motif of 2-hop
    for (int i = 0; i < query_graph_.size(); ++i) {
        for (int v1 = 0; v1 < query_graph_[i]->vertex_nums_; ++v1) {
            ((Mgraph *) query_graph_[i])->BuildN2Motif(v1);
        }
    }

}

//void Analysis::CountMaximalMotif() {
//    for (int i = 0; i < query_graph_.size(); ++i) {
//        for (int v1 = 0; v1 < query_graph_[i]->vertex_nums_; ++v1) {
//            for (int n = 0; n < query_graph_[i]->neighbors_[v1].size(); ++n) {
//                uint32_t v2 = query_graph_[i]->neighbors_[v1][n];
//                if (v1 > v2)
//                    continue;
//                auto &src_motif = ((Mgraph *) query_graph_[i])->GetMotif(v1, v2).GetSrcTriple();
//                auto &dst_motif = ((Mgraph *) query_graph_[i])->GetMotif(v1, v2).GetSrcTriple();
//                auto &triangle_motif = ((Mgraph *) query_graph_[i])->GetMotif(v1, v2).GetTriangle();
//                for (auto &kv: src_motif) {
//                    neighbor_motif_.insert(kv.first);
//                }
//                for (auto &kv: dst_motif)
//                    neighbor_motif_.insert(kv.first);
//                for (auto &kv: triangle_motif)
//                    triangle_motif_.insert(kv.first);
//
//
//            }
//        }
//    }
//#ifdef LOGGING
//    LOG(INFO) << " maximal query motif  size\n";
//        LOG(INFO) << " neighbor motif size : "<<neighbor_motif_.size()<<"\n";
//        LOG(INFO) << " triangle motif size : "<<triangle_motif_.size()<<"\n";
//
//#endif
//
//
//}




MFilter Analysis::MotifTest(const Motif &src_m1, const Motif &dst_m1, const Motif &src_m2, const Motif &dst_m2) {

    // same edge label
    MFilter res1 = src_m1.IsSatisfied(src_m2);
    MFilter res2 = dst_m1.IsSatisfied(dst_m2);
    if (res1 == MFilter::False && res2 == MFilter::False)
        return res1;
    res1 = src_m1.IsSatisfied(dst_m2);
    res2 = dst_m1.IsSatisfied(src_m2);
    return std::min(res1, res2);
}


void Analysis::Analyze() {

    // how many edges are affected in each query graph by single stream uint
    uint32_t size = streaming_.GetSize();
    std::vector<std::vector<uint32_t>> mapped_edge_nums(size,
                                                        std::vector<uint32_t>(query_graph_.size(), 0));

    std::vector<uint32_t> affected_edge_nums(size, 0);
    std::vector<std::map<MFilter, int>> motif_filtered(size, {{MFilter::False,   0},
                                                              {MFilter::Star,    0},
                                                              {MFilter::Two_Hop, 0}});

    for (int i = 0; i < size; ++i) {
        auto uint = streaming_.Front();
        streaming_.Pop();
        if (i % 1000 == 0) {
            for (int j = 0; j < i; ++j) {
                LOG(INFO) << "streaming: " << j << " " << affected_edge_nums[j] << ","
                          << "Star:" << "," << motif_filtered[j][MFilter::Star] << ","
                          << "TWO HOP:" << "," << motif_filtered[j][MFilter::Two_Hop] << ","
                          << "False:" << "," << motif_filtered[j][MFilter::False] << "\n";
            }
        }

        // data motif
        if (uint.src > uint.dst)
            std::swap(uint.dst, uint.src);
        // must add edge first
        ((Mgraph *) data_graph_)->AddEdge(uint.src, uint.dst, uint.edge_label);
        ((Mgraph *) data_graph_)->UpdateMotif(uint.src, uint.dst, uint.edge_label);
        ((Mgraph *) data_graph_)->BuildN2Motif(uint.src);
        ((Mgraph *) data_graph_)->BuildN2Motif(uint.dst);
        const Motif &src_m1 = ((Mgraph *) data_graph_)->GetMotif(uint.src);
        const Motif &dst_m1 = ((Mgraph *) data_graph_)->GetMotif(uint.dst);


        uint32_t edge = 0;
        // query graph motif contains

        triple t(uint.src_label, uint.dst_label, uint.edge_label);
        auto iter = triple_to_graph_edge_.find(t);
        if (iter != triple_to_graph_edge_.end()) {
            // motif filter
            for (auto p: iter->second) {
                int graph_id = std::get<0>(p), src = std::get<1>(p), dst = std::get<2>(p);

                // get query motif
                const Motif &src_m2 = ((Mgraph *) query_graph_[graph_id])->GetMotif(src);
                const Motif &dst_m2 = ((Mgraph *) query_graph_[graph_id])->GetMotif(dst);
                motif_filtered[i][MotifTest(src_m1, dst_m1, src_m2, dst_m2)]++;

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
        std::cout << "streaming:" << i << "," << affected_edge_nums[i] << ","
                  << "Star:" << "," << motif_filtered[i][MFilter::Star] << ","
                  << "Two_Hop:" << "," << motif_filtered[i][MFilter::Two_Hop] << ","
                  << "False:" << "," << motif_filtered[i][MFilter::False] << "\n";
        output << i << "," << affected_edge_nums[i] << ","
               << "Star:" << "," << motif_filtered[i][MFilter::Star] << ","
               << "Two_Hop:" << "," << motif_filtered[i][MFilter::Two_Hop] << ","
               << "False:" << "," << motif_filtered[i][MFilter::False] << "\n";
    }
    output.close();
}






