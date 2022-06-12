//
// Created by 10419 on 2022/6/12.
//

#ifndef MULTISUBGRAPHMATCHING_STREAMING_H
#define MULTISUBGRAPHMATCHING_STREAMING_H


#include <string>
#include <fstream>
#include <queue>
#include <util/type.h>

class Streaming {
public:
    struct StreamUint{
        bool is_insert;
        uint32_t src;
        label_type src_label;
        uint32_t dst;
        label_type dst_label;
        label_type edge_label;
    };
    void LoadStreamFromFile(std::string &stream_path );

    void Dump(std::string &stream_path);


    void Push(StreamUint stream_uint){
        streaming_.push(stream_uint);
    }

    bool Empty(){
        return streaming_.empty();
    }

    StreamUint Front(){
        return streaming_.front();
    }

    void Pop(){
        streaming_.pop();
    }

    size_t GetSize(){
        return streaming_.size();
    }
protected:
    std::queue<StreamUint> streaming_;

};


#endif //MULTISUBGRAPHMATCHING_STREAMING_H
