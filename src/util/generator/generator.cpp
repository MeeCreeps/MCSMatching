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




void Generator::GenerateStreaming(int streaming_size) {

    // generate streaming by removing edges randomly

    srand((unsigned) time(NULL));
    // avoid  divide the graph into multi graph
    std::vector<uint32_t> degree(data_graph_.neighbors_.size(), 0);
    for (int i = 0; i < data_graph_.neighbors_.size(); ++i)
        degree[i] = data_graph_.neighbors_[i].size();
    uint32_t max_id = data_graph_.vertex_label_.size();
    while (stream_.GetSize() < streaming_size) {
        uint32_t vertex1 = rand() % max_id;
        if (degree[vertex1] <= 1)
            continue;
        uint32_t vertex2 = data_graph_.neighbors_[vertex1][rand() % data_graph_.neighbors_[vertex1].size()];
        if (data_graph_.neighbors_[vertex1][vertex2] == NON_EXIST)
            continue;

        degree[vertex1] -= 1;
        degree[vertex2] -= 1;
        data_graph_.neighbors_[vertex1][vertex2] = NON_EXIST;
        data_graph_.neighbors_[vertex2][vertex1] = NON_EXIST;
        stream_.Push(Streaming::StreamUint{true, vertex1, data_graph_.vertex_label_[vertex1], vertex2,
                                           data_graph_.vertex_label_[vertex2],
                                           data_graph_.edge_label_[vertex1][vertex2]});
    }

    // rewrite graph
    data_graph_.Dump(data_graph_path_);

    // write stream
    stream_.Dump(stream_path_);


}

void Generator::GenerateQueries(int query_nums, QueryLimit limit) {


    // sort by avg degree of query graph
    struct q_degree {
        int query_id;
        float degree;

        bool operator<(const q_degree &rhs) const {
            return this->degree > rhs.degree;
        }
    };
    std::vector<q_degree> query_degree;
    std::vector<Graph> queries;

    // generator queries by random walk
    srand((unsigned) time(NULL));
    while (queries.size() < query_nums * QUERY_GENERATOR_RATE) {
        Graph query_graph;
        std::unordered_map<uint32_t, uint32_t> data_id_to_query_id;
        int edges_num = rand() % limit.max_edge_nums + limit.min_edge_nums;


        uint32_t query_v1 = 0, query_v2, vertex2, vertex_size = 0, j = 0;
        uint32_t vertex1 = rand() % data_graph_.vertex_nums_;
        // insert vertex1
        data_id_to_query_id.insert({vertex1, query_v1});
        query_graph.AddVertex(vertex1, data_graph_.vertex_label_[vertex1]);

        while (j < edges_num) {
            // visit neighbor
            vertex2 = data_graph_.neighbors_[vertex1][rand() % data_graph_.neighbors_[vertex1].size()];
            // return if  leaf edge
            if (data_graph_.neighbors_[vertex2].size() <= 1)
                break;
            // check whether vertex is exist or not
            if (data_id_to_query_id.find(vertex2) == data_id_to_query_id.end()) {
                query_v2 = vertex_size++;
                data_id_to_query_id.insert({vertex2, query_v2});
                query_graph.AddVertex(vertex2, data_graph_.vertex_label_[vertex2]);
            } else
                query_v2 = data_id_to_query_id[vertex2];
            // check whether edge is already exist
            bool edge_not_exist = true;
            for (auto &v: query_graph.neighbors_[query_v1]) {
                if (v == query_v2) {
                    edge_not_exist = false;
                    break;
                }
            }
            if (edge_not_exist) {
                query_graph.AddEdge(query_v1, query_v2, data_graph_.edge_label_[vertex1][vertex2]);
                ++j;
            }

            vertex1 = vertex2;
            query_v1 = query_v2;
        }
        if (j == edges_num) {
            query_degree.push_back({(int) queries.size(), (float) edges_num / vertex_size});
            queries.push_back(query_graph);
        }

    }
    std::sort(query_degree.begin(), query_degree.end());

    for (int i = 0; i < query_nums; ++i) {
        std::string query_path = query_path_prefix_ + "_" + std::to_string(i);
        queries[i].Dump(query_path);
    }
}








