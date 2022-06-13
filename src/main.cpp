//===----------------------------------------------------------------------===//
//
//                         
//
// 
//
// 
//
//===----------------------------------------------------------------------===//

#include "subgraphmatching/matching.h"
#include "analysis/analysis.h"
#include "util/CLI11.hpp"
#include "util/generator/generator.h"

#include <iostream>
#include "io.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "dir.h"
#include <dirent.h>
#include <cstddef>
int main(int argc, char **argv) {
    CLI::App app{"multi queries over streaming "};


    bool is_generate = true, is_analyze = true;
    std::string query_graph_dir, stream_path, data_graph_path, original_data_path, report_path;
    app.add_option("-d", data_graph_path, "data graph ")->required();
    app.add_option("-q", query_graph_dir, "directory of queries")->required();
    app.add_option("-s", stream_path, "streaming path")->required();
    app.add_option("-ig", is_generate, "generate stream and query graph");
    app.add_option("-od", original_data_path, "the data which is used to generate streaming and data graph");
    app.add_option("-ia", is_analyze, "analyze the streaming and query graph");
    app.add_option("-r", report_path, "output the analysis");
    CLI11_PARSE(app, argc, argv);

    if (is_generate) {
        std::cout << "generate streaming and query graph" << std::endl;

        Generator *generator = new Generator(original_data_path, data_graph_path, stream_path, query_graph_dir);
        generator->GenerateStreaming(STREAMING_SIZE);
        generator->GenerateQueries(QUERY_NUMS,{QUERY_WALK_SIZE_MIN,QUERY_WALK_SIZE_MAX});
        delete(generator);
    }

    std::cout<<"load  data graph"<<std::endl;
    Graph data_graph;
    data_graph.LoadGraphByFile(data_graph_path);

    std::cout<<"load  query graphs"<<std::endl;
    std::vector<Graph> queries;
    {
        DIR *dir;
        struct dirent *dp;
        if ((dir = opendir(query_graph_dir.c_str())) == NULL)
        {
            perror("Open dir error...");
        }
        while ((dp = readdir(dir)) != NULL) {
            std::string query_graph_path=std::string(dp->d_name );

            Graph query_graph;
            query_graph.LoadGraphByFile(query_graph_path);
            queries.push_back(query_graph);

        }
        closedir(dir);

    }


    std::cout<<"load  streaming"<<std::endl;
    Streaming streaming;
    streaming.LoadStreamFromFile(stream_path);

    if(is_analyze){
        std::cout<<"start analyze "<<std::endl;
        Analysis *analysis = new Analysis(data_graph,queries,streaming,report_path);
        analysis->Init();
        analysis->Analyze();
    }
}


