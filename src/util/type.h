//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#ifndef MULTISUBGRAPHMATCHING_TYPE_H
#define MULTISUBGRAPHMATCHING_TYPE_H

typedef uint32_t label_type;



struct StreamUint{
    bool is_insert;
    uint32_t src;
    label_type src_label;
    uint32_t dst;
    label_type dst_label;
    label_type edge_label;
};






#endif //MULTISUBGRAPHMATCHING_TYPE_H
