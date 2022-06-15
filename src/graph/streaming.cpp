//===----------------------------------------------------------------------===//
//
//                         
//
//     
//
// 
//
//===----------------------------------------------------------------------===//

#include "streaming.h"


void Streaming::Dump(std::string &stream_path) {
    std::ofstream output(stream_path);


    // format :  i/d  src  src_label  dst dst_label edge_label
    while (!streaming_.empty()) {
        Streaming::StreamUint operate = streaming_.front();
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

void Streaming::LoadStreamFromFile(std::string &stream_path) {
    std::ifstream infile(stream_path);

    if (!infile.good())
        return;

    char type;
    while (infile >> type) {
        StreamUint uint;
        if (type == 'i')
            uint.is_insert = true;
        else if (type == 'd')
            uint.is_insert = false;

        infile >> uint.src >> uint.src_label >> uint.dst >> uint.dst_label >> uint.edge_label;
        streaming_.push(uint);
    }
    infile.close();

}

