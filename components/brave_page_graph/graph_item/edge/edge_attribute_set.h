/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_ATTRIBUTE_SET_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_ATTRIBUTE_SET_H_

#include <string>
#include "brave/components/brave_page_graph/graph_item/edge.h"
#include "brave/components/brave_page_graph/graph_item/node.h"
#include "brave/components/brave_page_graph/graph_item/edge/edge_attribute.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::shared_ptr;
using ::std::string;

namespace brave_page_graph {

class EdgeAttributeSet : public EdgeAttribute {
 public:
  EdgeAttributeSet() = delete;
  EdgeAttributeSet(const PageGraphId id, shared_ptr<Node> in_node,
    shared_ptr<Node> out_node, const string& name, const string& value);
  explicit EdgeAttributeSet(const EdgeAttributeSet& attribute) = default;
  ~EdgeAttributeSet() = default;

  string ItemName() const;
  string AttributeValue() const;

 protected:
  string ToStringBody() const;
  const string name_;
  const string value_;
};

}  // namespace brave_page_graph

#endif BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_EDGE_EDGE_ATTRIBUTE_SET_H_
