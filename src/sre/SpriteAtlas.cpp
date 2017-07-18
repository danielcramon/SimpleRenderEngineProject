//
// Created by Morten Nobel-Jørgensen on 17/07/2017.
//

#include "sre/SpriteAtlas.hpp"
#include "sre/Texture.hpp"
#include "picojson.h"

#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <cerrno>
#include <iostream>
#include <glm/gtx/string_cast.hpp>

using namespace std;

sre::SpriteAtlas::SpriteAtlas(std::map<std::string, Sprite>&& sprites, std::shared_ptr<Texture> texture) {
    for (auto & s : sprites){
        this->sprites.insert({s.first,s.second});
    }
    this->texture = texture;
}

std::shared_ptr<sre::SpriteAtlas> sre::SpriteAtlas::create(std::string jsonFile, std::string imageFile) {
    picojson::value v;
    std::ifstream t(jsonFile);
    t >> v;
    std::string err = picojson::get_last_error();
    if (err != ""){
        cerr << err << endl;
        return {};
    }
    std::map<std::string, Sprite> sprites;
    picojson::array list = v.get("frames").get<picojson::array>();
    picojson::value meta = v.get("meta");
    auto texture = Texture::create().withFile(imageFile).build();
    for (picojson::value& spriteElement : list){
        string name = spriteElement.get("filename").get<string>();
        // "frame": {"x":154,"y":86,"w":92,"h":44},
        // "pivot": {"x":0.5,"y":0.5}
        int x = (int)spriteElement.get("frame").get("x").get<double>();
        int y = (int)spriteElement.get("frame").get("y").get<double>();

        int w = (int)spriteElement.get("frame").get("w").get<double>();
        int h = (int)spriteElement.get("frame").get("h").get<double>();
        y = texture->getHeight()-y-h;
        float px = (float)spriteElement.get("pivot").get("x").get<double>();
        float py = (float)spriteElement.get("pivot").get("y").get<double>();
        Sprite sprite({x,y},{w,h},{px,py},texture.get());
        sprites.emplace(std::pair<std::string, Sprite>(name, std::move(sprite)));
    }
    return std::shared_ptr<sre::SpriteAtlas>(new sre::SpriteAtlas(std::move(sprites), texture));
}

std::vector<std::string> sre::SpriteAtlas::getNames() {
    std::vector<std::string> res;
    for (auto & e : sprites){
        res.push_back(e.first);
    }
    return res;
}

sre::Sprite sre::SpriteAtlas::get(std::string name) {
    if (sprites.find(name) == sprites.end()){
        for (auto & s : sprites){
            cout << s.first<< " "<<(name == s.first)<<endl;
        }
        cout << "Not found "<<name<<" "<<sprites.size()<<endl;
    }
    sre::Sprite val = sprites[name];
    sre::Sprite& val2 = sprites[name];
    if (val.spriteSize.x==0){
        cout << "Val "<<glm::to_string(val.spriteSize)<<endl;
    }
    return val;
}