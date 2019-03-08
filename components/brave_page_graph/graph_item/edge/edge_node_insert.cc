/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/edge/edge_node_insert.h"
#include <string>
#include "brave/components/brave_page_graph/graph_item/edge/edge_node.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;
using ::std::to_string;

namespace brave_page_graph {

EdgeNodeInsert::EdgeNodeInsert(const PageGraphId id, const Node* in_node,
    const Node* out_node, const DOMNodeId parent_id,
    const DOMNodeId prior_sibling_id) :
    EdgeNode(id, in_node, out_node),
    parent_id_(parent_id),
    prior_sibling_id_(prior_sibling_id) {}

EdgeNodeInsert::~EdgeNodeInsert() {}

string EdgeNodeInsert::ItemName() const {
  return "EdgeNodeInsert#" + to_string(id_);
}

string EdgeNodeInsert::ToStringBody() const {
  return ItemName() +
    " [parent:" + to_string(parent_id_) +
    ", sibling:" + to_string(prior_sibling_id_) + "]";
}

}  // namespace brave_page_graph
