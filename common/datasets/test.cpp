#include <iostream>
#include <fstream>
#include <sstream>
#include "ODDatasets.h"
#include "PascalVOC.h"
#include "MSCoco.h"
#include "ImageNet.h"
#include "lmdb.h"
#include "caffe/util/db.hpp"

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

#include <algorithm>
#include <fstream>  // NOLINT(readability/streams)
#include <string>
#include <utility>
#include <vector>

#include "boost/scoped_ptr.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include "caffe/util/format.hpp"
#include "caffe/util/io.hpp"
#include "caffe/util/rng.hpp"

using namespace std;
using namespace caffe;

bool ConvertLabelToDatum(od::ODAnnotation an, Datum* datum) {
    vector<od::ODObject> objects = an.objects_;
    //datum->set_width(5*objects.size());
    cout << objects.size() << " " << endl;
    //datum->set_height(1);
    //datum->set_channels(1);
    datum->clear_float_data();
    google::protobuf::RepeatedField<float>* datumFloatData = datum->mutable_float_data();
    for (int i = 0; i < objects.size(); i++) {
        datumFloatData->Add(objects[i].label_);
        datumFloatData->Add(objects[i].bbox_[0]);
        datumFloatData->Add(objects[i].bbox_[1]);
        datumFloatData->Add(objects[i].bbox_[2]);
        datumFloatData->Add(objects[i].bbox_[3]);
    }
    cout << datumFloatData->size() << endl;
    datum->set_label(0);
    return true;
}

