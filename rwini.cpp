//
//  rwini.cpp
//  TeeGL
//
//  Created by daniel grenehed on 4/4/17.
//  Copyright © 2017 daniel grenehed. All rights reserved.
//

#include <string>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include "rwini.hpp"

/*
 
 inits module, returns true if the given filename is an .ini file
 
 */
bool RWIni::init(std::string filename) {
    this->filename = filename;
    
    std::string ini = std::string(".ini");
    if (filename.length() >= ini.length()) {
        return (0 == filename.compare(filename.length() - ini.length(), filename.length(), ini));
    }
    return false;
}

/*
 
 searches the ini file for key in section and returns the value of the key as a result
 
 */
std::string RWIni::getValue(std::string section, std::string key) {
    std::string tmp, result="", ls, sec = std::string("[") + toLower(section) + std::string("]");
    std::ifstream file(this->filename.c_str());
    if (!file | !file.is_open()) return "";
    bool right_section = false;
    while (std::getline(file, ls)) {
        
        if (isComment(ls)) continue;
        
        if (right_section) {
            if (isSection(ls)) right_section = false;
            else {
                tmp = value_for_key(ls, trim(toLower(key)));
                if (tmp != "") result = tmp;
            }
        } else {
            if (trim(toLower(ls)) == sec) {
                right_section = true;
                continue;
            }
        }
    }
    
    file.close();
    return result;
}

/*
 
 tries to get the value of the key in section and sets result to value, returns false if it failes to find value for key
 
 */
bool RWIni::getValue_as_String(std::string section, std::string key, std::string & result) {
    std::string val = getValue(section, key);
    if (val == std::string("")) return false;
    result = val;
    return true;
}

/*
 
  tries to get the value of the key in section and sets result to value, returns false if it failes to find value for key or cannot convert value to int
 
 */
bool RWIni::getValue_as_Int(std::string section, std::string key, int & result) {
    std::string val = getValue(section, key);
    if (val == std::string("")) return false;
    result = std::stoi(val);
    if (!result) return false;
    return true;
}

/*
 
  tries to get the value of the key in section and sets result to value, returns false if it failes to find value for key or cannot convert value to long
 
 */
bool RWIni::getValue_as_Long(std::string section, std::string key, long & result) {
    std::string val = getValue(section, key);
    if (val == std::string("")) return false;
    result = std::stol(val);
    if (!result) return false;
    return true;
}

/*
 
  tries to get the value of the key in section and sets result to value, returns false if it failes to find value for key or cannot convert value to float
 
 */
bool RWIni::getValue_as_Float(std::string section, std::string key, float & result) {
    std::string val = getValue(section, key);
    if (val == std::string("")) return false;
    result = std::stof(val);
    if (!result) return false;
    return true;
}

/*
 
  tries to get the value of the key in section and sets result to value, returns false if it failes to find value for key or cannot convert value to double
 
 */
bool RWIni::getValue_as_Double(std::string section, std::string key, double & result) {
    std::string val = getValue(section, key);
    if (val == std::string("")) return false;
    result = std::stod(val);
    if (!result) return false;
    return true;
}

/*
 
  tries to get the value of the key in section and sets result to value, returns false if it failes to find value for key or cannot convert value to bool
 
 */
bool RWIni::getValue_as_Bool(std::string section, std::string key, bool & result) {
    std::string val = getValue(section, key);
    if (val == std::string("")) return false;
    size_t first = getFirstChar(val);
    if (trim(toLower(val.substr(first, first+4))) == std::string("true")) {
        result = true;
        return true;
    } else if (trim(toLower(val.substr(first, first+5))) == std::string("false")) {
        result = false;
        return true;
    }
    return false;
}












/*
 
 Opens the ini file, searches for the key and sets its value to value
 if key isn't found, it adds the key=value to the end of the section
 if section isn't found, it adds the section to the end of the file and adds key=value the next line.
 
 */
