#ifndef XMLPARSER_H
#define XMLPARSER_H

#include <string>
#include <sstream>
#include <fstream>

#include "XMLNode.h"

class IFileable;
class XMLParser
{
private:
public:
    static std::map<std::string, const IFileable*> m_idToPointer;

    static void ClearPointerIds();

    static void GetNextOpenTag(const std::string &xml,
                               int startPosition,
                               std::string *tag,
                               int *beginPosition,
                               int *endTagPosition);

    static void GetNextTag(const std::string &xml,
                               int startPosition,
                               std::string *tag,
                               int *beginPosition,
                               int *endTagPosition);

    static void GetCorrespondingCloseTag(const std::string &xml,
                                         int startPositionAfterOpenTag,
                                         std::string tagName,
                                         int *beginPosition,
                                         int *endPosition);

    static void GetFirstAttribute(const std::string &tag,
                                  int startPosition,
                                  XMLAttribute *attribute,
                                  int *attributeEnd);

    static bool IsOpenTag(const std::string &tag);

public:
    XMLParser();

    static void RegisterId(const XMLNode *xmlInfo, const IFileable *pointer);
    static const IFileable* GetPointerFromId(const std::string &id);

    static std::string GetTagName(const std::string &tag,
                                  int *tagNameBegin = nullptr, int *tagNameEnd = nullptr);

    static XMLNode* FromFile(const std::string &filepath);
    static XMLNode* FromString(const std::string &xml);
};

#endif // XMLPARSER_H