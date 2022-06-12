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
    for(int i=0;i<query_graph_.size();++i){
        for(int j=0;j<query_graph_[i].vertex_nums_;++j){
            for(auto n=0;n<query_graph_[i].neighbors_[j].size();++n){
                if(j>query_graph_[i].neighbors_[j][i])
                    continue;
                

            }
        }
    }
}


void Analysis::Analyze() {

    for (int i = 0; i < streaming_.GetSize(); ++i) {
        auto uint = streaming_.Front();
        streaming_.Pop();
        for(int j=0;j<query_graph_.size();++j){




        }



    }


}


