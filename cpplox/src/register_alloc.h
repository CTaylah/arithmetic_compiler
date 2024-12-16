#pragma once


#include "three_address_code.h"
#include <utility>
#include <vector>
#include <map>
#include <optional>
#include <set>

//Step 1: Calculate the lines in TAC for which a variable is active (interference)
//Step 2: Make a graph where each node is a variable, and there is an edge between nodes where the live lines
//overlap
//
//Step 3: Color the graph with the greedy algorithm


struct Bounds{
    int init = -1;
    int last_use = -1;
};

struct GraphNode{
    std::string variable_name;

    bool operator<(const GraphNode& other) const{
        if (variable_name != other.variable_name){
            return variable_name < other.variable_name;
        }
        return false;
    }
};

//Graph format:
// vertex_name : pair({adjacent_vertices}, vertex_color)
using VariableGraph = std::map<GraphNode, std::pair<std::set<GraphNode>, int>>;

inline bool is_variable(std::optional<std::string> arg){
    if(arg.has_value()){
        return arg.value()[0] == 't';
    }
    else return false;
}

inline bool interferes(Bounds a, Bounds b){
    return (a.init <= b.last_use && b.init <= a.last_use);
}

inline std::map<std::string, Bounds> get_variable_lifetimes(TACInstructions tac_instructions){
    std::map<std::string, Bounds> lifetimes;
    int line = 0;
    while(!tac_instructions.is_empty()){
        TAC tac = tac_instructions.pop();
        lifetimes[tac.result] = Bounds({line, line});
        if(is_variable(tac.arg1)){
            lifetimes[tac.arg1].last_use = line;
        }
        if(is_variable(tac.arg2)){
            lifetimes[tac.arg2.value()].last_use = line;
        }
        line++;
    }

    return lifetimes;
}

inline VariableGraph build_graph(const std::map<std::string, Bounds>& lifetimes){
    VariableGraph graph;
    for (const auto& [key, value] : lifetimes){
        GraphNode node = {key};
        graph[node].second = -1;
        for (const auto& [pair_key, pair_value] : lifetimes){
            if(!(key == pair_key) && interferes(value, pair_value)){
                GraphNode adjacent_node = {pair_key};
                graph[node].first.insert(adjacent_node);
            }
        }
    }
    return graph;
}

/* 
 * Greedy graph coloring
*/
inline int get_lowest_available_color(VariableGraph graph,
        GraphNode node, std::set<int> colors){
    std::pair<std::set<GraphNode>,int> adjacents = graph[node];
    for(const auto& vertex : adjacents.first){
        colors.erase(graph[vertex].second);
    }
    if(colors.empty()){
        throw std::runtime_error("Not enough registers; register spilling not yet implemented");
        return -1;
    }
    return *colors.begin();
}

inline void color_graph(VariableGraph& graph, int num_colors) {
    std::set<int> colors;
    std::map<GraphNode, std::set<GraphNode>> colored_graph;
    for (int i = 1; i <= num_colors; ++i){colors.insert(i);}
    for (auto& [node, adj_set] : graph){
        int available_color = get_lowest_available_color(graph, node, colors);
        graph[node].second = available_color;
    }
}
/* 
 * Print Functions
 */
inline void print_lifetimes(const std::map<std::string, Bounds>& lifetimes) {
    for (const auto& [variable, bounds] : lifetimes) {
        std::cout << "Variable: " << variable
                  << ", Init: " << bounds.init
                  << ", Last Use: " << bounds.last_use << std::endl;
    }
}

inline void print_interference_graph(const VariableGraph& graph) {
    for (const auto& [node, neighbors] : graph) {
        std::cout << "Variable: " << node.variable_name << " ";
        std::cout << "Color: " << neighbors.second << " interferes with: ";
        for (const auto& neighbor : neighbors.first) {
            std::cout << neighbor.variable_name << " ";
        }
        std::cout << std::endl;
    }
}
inline std::map<std::string,int> interference_graph_to_map(const VariableGraph& graph){
    std::map<std::string,int> register_map;
    for (const auto& [node, neighbors_color] : graph) {
        register_map[node.variable_name] = neighbors_color.second; 
    }
    return register_map;
}

inline std::map<std::string,int> get_register_map(TACInstructions instructions, int num_registers){
    std::map<std::string, Bounds> lifetimes = get_variable_lifetimes(instructions);
    VariableGraph graph = build_graph(lifetimes);
    color_graph(graph, num_registers);
    std::map<std::string,int> register_map = interference_graph_to_map(graph);
    return register_map;
}
