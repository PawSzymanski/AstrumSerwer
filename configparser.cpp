/*
 *  configparser.cpp
 *
 *  Created: 2017-07-08
 *   Author: Patryk Wojtanowski
 */

#include "configparser.h"

ConfigParser::ConfigParser()
    : line(0), word(0), eol(false), eos(false)
{


}

bool ConfigParser::load(std::string dir)
{
    std::string word_str, section;
    std::fstream plik;
    std::vector<std::string> line_str;

    eol = false;
    eos = false;
    line = 0;
    word = 0;
    file_dir = dir;
    dane.clear();

    plik.open(dir,std::ios::in);
    if(!plik.good())
    {
        std::cout<<"nie można otworzyć pliku ["<<dir<<"]"<<std::endl;
        return false;
    }

    while(true)
    {
        plik>>word_str;
        if(plik.eof())
            break;

        if(word_str[0] == '@')
        {
            section = word_str.substr(1,word_str.size()-1);
            line_str.clear();
        }
        else if(word_str[word_str.size()-1] == ';' )
        {
            if(word_str.size()-1 != 0)
                line_str.push_back(word_str.substr(0,word_str.size()-1));
            dane[section].push_back(line_str);
            line_str.clear();
        }
        else
            line_str.push_back(word_str);
    }

    plik.close();

    return true;
}

bool ConfigParser::setSection(std::string s)
{
    if(dane.count(s) == 0)
    {
        std::cout<<"nie ma sekcji \""<<s<<"\" w pliku "<<file_dir<< std::endl;
        return false;
    }
    section = s;
    eos = false;
    eol = false;
    line = 0;
    word = 0;
    return true;
}

float ConfigParser::getFloat()
{
    std::string s = dane[section][line][word];
    std::stringstream ss(s);
    float l;
    ss>>l;
    next();
    return l;
}

std::string ConfigParser::getString()
{
    //std::cout << dane[section][line][word];
    std::string s = dane[section][line][word];
    next();
    return s;
}

bool ConfigParser::EndOfLine()
{
    return eol;
}

bool ConfigParser::EndOfSection()
{
    return eos;
}

std::vector<std::vector<std::string> > &ConfigParser::getData()
{
    return dane[section];
}

std::vector<std::string> ConfigParser::getSections()
{
    std::vector <std::string> sections;
    for(auto p : dane)
        sections.push_back(p.first);

    return sections;
}

void ConfigParser::next()
{
    ++word;
    if(word >= dane[section][line].size())
    {
        eol = true;
        word =0;
        ++line;
        if(line >= dane[section].size())
        {
            line = 0;
            eos = true;
        }
        else
            eos = false;
    }
    else eol = false;
}
