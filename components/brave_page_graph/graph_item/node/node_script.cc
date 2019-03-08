/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/node/node_script.h"
#include <string>
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"
#include "brave/components/brave_page_graph/page_graph.h"

using ::std::string;
using ::std::to_string;

namespace brave_page_graph {

NodeScript::NodeScript(const PageGraph* graph, const PageGraphId id,
    const ScriptId script_id, const ScriptType type) :
    Node(graph, id),
    script_id_(script_id),
    type_(type) {}

NodeScript::~NodeScript() {}

string NodeScript::ItemName() const {
  return "NodeScript#" + to_string(id_);
}

string NodeScript::ToStringBody() const {
  return ItemName() +
    " [ScriptId:" + to_string(script_id_) +
    ", Type:"  + script_type_to_string(type_) + "]"; 
}

}  // brave_page_graph
