/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    Permission is hereby granted, free of charge, to any person obtaining
    a copy of this software and associated documentation files (the
    "Software"), to deal in the Software without restriction, including
    without limitation the rights to use, copy, modify, merge, publish,
    distribute, sublicense, and/or sell copies of the Software, and to
    permit persons to whom the Software is furnished to do so, subject to
    the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
    CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
    TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
    SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef KSYNTAXHIGHLIGHTING_FORMAT_P_H
#define KSYNTAXHIGHLIGHTING_FORMAT_P_H

#include "definitionref_p.h"
#include "textstyledata_p.h"
#include "theme.h"

#include <QSharedData>
#include <QString>
#include <QStringRef>

namespace KSyntaxHighlighting {

class FormatPrivate : public QSharedData
{
public:
    FormatPrivate() = default;
    static FormatPrivate* detachAndGet(Format &format);

    TextStyleData styleOverride(const Theme &theme) const;
    QStringRef readAttr(QXmlStreamReader &reader, QString attr);
    void load(QXmlStreamReader &reader);

    DefinitionRef definition;
    QString name;
    TextStyleData style;
    Theme::TextStyle defaultStyle = Theme::Normal;
    quint16 id = 0;
    bool spellCheck = true;
};

}

#endif
