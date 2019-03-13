/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPHML_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPHML_H_

#include <string>

using ::std::string;
using ::std::to_string;

class Node;
class NodeHTMLElement;
class Edge;

namespace brave_page_graph {

typedef enum {
  GraphMLAttrTypeString = 0,
  GraphMLAttrTypeBoolean,
  GraphMLAttrTypeInt,
  GraphMLAttrTypeLong,
  GraphMLAttrTypeFloat,
  GraphMLAttrTypeDouble,
  GraphMLAttrTypeUnknown
} GraphMLAttrType;
string graphml_type_to_string(const GraphMLAttrType type) noexcept;

typedef enum {
  GraphMLAttrForTypeNode = 0,
  GraphMLAttrForTypeEdge,
  GraphMLAttrForTypeUnknown
} GraphMLAttrForType;
string graphml_for_to_string(const GraphMLAttrForType type) noexcept;

class GraphMLAttr {
 public:
  static GraphMLAttr Create(const GraphMLAttrForType for_value,
    const string& name_value, const GraphMLAttrType type_value);
  GraphMLAttr() = delete;
  string GraphMLId() const;
  string ToAttrDefString() const;
  string ToAttrString(const string& value) const;
  uint64_t Id() const;
  string MapKey() const;

 protected:
  GraphMLAttr(const GraphMLAttrForType for_value, const string& name_value,
      const GraphMLAttrType type_value, const uint64_t id_value) :
        id_(id_value),
        for_(for_value),
        name_(name_value),
        type_(type_value) {}
  const uint64_t id_;
  const GraphMLAttrForType for_;
  const string name_;
  const GraphMLAttrType type_;
};
string graphml_attr_def_to_string(const GraphMLAttr attr_def) noexcept;
string graphml_for_html_structure(const NodeHTMLElement* node) noexcept;
string graphml_for_page_graph(const PageGraph* graph) noexcept;

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPHML_H_
