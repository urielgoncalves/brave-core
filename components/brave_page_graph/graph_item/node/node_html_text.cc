/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item/node/node_html_text.h"
#include <iostream>
#include <string>
#include <sstream>
#include "base/logging.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/graph_item/node/node_html.h"
#include "brave/components/brave_page_graph/graph_item/edge/edge_node_remove.h"
#include "brave/components/brave_page_graph/graph_item/edge/edge_node_insert.h"
#include "brave/components/brave_page_graph/graph_item/edge/edge_node_delete.h"
#include "brave/components/brave_page_graph/page_graph.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::endl;
using ::std::string;
using ::std::stringstream;
using ::std::to_string;

namespace brave_page_graph {

NodeHTMLText::NodeHTMLText(const PageGraph* graph, const PageGraphId id,
    const DOMNodeId node_id,  const string& text) :
    NodeHTML(graph, id, node_id),
    text_(text) {}

NodeHTMLText::~NodeHTMLText() {}

string NodeHTMLText::ItemName() const {
  return "TextNode (length: "  + to_string(text_.size()) + ")";
}

string NodeHTMLText::ToHTMLString() const {
  return text_;
}

string NodeHTMLText::ToStringBody() const {
  return "(text)" + text_;
}

void NodeHTMLText::AddInEdge(const EdgeNodeRemove* edge) {
  parent_node_->RemoveChildNode(this);
  parent_node_ = nullptr;
  Node::AddInEdge(edge);
}

void NodeHTMLText::AddInEdge(const EdgeNodeInsert* edge) {
  parent_node_ = edge->GetParentNode();
  parent_node_->PlaceChildNodeAfterSiblingNode(this,
    edge->GetPriorSiblingNode());
  Node::AddInEdge(edge);
}

void NodeHTMLText::AddInEdge(const EdgeNodeDelete* edge) {
  MarkNodeDeleted();
  Node::AddInEdge(edge);
}

void NodeHTMLText::ToHTMLString(const uint32_t indent,
    stringstream& builder) const {
  indent_for_html(indent, builder);
  builder << ToHTMLString() << endl;
}

}  // namespace brave_page_graph
