#ifndef DESIGNXML_H
#define DESIGNXML_H

#include "Design.h"
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QXmlStreamAttributes>

class DesignXML {
private:
    static void addStaticFilesToXMLWriter(QXmlStreamWriter &xmlWriter, Design &des);
    static void addConstraintFilesToXMLWriter(QXmlStreamWriter &xmlWriter, Design &des);
    static void addPartitionsToXMLWriter(QXmlStreamWriter &xmlWriter, Design &des);
    static void addModulesToXMLWriter(QXmlStreamWriter &xmlWriter, Design &des);
    static void addConfigurationsToXMLWriter(QXmlStreamWriter &xmlWriter, Design &des);

    static void readDesignAttributes(QXmlStreamAttributes &designAttrs, Design &des);
    static void readStaticFileAttributes(QXmlStreamAttributes &staticFileAttrs, Design &des);
    static void readConstraintFileAttributes(QXmlStreamAttributes &constraintFileAttrs, Design &des);
    static void readPartition(QXmlStreamReader &xmlReader, Design &des);
    static void readModule(QXmlStreamReader &xmlReader, Design &des);
    static void readConfiguration(QXmlStreamReader &xmlReader, Design &des);

public:
    static bool saveDesign(std::string outputFile, Design &des);
    static bool readDesign(std::string inputFile, Design &des);
};

#endif
