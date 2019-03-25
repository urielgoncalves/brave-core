/* Copyright (c) 2019 The Brave Software Team. Distributed under the MPL2
 * license. This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_TYPES_H_
#define BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_TYPES_H_

#include <map>
#include <string>
#include <vector>

namespace brave_page_graph {

class Element;
class Edge;
class Node;
class NodeHTML;

typedef std::string RequestUrl;

typedef std::vector<const Edge*> EdgeList;
typedef std::vector<Node*> NodeList;
typedef std::vector<NodeHTML*> HTMLNodeList;
typedef std::map<const std::string, const std::string> AttributeMap;

typedef std::string ItemDesc;
typedef std::string ItemName;
typedef std::string GraphMLXML;
typedef std::string GraphMLId;
typedef std::vector<const GraphMLXML> GraphMLXMLList;

typedef enum {
  kGraphMLAttrDefBeforeNodeId = 0,
  kGraphMLAttrDefCallArgs,
  kGraphMLAttrDefEdgeType,
  kGraphMLAttrDefIsStyle,
  kGraphMLAttrDefKey,
  kGraphMLAttrDefMethodName,
  kGraphMLAttrDefNodeId,
  kGraphMLAttrDefNodeTag,
  kGraphMLAttrDefNodeText,
  kGraphMLAttrDefNodeType,
  kGraphMLAttrDefParentNodeId,
  kGraphMLAttrDefRequestType,
  kGraphMLAttrDefScriptType,
  kGraphMLAttrDefSuccess,
  kGraphMLAttrDefUrl,
  kGraphMLAttrDefValue,
  kGraphMLAttrDefUnknown,
} GraphMLAttrDef;

typedef enum {
  kGraphMLAttrTypeString = 0,
  kGraphMLAttrTypeBoolean,
  kGraphMLAttrTypeInt,
  kGraphMLAttrTypeLong,
  kGraphMLAttrTypeFloat,
  kGraphMLAttrTypeDouble,
  kGraphMLAttrTypeUnknown
} GraphMLAttrType;
std::string graphml_type_to_string(const GraphMLAttrType type) noexcept;

typedef enum {
  kGraphMLAttrForTypeNode = 0,
  kGraphMLAttrForTypeEdge,
  kGraphMLAttrForTypeUnknown
} GraphMLAttrForType;
std::string graphml_for_to_string(const GraphMLAttrForType type) noexcept;

typedef enum {
  kRequestTypeAJAX = 0,
  kRequestTypeAudio,
  kRequestTypeCSS,
  kRequestTypeDocument,
  kRequestTypeFont,
  kRequestTypeImage,
  kRequestTypeScriptClassic,
  kRequestTypeScriptModule,
  kRequestTypeSVG,
  kRequestTypeVideo,
  kRequestTypeUnknown
} RequestType;
std::string request_type_to_string(const RequestType type) noexcept;

typedef enum {
  kResourceTypeAudio = 0,
  kResourceTypeCSS,
  kResourceTypeDocument,
  kResourceTypeFont,
  kResourceTypeImage,
  kResourceTypeVideo,
  kResourceTypeScript,
  kResourceTypeSVG,
  kResourceTypeUnknown
} ResourceType;
std::string request_type_to_string(const ResourceType type) noexcept;

typedef enum {
  kScriptTypeClassic = 0,
  kScriptTypeModule,
  kScriptTypeExtension,
  kScriptTypeUnknown
} ScriptType;
std::string script_type_to_string(const ScriptType type) noexcept;

enum NodeType {
  kElementNode = 1,
  kAttributeNode = 2,
  kTextNode = 3,
  kCdataSectionNode = 4,
  kProcessingInstructionNode = 7,
  kCommentNode = 8,
  kDocumentNode = 9,
  kDocumentTypeNode = 10,
  kDocumentFragmentNode = 11,
};

typedef unsigned SourceCodeHash;
typedef unsigned UrlHash;
typedef uint64_t ScriptId;
typedef uint64_t DOMNodeId;
typedef uint64_t PageGraphId;
typedef std::string MethodName;

}  // namespace brave_page_graph

#endif  // BRAVE_COMPONENTS_BRAVE_PAGE_GRAPH_TYPES_H_
