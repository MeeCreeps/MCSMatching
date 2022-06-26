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
#include<set>
#include<unordered_set>

enum class MFilter {
     Two_Hop = 0,Star = 1, False = 2
};

class Motif {
public:


    Motif(uint32_t v_label_size, uint32_t e_label_size) :
            v_label_(NON_EXIST), e_label_size_(e_label_size), v_label_size_(v_label_size) { Init(); };

    //Motif(label_type v_label, label_type dst_label) : v_label_(v_label), dst_label_(dst_label) {};

    Motif(label_type v_label, uint32_t v_label_size, uint32_t e_label_size) :
            v_label_(v_label), e_label_size_(e_label_size), v_label_size_(v_label_size) { Init(); };


    void Init() {
        n1_v_dis_.resize(v_label_size_, 0);
        n1_e_dis_.resize(e_label_size_, 0);
        n2_v_dis_.resize(v_label_size_, 0);
        n2_e_dis_.resize(e_label_size_, 0);
        dis_idx_.resize(4);
    }

    void Count2Hop(Motif &rhs) {
        for (auto i1: rhs.dis_idx_[0]) {
            this->dis_idx_[2].insert(i1);
            this->n2_v_dis_[i1] += rhs.n1_v_dis_[i1];
        }
        for (auto i2: rhs.dis_idx_[1]) {
            this->dis_idx_[3].insert(i2);
            this->n2_e_dis_[i2] += rhs.n1_e_dis_[i2];
        }
    }

    virtual MFilter IsSatisfied(const Motif &rhs) const {
        for (auto i: rhs.dis_idx_[0]) {
            if(this->n1_v_dis_[i]<rhs.n1_v_dis_[i])
                return MFilter::Star;
        }
        for (auto i: rhs.dis_idx_[1]) {
            if(this->n1_e_dis_[i]<rhs.n1_e_dis_[i])
                return MFilter::Star;
        }
        for (auto i: rhs.dis_idx_[2]) {
            if(this->n2_v_dis_[i]<rhs.n2_v_dis_[i])
                return MFilter::Two_Hop;
        }
        for (auto i: rhs.dis_idx_[3]) {
            if(this->n2_e_dis_[i]<rhs.n2_e_dis_[i])
                return MFilter::Two_Hop;
        }
        return MFilter::False;

    };


    void BuildN1VDis(const std::vector<uint32_t> &neighbors_label) {
        for (auto l: neighbors_label) {
            UpdateN1VDIS(l, 1);
        }
    }

    void BuildN1EDis(const std::vector<uint32_t> &edges_label) {
        for (auto l: edges_label) {
            UpdateN1EDIS(l, 1);
        }
    }

    void UpdateN1VDIS(label_type label, uint32_t num) {
        this->dis_idx_[0].insert(label);
        this->n1_v_dis_[label] += num;
    }

    void UpdateN1EDIS(label_type label, uint32_t num) {
        this->dis_idx_[1].insert(label);
        this->n1_e_dis_[label] += num;
    }

    void ResetN2Dis(){
        dis_idx_[2].clear();
        dis_idx_[3].clear();
        std::vector<uint32_t>(v_label_size_,0).swap(n2_v_dis_);
        std::vector<uint32_t>(e_label_size_,0).swap(n2_e_dis_);

    };
protected:
    uint32_t e_label_size_;
    uint32_t v_label_size_;
    // in undirected data graph , id(u)<id(v)
    label_type v_label_;

    std::vector<uint32_t> n1_v_dis_;
    std::vector<uint32_t> n1_e_dis_;
    std::vector<uint32_t> n2_v_dis_;
    std::vector<uint32_t> n2_e_dis_;
    std::vector<std::unordered_set<int>> dis_idx_;
};


#endif //MULTISUBGRAPHMATCHING_MOTIF_H
