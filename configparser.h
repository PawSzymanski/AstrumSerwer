/*
 *  configparser.h
 *
 *  Created: 2017-07-08
 *   Author: Patryk Wojtanowski
 */

#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class ConfigParser
{
    std::map<std::string, std::vector<std::vector<std::string> > > dane;
    std::string section;
    std::string file_dir;
    unsigned int line, word;
    bool eol, eos;

public:
    ConfigParser();
    bool load(std::string dir);
    bool setSection(std::string s);
    float getFloat();
    std::string getString();
    bool EndOfLine();
    bool EndOfSection();
    std::vector< std::vector <std::string> > & getData();
    std::vector< std::string > getSections();

private:
    void next();
};

#endif // CONFIGPARSER_H
