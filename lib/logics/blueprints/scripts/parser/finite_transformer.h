/**
 * @file string_transformer.h
 * @author Kudryashov Ilya (kudriashov.it@phystech.edu)
 * @brief String transformer class
 * @version 0.1
 * @date 2024-12-10
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "hash/guid.h"

struct FTNode;

struct FiniteTransformer {
    FiniteTransformer();
    FiniteTransformer(const FTNode& root);

    struct ProcessedWord {
        std::string word{};
        bool match = false;
        GUID end_guid = GUID::zero;

        operator bool() const { return match; }
    };

    ProcessedWord operator()(std::string_view& string);

   private:
    std::shared_ptr<FTNode> add_or_extract(const FTNode& origin,
                                           bool& existed_before);

    std::map<GUID, std::shared_ptr<FTNode>> nodes_{};

    std::shared_ptr<FTNode> root_{};
};

struct FTNode {
    FTNode() = default;

    FTNode(const FTNode&) = default;
    FTNode(FTNode&&) = default;

    FTNode& operator=(const FTNode&) = default;
    FTNode& operator=(FTNode&&) = default;

    struct PendingConnection {
        FTNode* target = nullptr;
        std::map<char, std::string> transforms{};
    };

    /**
     * @brief Connect a node to the transformer.
     *  Syntax: `node_a >> node_b('symbol', 'replacement')`
     *
     * @param[in] addition
     */
    void operator>>(const PendingConnection& addition);

    PendingConnection by(char symbol);
    PendingConnection by(char symbol, char replacement);
    PendingConnection by(char symbol, const std::string& replacement);
    PendingConnection by(const std::string& symbols);
    PendingConnection by(const std::map<char, std::string>& transitions);
    PendingConnection except(char symbol);
    PendingConnection except(const std::string& symbols);
    PendingConnection by_alnum(const std::string& additions = "");
    PendingConnection by_numeric(const std::string& additions = "");
    PendingConnection by_alphabetic(const std::string& additions = "");
    PendingConnection by_mask(std::function<bool(char)> predicate,
                              const std::string& additions = "");

    void mark_terminal() { terminal_ = true; }

    template <class... Args>
    static FTNode merge(const Args&... nodes) {
        FTNode root{};

        (root.inmerge(nodes), ...);

        return root;
    }

    FTNode* append_bor(const std::string_view& word);

    FiniteTransformer bake() { return FiniteTransformer(*this); }

    friend struct FiniteTransformer;

    GUID get_guid() const { return guid_; }

   private:
    void inmerge(const FTNode& node) {
        for (const auto& [symbol, routes] : node.connections_) {
            for (const auto& route : routes) {
                operator>>(route.target->by(symbol, route.replacement));
            }
        }
        if (node.terminal_) mark_terminal();
    }

    bool terminal_ = false;

    struct Connection {
        FTNode* target = nullptr;
        std::string replacement{};
    };

    bool being_processed_ = false;
    GUID guid_ = GUID::gen();

    std::map<char, std::vector<Connection>> connections_{};

    std::vector<std::shared_ptr<FTNode>> virtual_nodes_{};
};
