/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_ATTRIBUTE_DELETE_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_ATTRIBUTE_DELETE_H_

#include <string>
#include "brave/components/brave_page_graph/graph_item/edge.h"
#include "brave/components/brave_page_graph/graph_item/edge/edge_attribute.h"
#include "brave/components/brave_page_graph/page_graph.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

class Node;
class NodeHTMLElement;
class NodeScript;
class PageGraph;

class EdgeAttributeDelete final : public EdgeAttribute {
friend class PageGraph;
 public:
  EdgeAttributeDelete() = delete;
  ~EdgeAttributeDelete() override;
  string ItemName() const override;

 protected:
  EdgeAttributeDelete(const PageGraph* graph, const PageGraphId id,
    const NodeScript* out_node, const NodeHTMLElement* in_node,
    const string& name);
  string ToStringBody() const override;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_ATTRIBUTE_DELETE_H_
