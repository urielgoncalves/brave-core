/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CALL_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CALL_H_

#include <string>
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

class Node;
class PageGraph;

class EdgeRequest final : public virtual Edge {
friend class PageGraph;
 public:
  EdgeRequest() = delete;
  ~EdgeRequest() override;
  string ItemName() const override;

 protected:
  EdgeRequest(const PageGraph* graph, const PageGraphId id,
    const Node* const out_node, const Node* const in_node, const string& url,
    const RequestType type);
  string ToStringBody() const override;
  GraphMLXMLGroup GraphMLAttributes() const override;

  const string url_;
  const RequestType type_;
};

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_CALL_H_
