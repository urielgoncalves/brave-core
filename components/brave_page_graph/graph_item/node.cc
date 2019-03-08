/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/node.h"
#include <sstream>
#include "brave/components/brave_page_graph/graph_item/edge.h"
#include "brave/components/brave_page_graph/graph_item.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::stringstream;

namespace brave_page_graph {

Node::Node(const PageGraph* graph, const PageGraphId id) :
    GraphItem(id),
    graph_(graph) {}

Node::~Node() {}

void Node::AddInEdge(Edge* in_edge) {
  in_edges_ptr_.push_back(in_edge);
}

void Node::AddOutEdge(Edge* out_edge) {
  out_edges_ptr_.push_back(out_edge);
}

string Node::ToStringPrefix() const {
  stringstream string_builder;
  for (const Edge* elm : in_edges_ptr_) {
    string_builder << elm->ItemName() << " -> \n";
  }
  string_builder << "  ";
  return string_builder.str();
}

string Node::ToStringSuffix() const {
  stringstream string_builder;
  string_builder << "\n";
  for (const Edge* elm : out_edges_ptr_) {
    string_builder << "     -> " << elm->ItemName() << "\n";
  }
  return string_builder.str();
}

}  // namespace brave_page_graph
