#include <iostream>
#include <fstream>
#include <sstream>
#include "ODDatasets.h"
#include "PascalVOC.h"
#include "MSCoco.h"
#include "ImageNet.h"

/*
#include "rapidjson/document.h"
using namespace rapidjson;
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "boost/filesystem.hpp"

using namespace rapidxml;
using namespace boost::filesystem;
*/
using namespace std;

int main() {

    cout << "Loading Pascal VOC dataset ..." << endl;
    string p1 = "/home/amax/cxt/data/pascal_voc/";
    od::PascalVOC pascal = od::PascalVOC(p1, 0);
    cout << "Dataset name: " << pascal.getDatasetName() << endl;
    cout << "Number of categories: " << pascal.getNumOfClasses() << endl;
    cout << "Number of classlist: " << pascal.getClassesList().size() << endl;
    cout << "Number of training images: " << pascal.getTrainImageList().size() << endl;
    cout << "Number of validation images: " << pascal.getValImageList().size() << endl;
    cout << "Number of trainging and validation images: " << pascal.getTrainvalImageList().size() << endl;
    cout << "Number of test images: " << pascal.getTestImageList().size() << endl;
    cout << "Number of annotated images: " << pascal.getAllAnnotations().size() << endl;
    cout << "Annotation of image 000005.jpg: " << endl;
    od::ODAnnotation a = pascal.getAnnotationByName("000005");
    cout << a << endl;
    cout << endl;

    cout << "Loading Tiny-imagenet dataset ..." << endl;
    string p2 = "/home/amax/cxt/data/tiny-imagenet-200/";
    od::ImageNet imagenet = od::ImageNet(p2, 0);
    cout << "Dataset name: " << imagenet.getDatasetName() << endl;
    cout << "Number of categories: " << imagenet.getNumOfClasses() << endl;
    cout << "Number of classlist: " << imagenet.getClassesList().size() << endl;
    cout << "Number of training images: " << imagenet.getTrainImageList().size() << endl;
    cout << "Number of validation images: " << imagenet.getValImageList().size() << endl;
    cout << "Number of trainging and validation images: " << imagenet.getTrainvalImageList().size() << endl;
    cout << "Number of test images: " << imagenet.getTestImageList().size() << endl;
    cout << "Number of annotated images: " << imagenet.getAllAnnotations().size() << endl;
    cout << "Annotation of image val_6139.JPEG: " << endl;
    a = imagenet.getAnnotationByName("val_6139.JPEG");
    cout << a << endl;
    cout << endl;

    string p3 = "/media/amax/Seagate Backup Plus Drive/cxt/data/MSCOCO/";
    od::MSCoco coco = od::MSCoco(p3, 0);
    cout << "Dataset name: " << coco.getDatasetName() << endl;
    cout << "Number of categories: " << coco.getNumOfClasses() << endl;
    cout << "Number of classlist: " << coco.getClassesList().size() << endl;
    cout << "Number of training images: " << coco.getTrainImageList().size() << endl;
    cout << "Number of validation images: " << coco.getValImageList().size() << endl;
    cout << "Number of trainging and validation images: " << coco.getTrainvalImageList().size() << endl;
    cout << "Number of test images: " << coco.getTestImageList().size() << endl;
    cout << "Number of annotated images: " << coco.getAllAnnotations().size() << endl;
    cout << "Annotation of image 233116.jpg: " << endl;
    a = coco.getAnnotationByName("233116");
    cout << a << endl;
    cout << endl;

    /*
    static const char* kTypeNames[] = 
        { "Null", "False", "True", "Object", "Array", "String", "Number"};
    ifstream input;
    string json;
    string path = "/media/amax/Seagate Backup Plus Drive/cxt/data/MSCOCO/Annotations/instances/instances_val2014.json";
    input.open(path.c_str());
    if (!input) cout << "error" << endl;
    stringstream buffer;
    buffer << input.rdbuf();
    json = buffer.str();
    Document* document = new Document();
    document->Parse(json.c_str());
    cout << "=====================" << endl;
    for (Value::ConstMemberIterator it = document->MemberBegin(); it != document->MemberEnd(); ++it)
    {  
        cout << it->name.GetString() << endl;
        
    }
    Value& ann = (*document)["annotations"];
    cout << ann.Size() << endl;
    Value& cat = (*document)["categories"];
    cout << cat.Size() << endl;
    Value& img = (*document)["images"];
    cout << img.Size() << endl;

    getchar();
    for (int i = 0; i < ann.Size(); i++) {
        cout << ann[i]["id"].GetInt() << " ";
        cout << ann[i]["image_id"].GetInt() << " ";
        cout << ann[i]["category_id"].GetInt() << " ";
        cout << ann[i]["area"].GetFloat() << " [";
        cout << ann[i]["bbox"][0].GetFloat() << " ";
        cout << ann[i]["bbox"][1].GetFloat() << " ";
        cout << ann[i]["bbox"][2].GetFloat() << " ";
        cout << ann[i]["bbox"][3].GetFloat() << "]" << endl;
    }

    getchar();
    for (int i = 0; i < cat.Size(); i++) {
        cout << cat[i]["id"].GetInt() << " ";
        cout << cat[i]["name"].GetString() << " ";
        cout << cat[i]["supercategory"].GetString() << endl;
    }
    getchar();
    for (int i = 0; i < img.Size(); i++) {
        cout << img[i]["id"].GetInt() << " ";
        cout << img[i]["file_name"].GetString() << " ";
        cout << img[i]["width"].GetInt() << " ";
        cout << img[i]["height"].GetInt() << endl;
    }
    */



    /*
    int bbox[] = {1, 2, 3, 4};
    string name = "cat", pose = "back";
    od::ODObject obj = od::ODObject(name, pose, 0, 0, bbox);
    cout << obj << endl;
    std::vector<od::ODObject> objs;
    objs.push_back(obj);
    od::ODAnnotation a = od::ODAnnotation(string("0000.jpg"), 224, 224, 3, objs);

    cout << a << endl;

    map<string, od::ODAnnotation> M;
    M["0000.jpg"] = a;
    cout << M["0000.jpg"].filename_ << endl;
    cout << M["0000.jpg"].width_ << endl;
    cout << M["0000.jpg"].objects_[0].class_name_ << endl;
    */

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
