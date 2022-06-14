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
        int idx = rand() % data_graph_.neighbors_[vertex1].size();
        uint32_t vertex2 = data_graph_.neighbors_[vertex1][idx];
        if (data_graph_.neighbors_[vertex1][idx] == NON_EXIST)
            continue;

        degree[vertex1] -= 1;
        degree[vertex2] -= 1;
        data_graph_.neighbors_[vertex1][idx] = NON_EXIST;
        for (auto &v:data_graph_.neighbors_[vertex2]) {
            if (v == vertex1)
                v = NON_EXIST;
        }
        stream_.Push(Streaming::StreamUint{true, vertex1, data_graph_.vertex_label_[vertex1], vertex2,
                                           data_graph_.vertex_label_[vertex2],
                                           data_graph_.edge_label_[vertex1][idx]});
    }

    // rewrite graph
    data_graph_.Dump(data_graph_path_);

    // write stream
    stream_.Dump(stream_path_);


}

void Generator::GenerateQueries(int query_nums, QueryLimit limit) {


    // sort by avg degree of query graph
    struct q_degree {
        size_t query_id;
        int edge_nums;
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
        std::vector<uint32_t> vertices;
        int edges_num = rand() % (limit.max_edge_nums - limit.min_edge_nums) + limit.min_edge_nums;


        uint32_t query_v1 = 0, query_v2, vertex2, vertex1, vertex_size = 1;

        vertex1 = rand() % data_graph_.vertex_nums_;
        while (data_graph_.neighbors_[vertex1].size() == 0) {
            vertex1 = rand() % data_graph_.vertex_nums_;

        }
        vertices.push_back(vertex1);
        // insert vertex1
        data_id_to_query_id.insert({vertex1, query_v1});
        query_graph.AddVertex(query_v1, data_graph_.vertex_label_[vertex1]);

        int delay = 0, edges_before = 0;
        while (query_graph.edge_nums_ < edges_num && delay < 3) {

            vertex1 = vertices[rand() % vertices.size()];
            query_v1 = data_id_to_query_id[vertex1];
            // visit neighbor
            int v_idx = rand() % data_graph_.neighbors_[vertex1].size();
            vertex2 = data_graph_.neighbors_[vertex1][v_idx];
            if (vertex1 == vertex2) {
                delay++;
                continue;
            }

            if (vertex2 == UINT32_MAX || data_graph_.neighbors_[vertex2].size() == 0)
                continue;
            // check whether vertex is exist or not
            if (data_id_to_query_id.find(vertex2) == data_id_to_query_id.end()) {
                query_v2 = vertex_size++;
                data_id_to_query_id.insert({vertex2, query_v2});
                query_graph.AddVertex(query_v2, data_graph_.vertex_label_[vertex2]);
                vertices.push_back(vertex2);
            } else
                query_v2 = data_id_to_query_id[vertex2];

            // check whether edge is already exist
            query_graph.AddEdge(query_v1, query_v2, data_graph_.edge_label_[vertex1][v_idx]);
            if (edges_before == query_graph.edge_nums_)
                ++delay;
            edges_before = query_graph.edge_nums_;
        }
        if (delay < 3) {
            query_degree.push_back(
                    {queries.size(), (int) queries.size(), (float) query_graph.edge_nums_ / vertex_size});
            queries.push_back(query_graph);
        }


    }
    std::sort(query_degree.begin(), query_degree.end());

    for (int i = 0; i < query_nums; ++i) {
        size_t id=query_degree[i].query_id;
        std::string query_path =
                query_path_prefix_ + "Q" + std::to_string(queries[id].edge_nums_) + "_" + std::to_string(i);
        queries[id].Dump(query_path);
    }
}








