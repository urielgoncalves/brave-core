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
#include "base/logging.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/graph_item.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/edge/edge.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node.h"
#include "brave/third_party/blink/brave_page_graph/graphml.h"
#include "brave/third_party/blink/brave_page_graph/graph_item/node/node.h"
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
  GraphMLAttr* attr_name = new GraphMLAttr(kGraphMLAttrForTypeEdge,
    "attr name");
  GraphMLAttr* attr_value = new GraphMLAttr(kGraphMLAttrForTypeEdge,
    "attr value");
  GraphMLAttr* before_node_attr = new GraphMLAttr(kGraphMLAttrForTypeEdge,
    "before", kGraphMLAttrTypeLong);
  GraphMLAttr* call_args = new GraphMLAttr(kGraphMLAttrForTypeEdge, "args");
  GraphMLAttr* edge_type = new GraphMLAttr(kGraphMLAttrForTypeEdge,
    "edge type");
  GraphMLAttr* is_style_attr = new GraphMLAttr(kGraphMLAttrForTypeEdge,
    "is style", kGraphMLAttrTypeBoolean);
  GraphMLAttr* key_attr = new GraphMLAttr(kGraphMLAttrForTypeEdge, "key");
  GraphMLAttr* method_attr = new GraphMLAttr(kGraphMLAttrForTypeEdge, "method");
  GraphMLAttr* tag_attr = new GraphMLAttr(kGraphMLAttrForTypeNode, "tag name");
  GraphMLAttr* node_id_attr = new GraphMLAttr(kGraphMLAttrForTypeNode,
    "node id", kGraphMLAttrTypeLong);
  GraphMLAttr* node_text = new GraphMLAttr(kGraphMLAttrForTypeNode, "text");
  GraphMLAttr* node_type = new GraphMLAttr(kGraphMLAttrForTypeNode,
    "node type");
  GraphMLAttr* parent_node_attr = new GraphMLAttr(kGraphMLAttrForTypeEdge,
    "parent", kGraphMLAttrTypeLong);
  GraphMLAttr* script_id_attr = new GraphMLAttr(kGraphMLAttrForTypeEdge,
    "script id", kGraphMLAttrTypeLong);
  GraphMLAttr* script_type = new GraphMLAttr(kGraphMLAttrForTypeNode,
    "script type");
  GraphMLAttr* status_type = new GraphMLAttr(kGraphMLAttrForTypeEdge, "status");
  GraphMLAttr* success_attr = new GraphMLAttr(kGraphMLAttrForTypeNode,
    "is success", kGraphMLAttrTypeBoolean);
  GraphMLAttr* url_attr = new GraphMLAttr(kGraphMLAttrForTypeNode, "url");
  GraphMLAttr* request_id_attr = new GraphMLAttr(kGraphMLAttrForTypeEdge,
    "request id", kGraphMLAttrTypeLong);
  GraphMLAttr* request_type_attr = new GraphMLAttr(kGraphMLAttrForTypeEdge,
    "request type");
  GraphMLAttr* resource_type_attr = new GraphMLAttr(kGraphMLAttrForTypeEdge,
    "resource type");
  GraphMLAttr* value_attr = new GraphMLAttr(kGraphMLAttrForTypeEdge, "value");

  const int num_attrs = 22;
  GraphMLAttr* all_attrs[num_attrs] = {attr_name, attr_value, before_node_attr,
    call_args, edge_type, key_attr, method_attr, tag_attr, node_id_attr,
    node_text, node_type, parent_node_attr, script_type, status_type,
    success_attr, url_attr, request_id_attr, request_type_attr,
    resource_type_attr, value_attr, is_style_attr};
}

GraphMLXML graphml_for_page_graph(const PageGraph* const graph) noexcept {
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
    builder << elm->GetGraphMLTag() << endl;
  }
  for (const unique_ptr<const Edge>& elm : graph->Edges()) {
    builder << elm->GetGraphMLTag() << endl;
  }

  builder << "\t</graph>" << endl;
  builder << "</graphml>" << endl;
  return builder.str();
}

namespace {
  uint32_t graphml_index = 0;
}

GraphMLAttr::GraphMLAttr(const GraphMLAttrForType for_value,
    const string& name) :
      id_(++graphml_index),
      for_(for_value),
      name_(name),
      type_(kGraphMLAttrTypeString) {}

GraphMLAttr::GraphMLAttr(const GraphMLAttrForType for_value, const string& name,
    const GraphMLAttrType type) :
      id_(++graphml_index),
      for_(for_value),
      name_(name),
      type_(type) {}

GraphMLId GraphMLAttr::GetGraphMLId() const {
  return "d" + to_string(id_);
}

GraphMLXML GraphMLAttr::ToDefinition() const {
   return "<key id=\"" + GetGraphMLId() + "\" " +
                "for=\"" + graphml_for_to_string(for_) + "\" " +
                "attr.name=\"" + name_ + "\" " +
                "attr.type=\"" + graphml_type_to_string(type_) + "\"/>";
}

GraphMLXML GraphMLAttr::ToValue(const char* value) const {
  return ToValue(string(value));
}

GraphMLXML GraphMLAttr::ToValue(const string& value) const {
  LOG_ASSERT(type_ == kGraphMLAttrTypeString);
  return "<data key=\"" + GetGraphMLId() + "\">" +
            "<![CDATA[" + value + "]]>" +
            "</data>";
}

GraphMLXML GraphMLAttr::ToValue(const uint64_t value) const {
  LOG_ASSERT(type_ == kGraphMLAttrTypeLong);
  return "<data key=\"" + GetGraphMLId() + "\">" + to_string(value) + "</data>";
}

GraphMLXML GraphMLAttr::ToValue(const bool value) const {
  LOG_ASSERT(type_ == kGraphMLAttrTypeBoolean);
  return "<data key=\"" + GetGraphMLId() + "\">" + to_string(value) + "</data>";
}

GraphMLAttr* graphml_attr_def_for_type(const GraphMLAttrDef type) noexcept {
  switch (type) {
    case kGraphMLAttrDefBeforeNodeId:
      return before_node_attr;
    case kGraphMLAttrDefCallArgs:
      return call_args;
    case kGraphMLAttrDefEdgeType:
      return edge_type;
    case kGraphMLAttrDefIsStyle:
      return is_style_attr;
    case kGraphMLAttrDefKey:
      return key_attr;
    case kGraphMLAttrDefMethodName:
      return method_attr;
    case kGraphMLAttrDefNodeTag:
      return tag_attr;
    case kGraphMLAttrDefNodeId:
      return node_id_attr;
    case kGraphMLAttrDefNodeText:
      return node_text;
    case kGraphMLAttrDefNodeType:
      return node_type;
    case kGraphMLAttrDefParentNodeId:
      return parent_node_attr;
    case kGraphMLAttrDefRequestId:
      return request_id_attr;
    case kGraphMLAttrDefRequestType:
      return request_type_attr;
    case kGraphMLAttrDefResourceType:
      return resource_type_attr;
    case kGraphMLAttrDefScriptId:
      return script_id_attr;
    case kGraphMLAttrDefScriptType:
      return script_type;
    case kGraphMLAttrDefStatus:
      return status_type;
    case kGraphMLAttrDefSuccess:
      return success_attr;
    case kGraphMLAttrDefUrl:
      return url_attr;
    case kGraphMLAttrDefValue:
      return value_attr;
    case kGraphMLAttrDefUnknown:
      return nullptr;
  }
}

}  // namespace brave_page_graph
