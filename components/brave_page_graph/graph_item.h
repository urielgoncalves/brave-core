/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_H_

#include <string>
#include "brave/components/brave_page_graph/types.h"

using ::std::string;

namespace brave_page_graph {

class GraphItem {
 public:
  GraphItem() = delete;
  explicit GraphItem(PageGraphId id);
  explicit GraphItem(const GraphItem& item) = default;
  ~GraphItem() = default;

  string ToString() const;
  virtual string ItemName() const;
  PageGraphId GetId() const;

 protected:
  string ToStringBody() const;
  string ToStringPrefix() const;
  string ToStringSuffix() const;
  const PageGraphId id_; 
};

}

#endif BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPH_ITEM_H_