int main() {

    cout << "Loading Pascal VOC dataset ..." << endl;
    string p1 = "/home/amax/cxt/data/pascal_voc/";
    //string p1 = "../data/pascal_voc/";
    od::PascalVOC pascal = od::PascalVOC(p1, 1);
    cout << "Dataset name: " << pascal.getDatasetName() << endl;
    cout << "Number of categories: " << pascal.getNumOfClasses() << endl;
    cout << "Number of classlist: " << pascal.getClassesList().size() << endl;
    cout << "Number of training images: " << pascal.getTrainImageList().size() << endl;
    cout << "Number of validation images: " << pascal.getValImageList().size() << endl;
    cout << "Number of trainging and validation images: " << pascal.getTrainvalImageList().size() << endl;
    cout << "Number of test images: " << pascal.getTestImageList().size() << endl;
    cout << "Number of annotated images: " << pascal.getAllAnnotations().size() << endl;
    cout << "Annotation of image 000005.jpg: " << endl;
    od::ODAnnotation a1 = pascal.getAnnotationByName("000005");
    cout << a1 << endl;
    cout << endl;

    vector<string> train_list = pascal.getTrainImageList();
    map<string, od::ODAnnotation> anns = pascal.getAllAnnotations();

    pascal.convertDatasetToLmdb("test", "/media/amax/cxt/data/Detection/lmdb/pascal_test");


    /*
    cout << "Loading Tiny-imagenet dataset ..." << endl;
    //string p2 = "/home/amax/cxt/data/tiny-imagenet-200/";
    string p2 = "../data/tiny-imagenet-200/";
    od::ImageNet imagenet = od::ImageNet(p2, 0);
    cout << "Dataset name: " << imagenet.getDatasetName() << endl;
    cout << "Number of categories: " << imagenet.getNumOfClasses() << endl;
    cout << "Number of classlist: " << imagenet.getClassesList().size() << endl;
    cout << "Number of training images: " << imagenet.getTrainImageList().size() << endl;
    cout << "Number of validation images: " << imagenet.getValImageList().size() << endl;
    cout << "Number of trainging and validation images: " << imagenet.getTrainvalImageList().size() << endl;
    cout << "Number of test images: " << imagenet.getTestImageList().size() << endl;
    cout << "Number of annotated images: " << imagenet.getAllAnnotations().size() << endl;
    cout << "Annotation of image val_0.JPEG: " << endl;
    od::ODAnnotation a2 = imagenet.getAnnotationByName("val_0.JPEG");
    cout << a2 << endl;
    */
    
    /*
    map<int, string> cates = imagenet.getClassesList();
    for (map<int, string>::iterator it = cates.begin(); it != cates.end(); it++)
        cout << it->first << " "  << it->second << endl;
    cout << endl;

    imagenet.convertDatasetToLmdb("train", "/media/amax/cxt/data/Detection/lmdb/imagenet/train_lmdb_256_shuffled", 256, 256);
    imagenet.convertDatasetToLmdb("train", "./train_lmdb");
    */


    /*
    //  the json file is too large to push to github.
    //string p3 = "/media/amax/Seagate Backup Plus Drive/cxt/data/MSCOCO/";
    string p3 = "./data/MSCOCO/";
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
    od::ODAnnotation a3 = coco.getAnnotationByName("233116");
    cout << a3 << endl;
    cout << endl;
    */

    /*
  const bool is_color = true;
  const bool check_size = false;
  const bool encoded = false;
  const string encode_type = "";

  int resize_height = 256;
  int resize_width = 256;

  string save_dir = "./pascal_train_label_lmdb";

  //if (FLAGS_shuffle) {
    // randomly shuffle data
    //LOG(INFO) << "Shuffling data";
    //shuffle(lines.begin(), lines.end());
  //}
  //LOG(INFO) << "A total of " << lines.size() << " images.";

  if (encode_type.size() && !encoded)
    LOG(INFO) << "encode_type specified, assuming encoded=true.";

  

  // Create new DB
  boost::scoped_ptr<db::DB> db(db::GetDB("lmdb"));
  db->Open(save_dir, db::NEW);
  boost::scoped_ptr<db::Transaction> txn(db->NewTransaction());

  // Storing to db
  std::string root_folder("./");
  std::string img_path = p1 + "/VOCTrain/VOC2007/JPEGImages/";
  Datum datum;
  int count = 0;
  int data_size = 0;
  bool data_size_initialized = false;

  //for (int line_id = 0; line_id < lines.size(); ++line_id) {
  for (int i = 0; i < train_list.size(); i++) {
    bool status;
    std::string enc = encode_type;
    //cout << train_list[i] << endl;
    if (encoded && !enc.size()) {
      // Guess the encoding type from the file name
      string fn = train_list[i];
      size_t p = fn.rfind('.');
      if ( p == fn.npos )
        LOG(WARNING) << "Failed to guess the encoding of '" << fn << "'";
      enc = fn.substr(p);
      std::transform(enc.begin(), enc.end(), enc.begin(), ::tolower);
    }

    cout << img_path + train_list[i] + '.' + enc << endl;
    //status = ReadImageToDatum(img_path + train_list[i] + ".jpg",
        //0, resize_height, resize_width, is_color,
        //enc, &datum);

    //cout << datum.channels() << " " << datum.height() << " " << datum.width() <<  " " << datum.data().size() << endl;
    // load label to datum
    od::ODAnnotation an = anns[train_list[i]];
    status = ConvertLabelToDatum(an, &datum);

    if (status == false) continue;
    if (check_size) {
      if (!data_size_initialized) {
        data_size = datum.channels() * datum.height() * datum.width();
        data_size_initialized = true;
        cout << "size of first data: " << data_size << endl;
      } else {
        const std::string& data = datum.data();
        CHECK_EQ(data.size(), data_size) << "Incorrect data field size "
            << data.size();
      }
    }
    // sequential
    //string key_str = caffe::format_int(line_id, 8) + "_" + lines[line_id].first;
    string key_str = train_list[i];

    // Put in db
    string out;
    CHECK(datum.SerializeToString(&out));
    txn->Put(key_str, out);

    if (++count % 100 == 0) {
      // Commit db
      txn->Commit();
      txn.reset(db->NewTransaction());
      LOG(INFO) << "Processed " << count << " files.";
    }
  }
  // write the last batch
  if (count % 100 != 0) {
    txn->Commit();
    LOG(INFO) << "Processed " << count << " files.";
  }
  */

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
