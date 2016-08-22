#include "StringUtils.h"

const String StringUtils::TOKEN_SPACE = " \t\n";

bool StringUtils::Contains(const String &str, const String &whatToFind)
{
    return str.find(whatToFind.ToCString(), 0) != String::npos;
}

float StringUtils::ToFloat(const String &str, bool *ok)
{
    String number = str;
    StringUtils::Trim(&number);
    std::istringstream iss(number);
    float v;
    iss >> v;
    if (ok) *ok = !iss.fail();
    return v;
}

int StringUtils::ToInt(const String &str, bool *ok)
{
    String number = str;
    StringUtils::Trim(&number);
    std::istringstream iss(number);
    int
            v;
    iss >> v;
    if (ok) *ok = !iss.fail();
    return v;
}

String StringUtils::FromFloat(float f, int precision)
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(precision) << f;
    return oss.str();
}

void StringUtils::TrimLeft(String *str)
{
    if(str->Empty())
    {
        return;
    }

    int i = 0;
    for (; i < str->Length(); ++i)
    {
        if (str->at(i) != ' ' && str->at(i) != '\t') break;
    }
    *str = i == str->Length() ? "" : str->substr(i, str->Length() - i);
}

void StringUtils::TrimRight(String *str)
{
    if(str->Empty())
    {
        return;
    }

    int i = str->Length() - 1;
    for (; i >= 0; --i)
    {
        if (str->at(i) != ' ' && str->at(i) != '\t') break;
    }
    *str = i < 0 ? "" : str->substr(0, i+1);
}

void StringUtils::Trim(String *str)
{
    StringUtils::TrimLeft(str);
    StringUtils::TrimRight(str);
}

String StringUtils::Trim(const String &str)
{
    String strCpy = str;
    Trim(&strCpy);
    return str;
}

String StringUtils::Concat(const std::vector<String> &particles, String joiner)
{
    String result = "";
    for (int i = 0; i < particles.size(); ++i)
    {
        if (i != 0) { result += joiner; }
        result += particles[i];
    }
    return result;
}


std::vector<String> StringUtils::Split(const String &content, char splitter)
{
    std::vector<String> result;
    if(content == "")
    {
        return result; // Empty
    }

    bool lastParticle = false;
    int lastIndexFound = 0;
    while (!lastParticle)
    {
        int indexFound = content.find_first_of(splitter, lastIndexFound);
        if (indexFound == String::npos)
        {
            lastParticle = true;
            indexFound = content.length();
        }
        String particle = content.substr(lastIndexFound, indexFound - lastIndexFound);
        result.push_back(particle);
        lastIndexFound = indexFound + 1;
    }
    return result;
}

std::vector<String> StringUtils::SplitTrim(const String &content, char splitter)
{
    std::vector<String> result = Split(content, splitter);
    for (String &str : result)
    {
        Trim(&str);
    }
    return result;
}

std::vector<String> StringUtils::BangEnumVariadicStringToNamesArray(const String &_va_args_)
{
    // We receive something like "Wololo, Apple = 49, Pear=29, Lololo=2193, Banana,Sandwich, Monkey=32"
    // We want this vector: ["Wololo", "Apple", "Pear", "Lololo", "Banana", "Sandwich", "Monkey"]
    std::vector<String> result = SplitTrim(_va_args_, ',');
    for (String &str : result)
    {
        str = SplitTrim(str, '=')[0];
    }
    return result;
}

String StringUtils::FormatInspectorLabel(const String &labelString)
{
    String labelFormatted = labelString;
    StringUtils::Replace(&labelFormatted, "_", " ");

    // Add space after every cap (except for consecutive caps).
    for (auto it = labelFormatted.begin(); it != labelFormatted.end(); ++it)
    {
        ++it;
        bool nextOneIsCap = (it != labelFormatted.end()) && IsCap(*it);
        --it;
        if (!nextOneIsCap && IsCap(*it) && it != labelFormatted.begin())
        {
            it = labelFormatted.insert(it, ' ');
            ++it;
        }
    }

    StringUtils::Replace(&labelFormatted, "  ", " ");

    return labelFormatted;
}

bool StringUtils::IsNumber(char c)
{
    return c >= 48 && c <= 57;
}

bool StringUtils::IsLetter(char c)
{
    return (c >= 97 && c <= 122) || IsCap(c);
}

bool StringUtils::IsCap(char c)
{
    return c >= 65 && c <= 90;
}

void StringUtils::AddInFrontOfWords(String particle, String *str)
{
    String &phrase = *str;
    if (!phrase.Empty() && phrase[0] != ' ')
    {
        phrase.insert(0, particle);
    }

    for (int i = 0; i < phrase.Length() - 1; ++i)
    {
        if (phrase[i] == ' ' && phrase[i+1] != ' ')
        {
            phrase.insert(i+1, particle);
            i += 2; // Sorry
        }
    }
}

void StringUtils::RemoveLineBreaks(String *str)
{
    std::replace(str->begin(), str->end(), '\n', ' ');
}

void StringUtils::Replace(String *content, const String &toFind, const String &replaceWithThis)
{
    String::size_type i;
    while ( (i = content->find(toFind)) != String::npos)
    {
        content->replace(i, toFind.Length(), replaceWithThis);
    }
}
