#include "DfsOpt_ProblemData.hpp"
#include "DfsOpt_XmlParser.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>
#include <vector>

#include <glibmm/ustring.h>
#include <libxml++/libxml++.h>

DfsOpt::InputData DfsOpt::XmlParser::parse(const std::string_view inFile) {
  xmlpp::DomParser schemaParser;
  schemaParser.parse_memory(XmlParser::schemaString().data());
  xmlpp::XsdValidator validator;
  validator.parse_document(schemaParser.get_document());

  xmlpp::DomParser inputParser;
  inputParser.parse_file(inFile.data());
  validator.validate(inputParser.get_document());

  auto toInt = [](const Glib::ustring& str) {
    return std::stoi(str.raw());
  };

  auto toDouble = [](const Glib::ustring& str) {
    return std::stod(str.raw());
  };

  auto parseReqs = [&](const xmlpp::Node* node, InputData& data) {
    for (const auto* child : node->get_children()) {
      const auto* elem = dynamic_cast<const xmlpp::Element*>(child);
      if (elem) {
        if (elem->get_name() == "budget") {
          data.budget_ = toInt(elem->get_child_text()->get_content());
        } else if (elem->get_name() == "num_qbs") {
          data.numQbs_ = toInt(elem->get_child_text()->get_content());
        } else if (elem->get_name() == "num_rbs") {
          data.numRbs_ = toInt(elem->get_child_text()->get_content());
        } else if (elem->get_name() == "num_wrs") {
          data.numWrs_ = toInt(elem->get_child_text()->get_content());
        } else if (elem->get_name() == "num_tes") {
          data.numTes_ = toInt(elem->get_child_text()->get_content());
        } else if (elem->get_name() == "num_flex") {
          data.numFlex_ = toInt(elem->get_child_text()->get_content());
        } else if (elem->get_name() == "num_dsts") {
          data.numDsts_ = toInt(elem->get_child_text()->get_content());
        }
      }
    }
  };

  auto parsePlayers = [&](const xmlpp::Node* node, std::vector<Player>& container) {
    for (const auto* child : node->get_children()) {
      const auto* elem = dynamic_cast<const xmlpp::Element*>(child);
      if (elem) {
        Player player{};
        player.name_ = elem->get_attribute("name")->get_value();
        player.team_ = elem->get_attribute("team")->get_value();
        player.salary_ = toInt(elem->get_attribute("cost")->get_value());
        player.expectedPts_ = toDouble(elem->get_attribute("pts")->get_value());
        container.push_back(player);
      }
    }
  };

  auto parseDsts = [&](const xmlpp::Node* node, std::vector<Dst>& container) {
    for (const auto* child : node->get_children()) {
      const auto* elem = dynamic_cast<const xmlpp::Element*>(child);
      if (elem) {
        Dst dst{};
        dst.team_ = elem->get_attribute("team")->get_value();
        dst.salary_ = toInt(elem->get_attribute("cost")->get_value());
        dst.expectedPts_ = toDouble(elem->get_attribute("pts")->get_value());
        container.push_back(dst);
      }
    }
  };

  InputData data{};
  const auto* root = inputParser.get_document()->get_root_node();
  for (const auto* node : root->get_children()) {
    if (node->get_name() == "requirements") {
      parseReqs(node, data);
    } else if (node->get_name() == "qbs") {
      parsePlayers(node, data.qbs_);
    } else if (node->get_name() == "rbs") {
      parsePlayers(node, data.rbs_);
    } else if (node->get_name() == "wrs") {
      parsePlayers(node, data.wrs_);
    } else if (node->get_name() == "tes") {
      parsePlayers(node, data.tes_);
    } else if (node->get_name() == "dsts") {
      parseDsts(node, data.dsts_);
    }
  }

  // A couple sanity checks
  if (data.qbs_.size() < data.numQbs_) {
    throw std::runtime_error("ERROR: Not enough QBs provided.");
  }
    if (data.rbs_.size() < data.numRbs_) {
    throw std::runtime_error("ERROR: Not enough RBs provided.");
  }
    if (data.wrs_.size() < data.numWrs_) {
    throw std::runtime_error("ERROR: Not enough WRs provided.");
  }
    if (data.tes_.size() < data.numTes_) {
    throw std::runtime_error("ERROR: Not enough TEs provided.");
  }
    if (data.rbs_.size() + data.wrs_.size() + data.tes_.size() <
      data.numRbs_ + data.numWrs_ + data.numTes_ + data.numFlex_) {
    throw std::runtime_error("ERROR: Not enough flex eligible players provided.");
  }
    if (data.dsts_.size() < data.numDsts_) {
    throw std::runtime_error("ERROR: Not enough DSTs provided.");
  }

  // Sort ascending by cost  to aid backtracking solution algorithm
  auto comp = [](const auto& l, const auto& r) {
    return l.salary_ < r.salary_;
  };

  std::sort(data.qbs_.begin(), data.qbs_.end(), comp);
  std::sort(data.rbs_.begin(), data.rbs_.end(), comp);
  std::sort(data.wrs_.begin(), data.wrs_.end(), comp);
  std::sort(data.tes_.begin(), data.tes_.end(), comp);
  std::sort(data.dsts_.begin(), data.dsts_.end(), comp);
  
  
  return data;
}

