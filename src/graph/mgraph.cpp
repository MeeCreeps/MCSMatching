//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#include "mgraph.h"


void Mgraph::RemoveEdge(uint32_t src, uint32_t dst) {
    Graph::RemoveEdge(src, dst);
}

void Mgraph::RemoveVertex(uint32_t vertex) {
    Graph::RemoveVertex(vertex);
}

void Mgraph::BuildN1Motif(uint32_t vertex) {
    Motif &m = GetMotif(vertex);
    const auto &edges_label = GetEdgeLabels(vertex);
    const auto &neighbors_label = GetNeighborLabels(vertex);
    m.BuildN1VDis(neighbors_label);
    m.BuildN1EDis(edges_label);

}

void Mgraph::BuildN2Motif(uint32_t vertex) {

    Motif &m= GetMotif(vertex);
    m.ResetN2Dis();
    for(uint32_t v2:neighbors_[vertex]){
        m.Count2Hop(GetMotif(v2));
    }
}


void Mgraph::UpdateMotif(uint32_t src, uint32_t dst, label_type label) {
    Motif &m1= GetMotif(src);
    m1.UpdateN1VDIS(vertex_label_[dst],1);
    m1.UpdateN1EDIS(label,1);
    Motif &m2= GetMotif(dst);
    m2.UpdateN1VDIS(vertex_label_[src],1);
    m2.UpdateN1EDIS(label,1);
}



void Mgraph::LoadGraphByFile(std::string &graph_path) {
    std::ifstream infile(graph_path);
    if (!infile.good()) {
        std::cout << "graph file doesn't exist ! " << std::endl;
        return;
    }

    char type;
    uint32_t vertex1, vertex2;
    label_type label;
    while (infile >> type) {
        if (type == 'v') {
            infile >> vertex1 >> label;
            AddVertex(vertex1, label);
        } else if (type == 'e') {
            infile >> vertex1 >> vertex2 >> label;
            AddEdge(vertex1, vertex2, label);

        }
    }
    // for sake of coding easily , needed to be changed later .
    motif_.resize(vertex_nums_, Motif(vertex_label_size_, edge_label_size_));
    infile.close();
}











