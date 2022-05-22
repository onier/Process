//
// Created by ubuntu on 5/21/22.
//

#include "xercesc/util/PlatformUtils.hpp"
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/framework/LocalFileFormatTarget.hpp>
#include <xercesc/framework/MemBufInputSource.hpp>
#include <xercesc/framework/MemBufFormatTarget.hpp>
#include <xercesc/dom/DOMNodeFilter.hpp>
#include <xercesc/util/XMLUni.hpp>
#include <xercesc/util/PlatformUtils.hpp>
#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/dom/DOMNamedNodeMap.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMImplementationRegistry.hpp>
#include <xercesc/dom/DOMImplementation.hpp>
#include <xercesc/dom/DOMLSSerializer.hpp>
#include <xercesc/dom/DOMLSOutput.hpp>
#include "Library.h"
#include "XML.h"

int main() {
    puppy::common::library::loadDefaultLibrary();
    auto startTaskShapeType = rttr::type::get_by_name("StartTaskShape");
    if (startTaskShapeType.is_valid()) {
        LOG(INFO) << "valid task";
    }
    auto taskVariant = startTaskShapeType.create();
    LOG(INFO) << taskVariant.get_type().is_wrapper() << " "
              << taskVariant.get_type().get_wrapped_type().get_raw_type().get_name();
    xercesc::XMLPlatformUtils::Initialize();
    xercesc::DOMImplementation *domImplementation =
            xercesc::DOMImplementationRegistry::getDOMImplementation(XStr("Core"));
    std::shared_ptr<xercesc::DOMLSSerializer> serializer = std::shared_ptr<xercesc::DOMLSSerializer>(
            domImplementation->createLSSerializer());
    auto config = serializer->getDomConfig();
    config->setParameter(xercesc::XMLUni::fgDOMWRTFormatPrettyPrint, true);
    auto out = domImplementation->createLSOutput();
    std::shared_ptr<xercesc::MemBufFormatTarget> formatTarget = std::shared_ptr<xercesc::MemBufFormatTarget>(
            new xercesc::MemBufFormatTarget());
    out->setByteStream(formatTarget.get());
    std::shared_ptr<xercesc::DOMDocument> document = std::shared_ptr<xercesc::DOMDocument>(
            domImplementation->createDocument(0, XStr(
                    startTaskShapeType.get_name().data()), 0));
    auto robotElem = document->getDocumentElement();
    puppy::common::XML::createElement(taskVariant, robotElem, document.get());
    document->normalizeDocument();
    serializer->write(document.get(), out);
    LOG(INFO) << std::string(reinterpret_cast<const char *>(formatTarget->getRawBuffer()));
//    rttr::variant variant = rttr::type::get_by_name("StartTaskShape").create();
//    LOG(INFO) << puppy::common::XML::toXMLString(variant);
    LOG(INFO) << taskVariant.get_type().get_name();

    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\" ?>\n"
                      "<StartTaskShape R=\"11\" xmlns=\"\">\n"
                      "\n"
                      "  <center X=\"2\" Y=\"3\"/>\n"
                      "\n"
                      "</StartTaskShape>";

    const std::vector<rttr::variant> vars = puppy::common::XML::parseXML(xml);
    LOG(INFO) << vars.size();
    auto var = vars[0];
    LOG(INFO) << puppy::common::XML::toXMLString(var);
}