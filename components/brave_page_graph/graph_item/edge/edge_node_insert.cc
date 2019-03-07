/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/edge/edge_node_insert.h"
#include <memory>
#include <string>
#include "brave/components/brave_page_graph/graph_item/edge/edge_node.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::shared_ptr;
using ::std::string;

EdgeNodeInsert::EdgeNodeInsert(const PageGraphId id, shared_ptr<Node> in_node,
  shared_ptr<Node> out_node, const DOMNodeId parent_id,
  const DOMNodeId prior_sibling_id = NULL) :
    EdgeNode(id, in_node, out_node),
    parent_id_(parent_id),
    prior_sibling_id_(prior_sibling_id) {}

string EdgeNodeInsert::ItemName() const {
  return "EdgeNodeInsert#" + id_;
}

string EdgeNodeInsert::ToStringBody() const {
  return ItemName() +
    " [parent:" + ::std::to_string(parent_id_) +
    ", sibling:" + ::std::to_string(prior_sibling_id_) + "]";
}