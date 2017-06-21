#include <iostream>
#include "ODDatasets.h"
#include "PascalVOC.h"


/*
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "boost/filesystem.hpp"

using namespace rapidxml;
using namespace boost::filesystem;
*/
using namespace std;

int main() {

    /*
    int bbox[] = {1, 2, 3, 4};
    string name = "cat", pose = "back";
    od::ODObject obj = od::ODObject(name, pose, 0, 0, bbox);
    std::vector<od::ODObject> objs;
    objs.push_back(obj);
    od::ODAnnotation a = od::ODAnnotation(string("0000.jpg"), 224, 224, 3, objs);

    map<string, od::ODAnnotation> M;
    M["0000.jpg"] = a;
    cout << M["0000.jpg"].filename_ << endl;
    cout << M["0000.jpg"].width_ << endl;
    cout << M["0000.jpg"].objects_[0].class_name_ << endl;
    */

    string p1 = "", p2 = "";
    od::PascalVOC pascal = od::PascalVOC(0, p1, p2);

    /*
    path p("./");
    directory_iterator end_itr;
    for (directory_iterator itr(p); itr != end_itr; ++itr)
    {
        if (is_regular_file(itr->path())) 
        {
            string current_file = itr->path().string();
            cout << current_file << endl;
        }
    }
    */

    /*
    file<> fdoc("/home/amax/cxt/data/pascal_voc/VOCTrain/VOC2007/Annotations/002208.xml");
    xml_document<> doc;
    doc.parse<0>(fdoc.data());

    xml_node<>* root = doc.first_node();
    cout << root->name() << endl;


    for (xml_node<>* node = root->first_node("object"); node; node = node->next_sibling()) {
        cout << node->name() << " " << node->value() << endl;
        if (node->name() == string("object")) {
            cout << "yes" << endl;
            cout << node->first_node("pose")->value() << endl;
        }
    }
    */
    return 0;
}
