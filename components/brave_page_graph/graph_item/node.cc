/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/node.h"
#include <memory>
#include <sstream>
#include "brave/components/brave_page_graph/graph_item/edge.h"
#include "brave/components/brave_page_graph/graph_item.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::shared_ptr;
using ::std::weak_ptr;
using ::std::stringstream;

namespace brave_page_graph {

Node::Node(const PageGraphId id) :
  GraphItem(id) {}

string Node::ItemName() const {
  return "Node#" + ::std::to_string(id_);
}

void Node::AddInEdge(shared_ptr<Edge> in_edge) {
  weak_ptr<Edge> in_edge_ref = in_edge;
  in_edges_ptr_->push_back(in_edge_ref);
}

void Node::AddOutEdge(shared_ptr<Edge> out_edge) {
  weak_ptr<Edge> out_edge_ref = out_edge;
  out_edges_ptr_->push_back(out_edge_ref);
}

string Node::ToStringPrefix() const {
  stringstream string_builder;
  for (const auto& elm : *in_edges_ptr_) {
    if (const auto& in_edge = elm.lock()) {
      string_builder << in_edge->ItemName() << " -> \n";
    }
  }
  string_builder << "  ";
  return string_builder.str();
}

string Node::ToStringBody() const {
  return ItemName();
}

string Node::ToStringSuffix() const {
  stringstream string_builder;
  string_builder << "\n";
  for (const auto& elm : *out_edges_ptr_) {
    if (const auto& out_edge = elm.lock()) {
      string_builder << "     -> " << out_edge->ItemName() << "\n";
    }
  }
  return string_builder.str();
}

}  // namespace brave_page_graph
