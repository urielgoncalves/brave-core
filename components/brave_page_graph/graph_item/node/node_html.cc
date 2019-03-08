/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/node/node_html.h"
#include <string>
#include <sstream>
#include "brave/components/brave_page_graph/graph_item/edge/edge_node_remove.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"
#include "brave/components/brave_page_graph/page_graph.h"

using ::std::string;
using ::std::stringstream;
using ::std::to_string;

namespace brave_page_graph {

NodeHTML::NodeHTML(const PageGraph* graph, const PageGraphId id, const
    DOMNodeId node_id, const string& tag_name) :
    Node(graph, id),
    node_id_(node_id),
    tag_name_(tag_name) {}

NodeHTML::~NodeHTML() {}

string NodeHTML::ItemName() const {
  return "NodeHTML#" + to_string(id_);
}

// Special case for when something (script) is removing an HTML element
// from the DOM.
// void AddOutEdge(shared_ptr<EdgeNodeRemove> in_edge) {
  // parent_edge_ = nullptr;
  // Node::AddOutEdge(in_edge);
// }

// void AddOutEdge(shared_ptr<Edge> out_edge) {
  // weak_ptr<Edge> out_edge_ref = out_edge;
  // out_edges_ptr_->push_back(out_edge_ref);
  // Node::AddOutEdge(in_edge);
// }

string NodeHTML::ToStringBody() const {
  stringstream string_builder;
  string_builder << ItemName();
  string_builder << " [DOMNodeId:";
  string_builder << to_string(node_id_);
  string_builder << ", tag:";
  string_builder << tag_name_;
  string_builder << ", attributes=";
  for (const auto& attr : current_attributes_) {
    string key = attr.first;
    string value = attr.second;
    string_builder << "{";
    string_builder << key;
    string_builder << "='";
    string_builder << value;
    string_builder << "'} ";
  }
  string_builder << "]";
  return string_builder.str();
}

}  // brave_page_graph
