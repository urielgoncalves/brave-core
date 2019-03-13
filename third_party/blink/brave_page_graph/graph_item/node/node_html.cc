/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html.h"
#include <string>
#include <sstream>
#include "base/logging.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::string;
using ::std::stringstream;

namespace brave_page_graph {

string graphml_node_html_text_type(void* node) {
  return "html";
}

NodeHTML::NodeHTML(const PageGraph* graph, const PageGraphId id,
    const DOMNodeId node_id) :
      Node(graph, id),
      node_id_(node_id) {}

NodeHTML::~NodeHTML() {}

GraphMLFuncAttrMap NodeHTML::GraphMLAttributeDefs() const {
  GraphMLFuncAttrMap mapping = Node::GraphMLAttributeDefs();
  mapping.emplace(
    &graphml_node_html_text_type,
    GraphMLAttr::Create(GraphMLAttrForTypeNode, "type",
      GraphMLAttrTypeString));
  return mapping;

}

void NodeHTML::MarkNodeDeleted() {
  LOG_ASSERT(is_deleted_ == false);
  is_deleted_ = true;
}

void indent_for_html(const uint32_t indent, stringstream& builder) {
  for (uint32_t i = 0; i < indent; i += 1) {
    builder << "  ";
  }
}

}  // namespace brave_page_graph
