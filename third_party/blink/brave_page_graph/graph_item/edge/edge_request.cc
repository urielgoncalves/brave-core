/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_request.h"
#include <string>
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge_node.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

EdgeRequest::EdgeRequest(const PageGraph* graph, const PageGraphId id,
    const Node* const out_node, const Node* const in_node, const string& url,
    const RequestType type) :
      Edge(graph, id, out_node, in_node),
      url_(url),
      type_(type) {}

EdgeRequest::~EdgeRequest() {}

ItemName EdgeRequest::GetItemName() const {
  return "EdgeRequest#" + ::std::to_string(id_);
}

ItemDesc EdgeRequest::GetDescBody() const {
  return GetItemName() + " [url:" + url_ +
    ", type:" + request_type_to_string(type_) + "]";
}

GraphMLXMLList EdgeRequest::GraphMLAttributes() const {
  return GraphMLXMLList({
    graphml_attr_def_for_type(kGraphMLAttrDefEdgeType)
      ->ToValue("request"),
    graphml_attr_def_for_type(kGraphMLAttrDefRequestType)
      ->ToValue(request_type_to_string(type_)),
    graphml_attr_def_for_type(kGraphMLAttrDefUrl)
      ->ToValue(url_)
  });
}

}  // namespace brave_page_graph
