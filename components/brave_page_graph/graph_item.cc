/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/components/brave_page_graph/graph_item.h"
#include <string>
#include "brave/components/brave_page_graph/page_graph.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

GraphItem::GraphItem(const PageGraph* graph, const PageGraphId id) :
    graph_(graph),
    id_(id) {}

GraphItem::~GraphItem() {}

PageGraphId GraphItem::GetId() const {
  return id_;
}

string GraphItem::ToStringBody() const {
  return ItemName();
}

string GraphItem::ToString() const {
  return ToStringPrefix() + ToStringBody() + ToStringSuffix();
}

}  // namespace brave_page_graph
