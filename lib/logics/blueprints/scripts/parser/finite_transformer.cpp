#include "finite_transformer.h"

#include <deque>
#include <set>
#include <sstream>

using NodePtr = std::shared_ptr<FTNode>;

FiniteTransformer::FiniteTransformer(const FTNode& root) {
    root_ = nodes_[root.get_guid()] = std::make_shared<FTNode>(root);

    std::deque<NodePtr> queue = {root_};

    while (!queue.empty()) {
        NodePtr node = queue.front();
        queue.pop_front();

        for (auto& [symbol, connections] : node->connections_) {
            for (auto& connection : connections) {
                bool existed = false;
                NodePtr neighbor = add_or_extract(*connection.target, existed);
                connection.target = neighbor.get();
                if (!existed) {
                    queue.push_back(neighbor);
                }
            }
        }
    }
}

struct ProcessingState {
    std::string prefix{};
    FTNode* node = nullptr;
    std::string_view view{};

    unsigned long long hash() const {
        static std::hash<std::string> hash;
        return (uintptr_t)node + hash(prefix);
    }

    friend bool operator<(const ProcessingState& alpha,
                          const ProcessingState& beta) {
        return alpha.hash() < beta.hash();
    }
};

// Yes, I decided to use a macro. Yes, I deserve all hatred in the world.
#define REPORT_OR_ABORT()                                \
    {                                                    \
        if (state.node->terminal_) {                     \
            result.end_guid = state.node->guid_;         \
            result.match = true;                         \
            result.word = state.prefix;                  \
            string.remove_prefix(state.prefix.length()); \
                                                         \
            return result;                               \
        }                                                \
        continue;                                        \
    }

FiniteTransformer::ProcessedWord FiniteTransformer::
operator()(std::string_view& string) {
    ProcessedWord result{};

    ProcessingState starting_state;
    starting_state.prefix = std::string();
    starting_state.node = root_.get();
    starting_state.view = string;

    std::set<ProcessingState> states = {starting_state};

    while (!states.empty()) {
        std::set<ProcessingState> new_states{};

        for (const ProcessingState& state : states) {
            if (state.view.size() == 0) REPORT_OR_ABORT();

            char symbol = state.view[0];
            auto& connections = state.node->connections_;
            if (!connections.contains(symbol)) REPORT_OR_ABORT();
            auto options = connections[symbol];

            if (options.empty()) REPORT_OR_ABORT();

            for (size_t id = 0; id < options.size(); ++id) {
                auto& link = options[id];

                ProcessingState new_state =
                    (id == options.size() - 1) ? std::move(state) : state;

                if (link.replacement != '\0') {
                    new_state.prefix.push_back(link.replacement);
                }
                new_state.view.remove_prefix(1);
                new_state.node = link.target;

                new_states.insert(std::move(new_state));
            }
        }

        states = new_states;
    }

    return result;
}

std::shared_ptr<FTNode> FiniteTransformer::
    add_or_extract(const FTNode& origin, bool& existed_before) {
    if (nodes_.contains(origin.guid_)) {
        existed_before = true;
        return nodes_[origin.guid_];
    }

    existed_before = false;
    return nodes_[origin.guid_] = std::make_shared<FTNode>(origin);
}

void FTNode::operator>>(const PendingConnection& addition) {
    for (auto [symbol, replacement] : addition.transforms) {
        if (!connections_.contains(symbol)) {
            connections_[symbol] = {};
        }

        FTNode::Connection conn_info{};
        conn_info.replacement = replacement;
        conn_info.target = addition.target;

        connections_[symbol].push_back(conn_info);
    }
}

FTNode::PendingConnection FTNode::by(char symbol) { return by(symbol, symbol); }

FTNode::PendingConnection FTNode::by(char symbol, char replacement) {
    return by((std::map<char, char>){{symbol, replacement}});
}

FTNode::PendingConnection FTNode::by(const std::string& symbols) {
    std::map<char, char> map{};
    for (char symbol : symbols) {
        map[symbol] = symbol;
    }
    return by(map);
}

FTNode::PendingConnection FTNode::by(const std::map<char, char> transitions) {
    PendingConnection conn{};
    conn.target = this;
    conn.transforms = transitions;
    return conn;
}

FTNode::PendingConnection FTNode::except(char symbol) {
    std::map<char, char> map{};
    for (char ch = 1; ch != 0; ++ch) {
        map[ch] = ch;
    }
    map.erase(symbol);
    return by(map);
}

FTNode::PendingConnection FTNode::except(const std::string& symbols) {
    std::map<char, char> map{};
    for (char symbol = 1; symbol != 0; ++symbol) {
        map[symbol] = symbol;
    }
    for (char symbol : symbols) {
        map.erase(symbol);
    }
    return by(map);
}

FTNode FTNode::merge(const FTNode& alpha, const FTNode& beta) {
    FTNode root{};
    for (const auto& [symbol, routes] : alpha.connections_) {
        for (const auto& route : routes) {
            root >> route.target->by(symbol, route.replacement);
        }
    }
    for (const auto& [symbol, routes] : beta.connections_) {
        for (const auto& route : routes) {
            root >> route.target->by(symbol, route.replacement);
        }
    }

    root.terminal_ = alpha.terminal_ || beta.terminal_;

    return root;
}

void FTNode::append_bor(const std::string_view& word) {
    if (word.length() == 0) {
        mark_terminal();
        return;
    }

    char symbol = word[0];

    if (!connections_.contains(symbol)) {
        auto new_node = std::make_shared<FTNode>();
        virtual_nodes_.push_back(new_node);
        operator>>(new_node->by(symbol));
    }

    std::string_view copy = word;
    copy.remove_prefix(1);
    connections_[symbol][0].target->append_bor(copy);
}
