/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include <memory>
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
using ::std::set;
using ::std::string;
using ::std::stringstream;
using ::std::to_string;
using ::std::unique_ptr;

namespace brave_page_graph {

namespace {
  GraphMLAttr* attr_name = new GraphMLAttr(GraphMLAttrForTypeEdge, "attr name");
  GraphMLAttr* attr_value = new GraphMLAttr(GraphMLAttrForTypeEdge,
    "attr value");
  GraphMLAttr* before_node_attr = new GraphMLAttr(GraphMLAttrForTypeEdge,
    "before", GraphMLAttrTypeInt);
  GraphMLAttr* call_args = new GraphMLAttr(GraphMLAttrForTypeEdge, "args");
  GraphMLAttr* edge_type = new GraphMLAttr(GraphMLAttrForTypeEdge, "edge type");
  GraphMLAttr* key_attr = new GraphMLAttr(GraphMLAttrForTypeEdge, "key");
  GraphMLAttr* method_attr = new GraphMLAttr(GraphMLAttrForTypeEdge, "method");
  GraphMLAttr* tag_attr = new GraphMLAttr(GraphMLAttrForTypeNode, "tag name");
  GraphMLAttr* node_id_attr = new GraphMLAttr(GraphMLAttrForTypeNode,
    "node id", GraphMLAttrTypeInt);
  GraphMLAttr* node_text = new GraphMLAttr(GraphMLAttrForTypeNode, "text");
  GraphMLAttr* node_type = new GraphMLAttr(GraphMLAttrForTypeNode, "node type");
  GraphMLAttr* parent_node_attr = new GraphMLAttr(GraphMLAttrForTypeEdge,
    "parent", GraphMLAttrTypeInt);
  GraphMLAttr* script_type = new GraphMLAttr(GraphMLAttrForTypeNode,
    "script type");
  GraphMLAttr* url_attr = new GraphMLAttr(GraphMLAttrForTypeEdge, "url");
  GraphMLAttr* request_type_attr = new GraphMLAttr(GraphMLAttrForTypeEdge,
    "request type");
  GraphMLAttr* value_attr = new GraphMLAttr(GraphMLAttrForTypeEdge, "value");

  const int num_attrs = 16;
  GraphMLAttr* all_attrs[num_attrs] = {attr_name, attr_value, before_node_attr,
    call_args, edge_type, key_attr, method_attr, tag_attr, node_id_attr,
    node_text, node_type, parent_node_attr, script_type, url_attr,
    request_type_attr, value_attr};
}

string graphml_for_html_structure(const NodeHTMLElement* const node) noexcept {
  if (node->ChildNodes().size() == 0) {
    return "";
  }

  stringstream builder;
  uint32_t counter = 0;
  for (NodeHTML* child_node : node->ChildNodes()) {
    builder << "<edge id\"t" << to_string(++counter) << "\" " <<
                      "source=\"" << node->GraphMLId() << "\" " <<
                      "target=\"" << child_node->GraphMLId() << "\">";
    // builder << html_structure_attr_type->ToValue("child");
    builder << "</edge>" << endl;
  }

  return builder.str();
}

string graphml_for_page_graph(const PageGraph* const graph) noexcept {
  stringstream builder;
  builder << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" << endl;
  builder << "<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\"" << endl;
  builder << "\t\txmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"" << endl;
  builder << "\t\txsi:schemaLocation=\"http://graphml.graphdrawing.org/xmlns" << endl;
  builder << "\t\t\thttp://graphml.graphdrawing.org/xmlns/1.0/graphml.xsd\">" << endl;
  
  for (int i = 0; i < num_attrs; i += 1) {
    builder << "\t" << all_attrs[i]->ToDefinition() << "\n";
  }

  builder << "\t<graph id=\"G\" edgedefault=\"directed\">" << endl;

  for (const unique_ptr<Node>& elm : graph->Nodes()) {
    builder << elm->GraphMLTag() << endl;
  }
  for (const unique_ptr<const Edge>& elm : graph->Edges()) {
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

namespace {
  uint32_t graphml_index = 0;
}

GraphMLAttr::GraphMLAttr(const GraphMLAttrForType for_value,
    const string& name) :
      id_(++graphml_index),
      for_(for_value),
      name_(name),
      type_(GraphMLAttrTypeString) {}

GraphMLAttr::GraphMLAttr(const GraphMLAttrForType for_value, const string& name,
    const GraphMLAttrType type) :
      id_(++graphml_index),
      for_(for_value),
      name_(name),
      type_(type) {}

string GraphMLAttr::GraphMLId() const {
  return "d" + to_string(id_);
}

GraphMLXML GraphMLAttr::ToDefinition() const {
   return "<key id=\"" + GraphMLId() + "\" " +
                "for=\"" + graphml_for_to_string(for_) + "\" " +
                "attr.name=\"" + name_ + "\" " +
                "attr.type=\"" + graphml_type_to_string(type_) + "\"/>\n";
}

GraphMLXML GraphMLAttr::ToValue(const string& value) const {
  if (type_ == GraphMLAttrTypeInt) {
    return "<data key=\"" + GraphMLId() + "\">" + value + "]]></data>\n";
  }
  return "<data key=\"" + GraphMLId() + "\">" +
            "<![CDATA[" + value + "]]>" +
            "</data>\n";
}

GraphMLAttr* graphml_attr_def_for_type(const GraphMLAttrDef type) noexcept {
  switch (type) {
    case GraphMLAttrDefBeforeNodeId:
      return before_node_attr;
    case GraphMLAttrDefCallArgs:
      return call_args;
    case GraphMLAttrDefEdgeType:
      return edge_type;
    case GraphMLAttrDefKey:
      return key_attr;
    case GraphMLAttrDefMethodName:
      return method_attr;
    case GraphMLAttrDefNodeTag:
      return tag_attr;
    case GraphMLAttrDefNodeId:
      return node_id_attr;
    case GraphMLAttrDefNodeText:
      return node_text;
    case GraphMLAttrDefNodeType:
      return node_type;
    case GraphMLAttrDefParentNodeId:
      return parent_node_attr;
    case GraphMLAttrDefRequestType:
      return request_type_attr;
    case GraphMLAttrDefScriptType:
      return script_type;
    case GraphMLAttrDefUrl:
      return url_attr;
    case GraphMLAttrDefValue:
      return value_attr;
    case GraphMLAttrDefUnknown:
      return nullptr;
  }
}

}  // namespace brave_page_graph
