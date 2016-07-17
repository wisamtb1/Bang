#ifndef STRINGUTILS_H
#define STRINGUTILS_H

#include <string>
#include <algorithm>

class StringUtils
{
private:
    StringUtils() {}

public:

    /**
     * @brief AddInFrontOfWords
     * Adds the passed particle in front of every word.
     * For example: ("-I", "file1 dir/file2 dir/file3") is converted to
     * "-Ifile1 -Idir/file2 -Idir/file3"
     */
    static void AddInFrontOfWords(std::string particle, std::string *str);
    static void RemoveLineBreaks(std::string *str);
};

#endif // STRINGUTILS_H