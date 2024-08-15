#include "script.h"

#include "parser/lexemizer.h"
#include "parser/tree_builder.h"

Script::Script(const std::string& string) : lexemes_(lexify(string)) {}

static void insert_guids(std::vector<Lexeme::LexemePtr>& lexemes,
                         const SubcomponentNameMap& name_map) {
    for (auto& lexeme : lexemes) {
        lexemes::NamedComponent* named_component =
            lexeme.as<lexemes::NamedComponent>();

        if (!named_component) continue;

        std::string name = named_component->get_name();
        auto component = name_map.find(name);

        if (component == name_map.end()) {
            log_printf(ERROR_REPORTS, "error",
                       "Could not find a component with the name \"%s\" "
                       "(mentioned at line %lu, column %lu).\n",
                       name.c_str(), lexeme->get_line(), lexeme->get_column());
            continue;
        }

        named_component->set_guid(component->second->get_guid());
    }
}

void Script::assemble(Scene& scene, const SubcomponentNameMap& name_map) {
    insert_guids(lexemes_, name_map);

    ParsedTree tree = build_exec_ast(lexemes_);

    nodes_ = tree.roots;

    for (auto node : tree.nodes) {
        node.lock()->assign_scene(scene);
    }

    for (auto node : tree.queue) {
        node.lock()->update(*node.lock());
    }

    for (auto pipe : nodes_) {
        log_printf(STATUS_REPORTS, "status", "Pipe: %s.\n",
                   pipe->debug().c_str());
    }
}

void Script::Node::subscribe_to(ChildReference other_node) {
    Update::Listener listener([this](Script::Node& node) {
        bool should_notify = update(node);
        if (should_notify) {
            trigger();
        }
    });

    other_node->get_update_event().subscribe(listener);

    listeners_.push_back(listener);
}