bool RWIni::setValue(std::string section, std::string key, std::string value) {
    // set value of key in section in ini-file
    std::string ls, sec = std::string("[") + toLower(section) + std::string("]");
    
     std::vector<std::string> buffr;
    
    if (iniExists()) { // ini exists, read and modify content
    
        std::ifstream file(this->filename.c_str());
        if (!file) return false;
    
   
        bool right_section = false;
        bool value_set = false;
        while (std::getline(file, ls)) {
        
        // find section
        
            if (isComment(ls)) {
           
            
            } else if (right_section) {
            // check if its new section
                if (isSection(ls)) {
                    if (!value_set) {
                        // append key=value
                        buffr.push_back(key+"="+value);
                        value_set = true;
                    }
                
                } else {
                    // check if line starts with key=
                    if (isKey(ls, key) && !value_set) {// key found, replace line with key=value
                        buffr.push_back(key+"="+value);
                        value_set = true;
                        continue;
                    }
                }
            
            } else {
                if (trim(toLower(ls)) == sec) {
                    right_section = true;
                }
            }
        
            buffr.push_back(ls);
        }
    
        file.close();
    
        if (!value_set) {
            if (right_section) {
                buffr.push_back(key+"="+value);
            }
            else {
                buffr.push_back(sec);
                buffr.push_back(key+"="+value);
            }
        }
    } else { // ini does not exit, create content and file
        buffr.push_back(sec);
        buffr.push_back(key+"="+value);
    }
    // write buffr to file
    
    std::ofstream wfile(this->filename);
    //wfile.open(this->filename, std::ofstream::out | std::ofstream::trunc); //open and discard all text in the file
    
    if (!wfile) return false;
    
    for (size_t i =0;  i < buffr.size(); i++) {
        wfile << buffr.at(i) << '\n';
    }
    
    wfile.close();
    
    return true;
}

/*
 
 test if able to access the file
 
 */
bool RWIni::iniExists() {
    std::ifstream f(this->filename);
    if (!f) return false;
    f.close();
    return true;
}


/*
 
 returns the filepath of the RWIni object
 
 */
std::string RWIni::getIniFilename() {
    return this->filename;
}




///////////////////////////////////////////////////////////////////////////////
// private methods
///////////////////////////////////////////////////////////////////////////////







/*
 
 returns the location of the first character that is not space of string
 
 */
size_t RWIni::getFirstChar(const std::string & str) {
    return str.find_first_not_of(' ');
}

/**/
bool RWIni::isKey(const std::string & s, const std::string & key) {
    size_t start = getFirstChar(s);
    if (trim(toLower(s.substr(start, start+key.length()))) != key) return false;
    // first word is equal to key
    
    size_t p =  s.find('=');
    size_t fnos = s.find_first_not_of(' ', start+key.length());
    
    if (p == std::string::npos || p != fnos) {
        return false;
    }
    return true;
}

/* 
 
 returns true if the given string is a section
 
 */
bool RWIni::isSection(const std::string &s) {
    size_t f = s.find_first_not_of(' ');
    size_t o = s.find(std::string("["));
    size_t c = s.find(std::string("]"));
    if (f == o && o < c) // only whitespace before [ and ] is after [
        return true;
    return false;
}

/*
 
 returns true if the given string is a section
 
 */
bool RWIni::isComment(const std::string &s) {
    size_t hc = s.find(std::string("#"));
    size_t cc = s.find(std::string(";"));
    size_t f = s.find_first_not_of(' ');
    if (hc == f || cc == f) { // first character is either # or ; and is therefore a coomment
        return true;
    }
    return false;
}

/*
 
 returns value for key if it exists else it returns ""
 
 */
std::string RWIni::value_for_key(const std::string & s, const std::string key) {
    size_t start = getFirstChar(s);
    if (trim(toLower(s.substr(start, start+key.length()))) != key) return "";
    // first word is equal to key
    
    size_t p =  s.find('=');
    size_t fnos = s.find_first_not_of(' ', start+key.length());
    
    if (p == std::string::npos || p != fnos) {
        return "";
    }
    
    p = s.find_first_not_of(" \t\n\r\0", p+1);
    return s.substr(p);
}

/*
 
 removes some characters
 
 */
std::string RWIni::trim(std::string s) {
    size_t l = s.length();
    if (l == 0) { // string s has size of 0
        return s;
    }
    size_t b = s.find_first_not_of(" \t\r\n\0");
    if (b == std::string::npos) {
        b = 0;
    }
    size_t e = s.find_last_not_of(" \t\r\n\0");
    if (e == std::string::npos) {
        return s.substr(b);
    }
    return s.substr(b, e-b+1);
}

/*
 
 returns the same string with all characters as lowercase
 
 */
std::string RWIni::toLower(const std::string & s) {
    std::string str = s;
    for (std::string::iterator i = str.begin(); i != str.end(); i++) {
        if (tolower(*i) != (int)*i) *i = (char)tolower(*i);
    }
    return str;
}





