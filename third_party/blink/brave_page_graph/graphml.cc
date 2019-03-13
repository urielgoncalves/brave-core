/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include <map>
#include <ostream>
#include <set>
#include <sstream>
#include <string>
#include "brave/third_party/blink/brave_page_graph/graph_item.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html_element.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node_html.h"
#include "brave/third_party/blink/brave_page_graph/page_graph.h"
#include "brave/third_party/blink/brave_page_graph/types.h"

using ::std::endl;
using ::std::map;
using ::std::set;
using ::std::string;
using ::std::stringstream;
using ::std::to_string;

namespace brave_page_graph {

namespace {
  GraphMLAttr* html_structure_attr_type = nullptr;
  map<string, GraphMLAttr> graphml_attr_map;
}

string graphml_for_html_structure(const NodeHTMLElement* node) noexcept {
  if (node->ChildNodes().size() == 0) {
    return "";
  }

  if (html_structure_attr_type == nullptr) {
    GraphMLAttr new_attr = GraphMLAttr::Create(GraphMLAttrForTypeNode, "type",
      GraphMLAttrTypeString);
    html_structure_attr_type = &graphml_attr_map[new_attr.MapKey()];
  }
  stringstream builder;
  uint32_t counter = 0;
  for (NodeHTML* child_node : node->ChildNodes()) {
    builder << "<edge id\"t" << to_string(++counter) << "\" " <<
                      "source=\"" << node->GraphMLId() << "\" " <<
                      "target=\"" << child_node->GraphMLId() << "\">";
    builder << html_structure_attr_type->ToAttrString("child");
    builder << "</edge>" << endl;
  }

  return builder.str();
}

string graphml_for_page_graph(const PageGraph* graph) noexcept {
  stringstream builder;
  builder << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
  builder << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"" << endl;
  builder << "\t\txmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << endl;
  builder << "\t\txsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns" << endl;
  builder << "\t\t\thttp://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">" << endl;
  
  // Make sure we don't define the same GraphML attribute twice.
  set<uint64_t> graphml_set;
  for (const GraphItem* elm : graph->GraphItems()) {
    for (const auto& elm : elm->GraphMLAttributeDefs()) {
      const GraphMLAttr attr = elm.second;
      if (graphml_set.count(attr.Id()) == 1) {
        continue;
      }
      builder << attr.ToAttrDefString() << endl;
    }
  }

  builder << "\t<graph id=\"G\" edgedefault=\"directed\">" << endl;

  for (const Node* elm : graph->Nodes()) {
    builder << elm->GraphMLTag() << endl;
  }
  for (const Edge* elm : graph->Edges()) {
    builder << elm->GraphMLTag() << endl;
  }
  for (const NodeHTMLElement* elm : graph->HTMLElementNodes()) {
    builder << graphml_for_html_structure(elm);
  }

  builder << "\t</graph>" << endl;
  builder << "</graphml>" << endl;
  return builder.str();
}

string graphml_type_to_string(const GraphMLAttrType type) noexcept {
  switch (type) {
    case GraphMLAttrTypeString:
      return "string";
    case GraphMLAttrTypeBoolean:
      return "boolean";
    case GraphMLAttrTypeInt:
      return "int";
    case GraphMLAttrTypeLong:
      return "long";
    case GraphMLAttrTypeFloat:
      return "float";
    case GraphMLAttrTypeDouble:
      return "double";
    case GraphMLAttrTypeUnknown:
    default:
      return "unknown";
  }
}

string graphml_for_to_string(const GraphMLAttrForType type) noexcept {
  switch (type) {
    case GraphMLAttrForTypeNode:
      return "node";
    case GraphMLAttrForTypeEdge:
      return "edge";
    case GraphMLAttrForTypeUnknown:
    default:
      return "unknown";
  }
}

GraphMLAttr GraphMLAttr::Create(const GraphMLAttrForType for_value,
    const string& name_value, const GraphMLAttrType type_value) {
  string key = graphml_for_to_string(for_value) + ":" +
    name_value + ":" + graphml_type_to_string(type_value);

  if (graphml_attr_map.count(key) == 1) {
    return graphml_attr_map[key];
  }

  GraphMLAttr new_graphml_type = GraphMLAttr(for_value, name_value, type_value,
    graphml_attr_map.size());
  
  graphml_attr_map.emplace(key, new_graphml_type);
  return new_graphml_type;
}

string GraphMLAttr::GraphMLId() const {
  return "d" + to_string(id_);
}

string GraphMLAttr::ToAttrDefString() const {
   return "<key id=\"" + GraphMLId() + "\" " +
                "for=\"" + graphml_for_to_string(for_) + "\" " +
                "attr.name=\"" + name_ + "\" " +
                "attr.type=\"" + graphml_type_to_string(type_) + "\"/>\n";
}

string GraphMLAttr::ToAttrString(const string& value) const {
  return "<data key=\"" + GraphMLId() + "\">" +
            "<![CDATA[" + value + "]]>" +
            "</data>\n";
}

uint64_t GraphMLAttr::Id() const {
  return id_;
}

string GraphMLAttr::MapKey() const {
  return graphml_for_to_string(for_) + ":" + name_ +
    ":" + graphml_type_to_string(type_);
}

}  // namespace brave_page_graph
