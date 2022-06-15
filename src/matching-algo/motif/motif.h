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

    Motif(uint32_t edge_label_size){

    }

    struct Edge {
        uint32_t src,
        uint32_t  dst,
    };


    virtual bool IsSatisfied(const Motif &rhs) const{
        return true;
    };


    Edge GetEdge() { return edge_ };

protected:
    Edge edge_;

};


#endif //MULTISUBGRAPHMATCHING_MOTIF_H
