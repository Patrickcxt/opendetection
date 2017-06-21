#include <iostream>
//#include "ODDatasets.h"
//#include "PascalVOC.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"

#include "boost/filesystem.hpp"

using namespace std;
using namespace rapidxml;
using namespace boost::filesystem;
int main() {

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
    string p1 = "", p2 = "";
    od::PascalVOC pascal = od::PascalVOC(0, p1, p2);
    */


    file<> fdoc("/home/amax/cxt/data/pascal_voc/VOCTrain/VOC2007/Annotations/002208.xml");
    xml_document<> doc;
    doc.parse<0>(fdoc.data());

    xml_node<>* root = doc.first_node();
    cout << root->name() << endl;

    //xml_node<>* node = root->first_node("object");
    //cout << node->name() << endl;

    for (xml_node<>* node = root->first_node("object"); node; node = node->next_sibling()) {
        cout << node->name() << " " << node->value() << endl;
        if (node->name() == string("object")) {
            cout << "yes" << endl;
            cout << node->first_node("pose")->value() << endl;
        }
    }
    return 0;
}
