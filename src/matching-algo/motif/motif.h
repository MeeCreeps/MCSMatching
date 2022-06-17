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


class Motif {
public:
    Motif() = default;

    Motif(size_t edge_label_size) : label_nums_(edge_label_size) {
        for(size_t i=0;i<edge_label_size;++i){
            line_.push_back(std::vector<int>(edge_label_size-i,0));
            star_.push_back(std::vector<int>(edge_label_size-i,0));
            triangle_.push_back(std::vector<int>(edge_label_size-i,0));
        }
    }

    struct Edge {
        uint32_t src;
        uint32_t dst;

        Edge() : src(UINT32_MAX), dst(UINT32_MAX) {}
    };


    virtual bool IsSatisfied(const Motif &rhs) const {
        for (int i = 0; i < rhs.line_.size(); ++i) {
            for (int j = 0; j < rhs.line_[i].size(); ++j) {
                if (line_[i][j] < rhs.line_[i][j] || star_[i][j] < rhs.star_[i][j] ||
                    triangle_[i][j] < rhs.triangle_[i][j])
                    return false;
            }
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
        star_[l1][l2 - l1] += num;
    }

    void AddTriangle(label_type l1, label_type l2, int num) {

        triangle_[l1][l2 - l1] += num;
    }

    // l1 < l2
    void AddLine(label_type l1, label_type l2, int num) {
        line_[l1][l2 - l1] += num;
    }

    //Edge GetEdge() { return edge_; };


protected:
    //Edge edge_;
    size_t label_nums_;
    // 4 types , we need to travasal 2 step to build fourth type , inefficient
    std::vector<std::vector<int>> line_;
    std::vector<std::vector<int>> star_;
    std::vector<std::vector<int>> triangle_;

};


#endif //MULTISUBGRAPHMATCHING_MOTIF_H
