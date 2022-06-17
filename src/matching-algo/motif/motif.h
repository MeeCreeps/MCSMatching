//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef MULTISUBGRAPHMATCHING_MOTIF_H
#define MULTISUBGRAPHMATCHING_MOTIF_H

#include <unordered_map>

class Motif {
public:
    Motif() = default;

    Motif(size_t edge_label_size) : label_nums_(edge_label_size) {
    }

    struct Edge {
        uint32_t src;
        uint32_t dst;

        Edge() : src(UINT32_MAX), dst(UINT32_MAX) {}
    };


    virtual bool IsSatisfied(const Motif &rhs) const {
        for(auto &key:rhs.triangle_){
            auto iter = triangle_.find(key.first);
            if(iter==triangle_.end()|| key.second>iter->second)
                return false;
        }
        for(auto &key:rhs.line_){
            auto iter = line_.find(key.first);
            if(iter==line_.end()|| key.second>iter->second)
                return false;
        }
        for(auto &key:rhs.star_){
            auto iter = star_.find(key.first);
            if(iter==star_.end()|| key.second>iter->second)
                return false;
        }

        return true;

    };

    // expired
    size_t GetMotifIdx(label_type l1, label_type l2) {
        if (l1 < l2)
            return label_nums_ * l1 - l1 * (l1 - 1) / 2 + l2 - l1;
        else
            return label_nums_ * l2 - l2 * (l2 - 1) / 2 + l1 - l2;

    }

    void AddStar(label_type l1, label_type l2, int num) {
        std::pair<label_type,label_type > pair{l1,l2};
        auto iter = star_.find(pair);
        if(iter!=star_.end())
            star_[pair]+=num;
        else
            star_.insert({pair,num});
    }

    void AddTriangle(label_type l1, label_type l2, int num) {
        std::pair<label_type,label_type > pair{l1,l2};
        auto iter = triangle_.find(pair);
        if(iter!=triangle_.end())
            triangle_[pair]+=num;
        else
            triangle_.insert({pair,num});
    }

    // l1 < l2
    void AddLine(label_type l1, label_type l2, int num) {
        std::pair<label_type,label_type > pair{l1,l2};
        auto iter = line_.find(pair);
        if(iter!=line_.end())
            line_[pair]+=num;
        else
            line_.insert({pair,num});
    }

    //Edge GetEdge() { return edge_; };


protected:
    //Edge edge_;

    size_t label_nums_;
    // 4 types , we need to travasal 2 step to build fourth type , inefficient
    std::unordered_map<std::pair<label_type,label_type>,int,pair_hash> line_;
    std::unordered_map<std::pair<label_type,label_type>,int,pair_hash> star_;
    std::unordered_map<std::pair<label_type,label_type>,int,pair_hash> triangle_;


};


#endif //MULTISUBGRAPHMATCHING_MOTIF_H
