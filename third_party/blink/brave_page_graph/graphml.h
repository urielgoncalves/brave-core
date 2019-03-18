/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPHML_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPHML_H_

#include <string>
#include <vector>

using ::std::string;
using ::std::to_string;
using ::std::vector;

namespace brave_page_graph {

class Edge;
class Node;
class NodeHTMLElement;
class PageGraph;

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

typedef string GraphMLXML;
typedef vector<const GraphMLXML> GraphMLXMLGroup;

class GraphMLAttr {
 public:
  GraphMLAttr() = delete;
  GraphMLAttr(const GraphMLAttrForType for_value, const string& name);
  GraphMLAttr(const GraphMLAttrForType for_value, const string& name,
    const GraphMLAttrType type);
  string GraphMLId() const;
  GraphMLXML ToDefinition() const;
  GraphMLXML ToValue(const string& value) const;

 protected:
  const uint64_t id_;
  const GraphMLAttrForType for_;
  const string name_;
  const GraphMLAttrType type_;
};

typedef enum {
  GraphMLAttrDefBeforeNodeId = 0,
  GraphMLAttrDefCallArgs,
  GraphMLAttrDefEdgeType,
  GraphMLAttrDefKey,
  GraphMLAttrDefMethodName,
  GraphMLAttrDefNodeId,
  GraphMLAttrDefNodeTag,
  GraphMLAttrDefNodeText,
  GraphMLAttrDefNodeType,
  GraphMLAttrDefParentNodeId,
  GraphMLAttrDefRequestType,
  GraphMLAttrDefScriptType,
  GraphMLAttrDefUrl,
  GraphMLAttrDefValue,
  GraphMLAttrDefUnknown,
} GraphMLAttrDef;
GraphMLAttr* graphml_attr_def_for_type(const GraphMLAttrDef type) noexcept;

GraphMLXML graphml_attr_def_to_string(const GraphMLAttr attr_def) noexcept;
string graphml_for_html_structure(const NodeHTMLElement* const node) noexcept;
string graphml_for_page_graph(const PageGraph* const graph) noexcept;

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_GRAPHML_H_
