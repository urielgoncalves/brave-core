/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/edge/edge_node_insert.h"
#include <string>
#include "brave/components/brave_page_graph/graph_item/edge/edge_node.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/graph_item/node/node_html_element.h"
#include "brave/components/brave_page_graph/graph_item/node/node_html.h"
#include "brave/components/brave_page_graph/graph_item/node/node_actor.h"
#include "brave/components/brave_page_graph/page_graph.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;
using ::std::to_string;

namespace brave_page_graph {

EdgeNodeInsert::EdgeNodeInsert(const PageGraph* graph, const PageGraphId id,
    const NodeActor* out_node, const NodeHTML* in_node,
    const DOMNodeId parent_id, const DOMNodeId prior_sibling_id) :
    EdgeNode(graph, id, out_node, in_node),
    parent_id_(parent_id),
    prior_sibling_id_(prior_sibling_id) {}

EdgeNodeInsert::~EdgeNodeInsert() {}

NodeHTMLElement* EdgeNodeInsert::GetParentNode() const {
  return graph_->GetHTMLElementNode(parent_id_);
}

NodeHTML* EdgeNodeInsert::GetPriorSiblingNode() const {
  return graph_->GetHTMLNode(prior_sibling_id_);
}

string EdgeNodeInsert::ItemName() const {
  return "EdgeNodeInsert#" + to_string(id_);
}

string EdgeNodeInsert::ToStringBody() const {
  return ItemName() +
    " [parent:" + to_string(parent_id_) +
    ", sibling:" + to_string(prior_sibling_id_) + "]";
}

}  // namespace brave_page_graph
