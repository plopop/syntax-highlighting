/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef KSYNTAXHIGHLIGHTING_DEFINITION_H
#define KSYNTAXHIGHLIGHTING_DEFINITION_H

#include "ksyntaxhighlighting_export.h"

#include <QTypeInfo>
#include <QPair>

#include <memory>

class QChar;
class QString;
class QStringList;
template <typename T> class QVector;

namespace KSyntaxHighlighting {

class Context;
class Format;
class KeywordList;

class DefinitionData;

/**
 * Defines the insert position when commenting code.
 * @since 5.50
 * @see Definition::singleLineCommentPosition()
 */
enum class CommentPosition
{
    //! The comment marker is inserted at the beginning of a line at column 0
    StartOfLine = 0,
    //! The comment marker is inserted after leading whitespaces right befire
    //! the first non-whitespace character.
    AfterWhitespace
};

/**
 * Represents a syntax definition.
 *
 * @section def_intro Introduction to Definitions
 *
 * A Definition is the short term for a syntax highlighting definition. It
 * typically is defined in terms of an XML syntax highlighting file, containing
 * all information about a particular syntax highlighting. This includes the
 * highlighting of keywords, information about code folding regions, and
 * indentation preferences.
 *
 * @section def_info General Information
 *
 * Each Definition contains a non-translated unique name() and a section().
 * In addition, for putting this information e.g. into menus, the functions
 * translatedName() and translatedSection() are provided. However, if isHidden()
 * returns @e true, the Definition should not be visible in the UI. The location
 * of the Definition can be obtained through filePath(), which either is the
 * location on disk or a path to a compiled-in Qt resource.
 *
 * The supported files of a Definition are defined by the list of extensions(),
 * and additionally by the list of mimeTypes(). Note, that extensions() returns
 * wildcards that need to be matched against the filename of the file that
 * requires highlighting. If multiple Definition%s match the file, then the one
 * with higher priority() wins.
 *
 * @see Repository
 * @since 5.28
 */
class KSYNTAXHIGHLIGHTING_EXPORT Definition
{
public:
    /**
     * Default constructor, creating an empty (invalid) Definition instance.
     * isValid() for this instance returns @e false.
     *
     * Use the Repository instead to obtain valid instances.
     */
    Definition();

    /**
     * Copy constructor.
     * Both this definition as well as @p other share the Definition data.
     */
    Definition(const Definition &other);

    /**
     * Destructor.
     */
    ~Definition();

    /**
     * Assignment operator.
     * Both this definition as well as @p rhs share the Definition data.
     */
    Definition& operator=(const Definition &rhs);

    /**
     * Checks two definitions for equality.
     */
    bool operator==(const Definition &other) const;

    /**
     * Checks two definitions for inequality.
     */
    bool operator!=(const Definition &other) const;

    /** Checks whether this object refers to a valid syntax definition. */
    bool isValid() const;
    /** Returns the full path to the definition XML file containing
     *  the syntax definition. Note that this can be a path to QRC content.
     */
    QString filePath() const;

    /** Name of the syntax.
     *  Used for internal references, prefer translatedName() for display.
     */
    QString name() const;
    /** Translated name for display. */
    QString translatedName() const;
    /** The group this syntax definition belongs to.
     *  For display, consider translatedSection().
     */
    QString section() const;
    /** Translated group name for display. */
    QString translatedSection() const;
    /** Mime types associated with this syntax definition. */
    QVector<QString> mimeTypes() const;
    /**
     * File extensions associated with this syntax definition.
     * The returned list contains wildcards.
     */
    QVector<QString> extensions() const;
    /** Returns the definition version. */
    int version() const;
    /**
     * Returns the definition priority.
     * A Definition with higher priority wins over Definitions with lower priorities.
     */
    int priority() const;
    /** Returns @c true if this is an internal definition that should not be
     * displayed to the user.
     */
    bool isHidden() const;
    /** Generalized language style, used for indentation. */
    QString style() const;
    /** Indentation style to be used for this syntax. */
    QString indenter() const;
    /** Name and email of the author of this syntax definition. */
    QString author() const;
    /** License of this syntax definition. */
    QString license() const;

