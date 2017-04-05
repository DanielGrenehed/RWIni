//
//  rwini.hpp
//  TeeGL
//
//  Created by daniel grenehed on 4/4/17.
//  Copyright © 2017 daniel grenehed. All rights reserved.
//

#ifndef rwini_hpp
#define rwini_hpp

class RWIni {
public:
    bool init(std::string filename);
    
    std::string getValue(std::string section, std::string key);
    
    bool getValue_as_String(std::string section, std::string key, std::string & result);
    bool getValue_as_Int(std::string section, std::string key, int & result);
    bool getValue_as_Long(std::string section, std::string key, long & result);
    bool getValue_as_Float(std::string section, std::string key, float & result);
    bool getValue_as_Double(std::string section, std::string key, double & result);
    bool getValue_as_Bool(std::string section, std::string key, bool & result);
    
    bool setValue(std::string setion, std::string key, std::string value);
    bool iniExists();
    
    std::string getIniFilename();
private:
    size_t getFirstChar(const std::string & str);
    bool isKey(const std::string & s, const std::string & key);
    bool isSection(const std::string & s);
    bool isComment(const std::string & s);
    std::string value_for_key(const std::string & s, const std::string key);
    std::string trim(std::string s);
    std::string toLower(const std::string & s);
protected:
    std::string filename;
};

#endif /* rwini_hpp */
