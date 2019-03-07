/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>
#include "brave/components/brave_page_graph/graph_item/edge.h"
#include "brave/components/brave_page_graph/graph_item.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

Edge::Edge(const PageGraphId id, shared_ptr<Node> in_node,
      shared_ptr<Node> out_node) :
    GraphItem(id) {
  in_node_ptr_ = in_node;
  out_node_ptr_ = out_node;
}

string Edge::ItemName() const {
  return "Edge#" + ::std::to_string(id_);
}

string Edge::ToStringPrefix() const {
  if (const auto& in_node_ = in_node_ptr_.lock()) {
    return in_node_->ItemName() + " -> ";
  }
  return "(null) -> ";
}

string Edge::ToStringBody() const {
  return ItemName();
}

string Edge::ToStringSuffix() const {
  if (const auto& out_node_ = out_node_ptr_.lock()) {
    return " -> " + out_node_->ItemName();
  }
  return " -> (null)";
}

}  // namespace brave_page_graph
