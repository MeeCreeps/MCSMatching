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

enum class MFilter {
    Triangle = 0, Star = 1, False = 3
};

class Motif {
public:
    Motif() : src_label_(NON_EXIST), dst_label_(NON_EXIST) {};

    Motif(label_type src_label, label_type dst_label) : src_label_(src_label), dst_label_(dst_label) {};


    virtual MFilter IsSatisfied(const Motif &rhs) const {

        // UNDIRECTED and without vertex label
        MFilter res = IsSatisfiedOneDire(rhs, false);
#ifdef UNDIRECTED
        if (res == MFilter::False)
            return res;
        else if (rhs.dst_label_ == src_label_ && rhs.src_label_ == dst_label_)
            return IsSatisfiedOneDire(rhs, true);

#endif
        return res;
    };


    virtual MFilter IsSatisfiedOneDire(const Motif &rhs, bool is_reverse) const {
        // Triangle
        if (!is_reverse) {
            for (auto &t: rhs.triangle_) {
                auto iter = triangle_.find(t.first);
                if (iter == triangle_.end() || t.second > iter->second) {
                    return MFilter::Triangle;
                }
            }
            // src neighbor
            for (auto &key: rhs.src_triple_) {
                auto iter = src_triple_.find(key.first);
                if (iter == src_triple_.end() || key.second > iter->second)
                    return MFilter::Star;
            }
            for (auto &key: rhs.dst_triple_) {
                auto iter = dst_triple_.find(key.first);
                if (iter == dst_triple_.end() || key.second > iter->second)
                    return MFilter::Star;
            }
        } else {
            for (auto &t: rhs.triangle_) {
                auto iter = triangle_.find({std::get<2>(t.first), std::get<1>(t.first), std::get<0>(t.first)});
                if (iter == triangle_.end() || t.second > iter->second) {
                    return MFilter::Triangle;
                }
            }
            // u neighbor
            for (auto &key: rhs.src_triple_) {
                auto iter = dst_triple_.find(key.first);
                if (iter == dst_triple_.end() || key.second > iter->second)
                    return MFilter::Star;
            }
            for (auto &key: rhs.dst_triple_) {
                auto iter = src_triple_.find(key.first);
                if (iter == src_triple_.end() || key.second > iter->second)
                    return MFilter::Star;
            }
        }
        return MFilter::False;
    }

    void SetLables(label_type src_label, label_type dst_label) {
        src_label_ = src_label;
        dst_label_ = dst_label;
    };

    void UpdateSrc(label_type vl, label_type el, int num) {
        src_triple_[{vl, el}] += num;
        if (num < 0 && src_triple_[{vl, el}] == 0) {
            src_triple_.erase({vl, el});
        }
    }

    void UpdateDst(label_type vl, label_type el, int num) {
        dst_triple_[{vl, el}] += num;
        if (num < 0 && dst_triple_[{vl, el}] == 0) {
            dst_triple_.erase({vl, el});
        }
    }

    void UpdateTriangle(label_type edge_l1, label_type edge_l2, label_type vl, int num) {
        triangle_[{edge_l1, edge_l2, vl}] += num;
    }

    std::unordered_map<std::pair<label_type, label_type>, int, pair_hash> &GetSrcTriple() {
        return src_triple_;
    }

    std::unordered_map<std::pair<label_type, label_type>, int, pair_hash> &GetDstTriple() {
        return dst_triple_;
    }

protected:

    // in undirected data graph , id(u)<id(v)
    label_type src_label_;
    label_type dst_label_;

    std::unordered_map<std::pair<label_type, label_type>, int, pair_hash> src_triple_;
    std::unordered_map<std::pair<label_type, label_type>, int, pair_hash> dst_triple_;
    // <E(u,x),x,E(v,x)>
    std::unordered_map<std::tuple<label_type, label_type, label_type>, int, tuple_hash> triangle_;


};


#endif //MULTISUBGRAPHMATCHING_MOTIF_H
