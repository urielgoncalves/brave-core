/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include <string>
#include "brave/components/brave_page_graph/graph_item.h"
#include "brave/components/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

GraphItem::GraphItem(PageGraphId id) :
  id_(id) {}

PageGraphId GraphItem::GetId() const {
  return id_;
}

string GraphItem::ToStringPrefix() const {
  return "?";
}

string GraphItem::ToStringBody() const {
  return "?";
}

string GraphItem::ToStringSuffix() const {
  return "?";
}


string GraphItem::ToString() const {
  return ToStringPrefix() + ToStringBody() + ToStringSuffix();
}

}  // namespace brave_page_graph
