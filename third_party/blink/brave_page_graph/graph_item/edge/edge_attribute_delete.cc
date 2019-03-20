/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_attribute_delete.h"
#include <string>
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_attribute.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html_element.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_script.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::string;
using ::std::to_string;

namespace brave_page_graph {

EdgeAttributeDelete::EdgeAttributeDelete(const PageGraph* graph,
    const PageGraphId id, const NodeScript* const out_node,
    const NodeHTMLElement* const in_node, const string& name) :
      EdgeAttribute(graph, id, out_node, in_node, name) {}

EdgeAttributeDelete::~EdgeAttributeDelete() {}

ItemName EdgeAttributeDelete::GetItemName() const {
  return "EdgeAttributeDelete#" + ::std::to_string(id_);
}

ItemDesc EdgeAttributeDelete::GetDescBody() const {
  return GetItemName() + " [" + AttributeName() + "]";
}

GraphMLXMLList EdgeAttributeDelete::GraphMLAttributes() const {
  GraphMLXMLList attrs = EdgeAttribute::GraphMLAttributes();
  attrs.push_back(graphml_attr_def_for_type(kGraphMLAttrDefEdgeType)
      ->ToValue("attr delete"));
  return attrs;
}

}  // namespace brave_page_graph
