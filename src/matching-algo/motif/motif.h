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

    Motif(size_t edge_label_size) : label_nums_(edge_label_size), size_(edge_label_size * (edge_label_size + 1) / 2) {
        line_.resize(size_, 0);
        triangle_.resize(size_, 0);
        star_.resize(size_, 0);
    }

    struct Edge {
        uint32_t src;
        uint32_t dst;

        Edge() : src(UINT32_MAX), dst(UINT32_MAX) {}
    };


    virtual bool IsSatisfied(const Motif &rhs) const {
        for (int i = 0; i < size_; ++i) {
            if (line_[i] < rhs.line_[i])
                return false;
        }
        for (int i = 0; i < size_; ++i) {
            if (star_[i] < rhs.star_[i])
                return false;
        }
        for (int i = 0; i < size_; ++i) {
            if (triangle_[i] < rhs.star_[i])
                return false;
        }
        return true;
    };

    size_t GetMotifIdx(label_type l1, label_type l2) {
        if (l1 < l2)
            return label_nums_ * l1 - l1 * (l1 - 1) / 2 + l2 - l1;
        else
            return label_nums_ * l2 - l2 * (l2 - 1) / 2 + l1 - l2;

    }

    void AddStar(label_type l1, label_type l2,int num) {
        star_[GetMotifIdx(l1, l2)] += num;
    }

    void AddTriangle(label_type l1, label_type l2,int num) { triangle_[GetMotifIdx(l1, l2)] += num; }

    void AddLine(label_type l1, label_type l2,int num) { line_[GetMotifIdx(l1, l2)] += num; }

    //Edge GetEdge() { return edge_; };


protected:
    //Edge edge_;
    size_t size_;
    size_t label_nums_;
    std::vector<int> line_;
    std::vector<int> star_;
    std::vector<int> triangle_;

};


#endif //MULTISUBGRAPHMATCHING_MOTIF_H