std::string_view DfsOpt::XmlParser::schemaString() {
  using namespace std::string_view_literals;
  return R"xsd(<?xml version="1.0" encoding="UTF-8" ?>
<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema">

<xs:simpleType name="stringType">
  <xs:restriction base="xs:string"/>
</xs:simpleType>

<xs:simpleType name="intType">
  <xs:restriction base="xs:nonNegativeInteger"/>
</xs:simpleType>

<xs:simpleType name="decType">
  <xs:restriction base="xs:decimal">
    <xs:minInclusive value="0.0"/>
  </xs:restriction>
</xs:simpleType>

<xs:complexType name="requirementsType">
  <xs:sequence>
    <xs:element name="budget" minOccurs="1" maxOccurs="1" type="intType"/>
    <xs:element name="num_qbs" minOccurs="0" maxOccurs="1" type="intType"/>
    <xs:element name="num_rbs" minOccurs="0" maxOccurs="1" type="intType"/>
    <xs:element name="num_wrs" minOccurs="0" maxOccurs="1" type="intType"/>
    <xs:element name="num_tes" minOccurs="0" maxOccurs="1" type="intType"/>
    <xs:element name="num_flex" minOccurs="0" maxOccurs="1" type="intType"/>
    <xs:element name="num_dsts" minOccurs="0" maxOccurs="1" type="intType"/>
  </xs:sequence>
</xs:complexType>

<xs:complexType name="playerType">
  <xs:attribute name="name" type="stringType" use="required"/>
  <xs:attribute name="team" type="stringType" use="required"/>
  <xs:attribute name="cost" type="intType" use="required"/>
  <xs:attribute name="pts" type="decType" use="required"/>
</xs:complexType>

<xs:complexType name="playerListType">
  <xs:sequence>
    <xs:element name="player" minOccurs="1" maxOccurs="unbounded" type="playerType"/>
  </xs:sequence>
</xs:complexType>

<xs:complexType name="dstType">
  <xs:attribute name="team" type="stringType" use="required"/>
  <xs:attribute name="cost" type="intType" use="required"/>
  <xs:attribute name="pts" type="decType" use="required"/>
</xs:complexType>

<xs:complexType name="dstListType">
  <xs:sequence>
    <xs:element name="dst" minOccurs="1" maxOccurs="unbounded" type="dstType"/>
  </xs:sequence>
</xs:complexType>

<xs:complexType name="dataType">
  <xs:sequence>
    <xs:element name="requirements" minOccurs="1" maxOccurs="1" type="requirementsType"/>
    <xs:element name="qbs" minOccurs="0" maxOccurs="1" type="playerListType"/>
    <xs:element name="rbs" minOccurs="0" maxOccurs="1" type="playerListType"/>
    <xs:element name="wrs" minOccurs="0" maxOccurs="1" type="playerListType"/>
    <xs:element name="tes" minOccurs="0" maxOccurs="1" type="playerListType"/>
    <xs:element name="dsts" minOccurs="0" maxOccurs="1" type="dstListType"/>
  </xs:sequence>
</xs:complexType>

<xs:element name="data" type="dataType"/>

</xs:schema>)xsd"sv;
}
