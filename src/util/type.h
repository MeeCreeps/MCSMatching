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

#define NON_EXIST UINT32_MAX
struct StreamUint{
    bool is_insert;
    uint32_t src;
    label_type src_label;
    uint32_t dst;
    label_type dst_label;
    label_type edge_label;
};


struct QueryLimit{
    int max_edge_nums;
    int min_edge_nums;

};






#endif //MULTISUBGRAPHMATCHING_TYPE_H
