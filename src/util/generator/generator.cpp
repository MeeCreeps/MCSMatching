//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#include "generator.h"



void Generator::StreamDump() {

    std::ofstream output(stream_path_);

    output << streaming_.size() << "\n";
    // format :  i/d  src  src_label  dst dst_label edge_label
    while (!streaming_.empty()) {
        StreamUint operate = streaming_.front();
        streaming_.pop();
        if (operate.is_insert)
            output << "i" << " ";
        else
            output << "d" << " ";
        output << operate.src << " " << operate.src_label << " " << operate.dst << " " << operate.dst_label << " "
               << operate.edge_label << "\n";
    }
    output.close();
}

void Generator::GenerateStreaming(int streaming_size) {

}

void Generator::GenerateQueries(int query_size, QueryLimit limit) {

}