    /**
     * Returns whether the character @p c is a word delimiter.
     * A delimiter defines whether a characters is a word boundary. Internally,
     * delimiters are used for matching keyword lists. As example, typically the
     * dot '.' is a word delimiter. However, if you have a keyword in a keyword
     * list that contains a dot, you have to add the dot to the
     * @e weakDeliminator attribute of the @e general section in your
     * highlighting definition. Similarly, sometimes additional delimiters are
     * required, which can be specified in @e additionalDeliminator.
     *
     * Checking whether a character is a delimiter is useful for instance if
     * text is selected with double click. Typically, the whole word should be
     * selected in this case. Similarly to the example above, the dot '.'
     * usually acts as word delimiter. However, using this function you can
     * implement text selection in such a way that keyword lists are correctly
     * selected.
     *
     * @note By default, the list of delimiters contains the following
     *       characters: \\t !%&()*+,-./:;<=>?[\\]^{|}~
     *
     * @since 5.50
     * @see isWordWrapDelimiter()
     */
    bool isWordDelimiter(QChar c) const;

    /**
     * Returns whether it is safe to break a line at before the character @c.
     * This is useful when wrapping a line e.g. by applying static word wrap.
     *
     * As example, consider the LaTeX code
     * @code
     * \command1\command2
     * @endcode
     * Applying static word wrap could lead to the following code:
     * @code
     * \command1\
     * command2
     * @endcode
     * command2 without a leading backslash is invalid in LaTeX. If '\\' is set
     * as word wrap delimiter, isWordWrapDelimiter('\\') then returns true,
     * meaning that it is safe to break the line before @c. The resulting code
     * then would be
     * @code
     * \command1
     * \command2
     * @endcode
     *
     * @note By default, the word wrap delimiters are equal to the word
     *       delimiters in isWordDelimiter().
     *
     * @since 5.50
     * @see isWordDelimiter()
     */
    bool isWordWrapDelimiter(QChar c) const;

    /**
     * Returns whether indentation-based folding is enabled.
     * An example for indentation-based folding is Python.
     * When indentation-based folding is enabled, make sure to also check
     * foldingIgnoreList() for lines that should be treated as empty.
     *
     * @see foldingIgnoreList(), State::indentationBasedFoldingEnabled()
     */
    bool indentationBasedFoldingEnabled() const;

    /**
     * If indentationBasedFoldingEnabled() returns @c true, this function returns
     * a list of regular expressions that represent empty lines. That is, all
     * lines matching entirely one of the regular expressions should be treated
     * as empty lines when calculating the indentation-based folding ranges.
     *
     * @note This list is only of relevance, if indentationBasedFoldingEnabled()
     *       returns @c true.
     *
     * @see indentationBasedFoldingEnabled()
     */
    QStringList foldingIgnoreList() const;

    /**
     * Returns the section names of keywords.
     * @since 5.49
     * @see keywordList()
     */
    QStringList keywordLists() const;

    /**
     * Returns the list of keywords for the keyword list @p name.
     * @since 5.49
     * @see keywordLists()
     */
    QStringList keywordList(const QString& name) const;

    /**
     * Returns a list of all Format items used by this definition.
     * The order of the Format items equals the order of the itemDatas in the xml file.
     * @since 5.49
     */
    QVector<Format> formats() const;

    /**
     * Returns a list of Definitions that are referenced with the IncludeRules rule.
     * The returned list does not include this Definition. In case no other
     * Definitions are referenced via IncludeRules, the returned list is empty.
     *
     * @since 5.49
     */
    QVector<Definition> includedDefinitions() const;

    /**
     * Returns the marker that starts a single line comment.
     * For instance, in C++ the single line comment marker is "//".
     * @since 5.50
     * @see singleLineCommentPosition();
     */
    QString singleLineCommentMarker() const;

    /**
     * Returns the insert position of the comment marker for sinle line
     * comments.
     * @since 5.50
     * @see singleLineCommentMarker();
     */
    CommentPosition singleLineCommentPosition() const;

    /**
     * Returns the markers that start and end multiline comments.
     * For instance, in XML this is defined as "<!--" and "-->".
     * @since 5.50
     */
    QPair<QString, QString> multiLineCommentMarker() const;

private:
    friend class DefinitionData;
    friend class DefinitionRef;
    explicit Definition(const std::shared_ptr<DefinitionData> &dd);
    std::shared_ptr<DefinitionData> d;
};

}

Q_DECLARE_TYPEINFO(KSyntaxHighlighting::Definition, Q_MOVABLE_TYPE);

#endif
