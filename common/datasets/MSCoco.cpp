//
// Created by Xiongtao Chen on 23.06.2017
//

#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "MSCoco.h"

namespace od
{

  void MSCoco::loadDataset()
  {
    // Parse train_annotation_path
    std::ifstream input;
    input.open(this->train_annotation_path_.c_str());
    if (!input)
    {
      std::cerr << "error: unable to open file: " << this->train_annotation_path_ << std::endl; 
    }
    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string jsontext = buffer.str();

    this->train_doc_ = new rj::Document();
    this->train_doc_->Parse(jsontext.c_str());
    input.close();
    input.clear();
    buffer.str("");

    // Parse val_annotation_path
    input.open(this->val_annotation_path_.c_str());
    if (!input)
    {
      std::cerr << "error: unable to open file: " << this->val_annotation_path_ << std::endl; 
    }
    buffer << input.rdbuf();
    jsontext = buffer.str();

    this->val_doc_ = new rj::Document();
    this->val_doc_->Parse(jsontext.c_str());
    input.close();
    input.clear();
    buffer.str("");


    // Parse test_annotation_path
    this->test_doc_ = NULL;   
  }

  void MSCoco::loadClassList()
  {
    this->num_classes_ = 80;
    std::map<int, std::string> categories;
    rj::Value& cate = (*train_doc_)["categories"];
    std::cout << cate.Size() << std::endl;
    for (int i = 0; i < cate.Size(); i++)
    {
        int id = cate[i]["id"].GetInt();
        std::string class_name = cate[i]["name"].GetString();
        std::cout << id << " " << class_name;
        categories[id] = class_name; 
    }
    this->class_list_ = categories;
  }

  void MSCoco::loadImageLists()
  {

    std::vector<std::string> train_image_list_;
    std::vector<std::string> val_image_list_;
    std::vector<std::string> trainval_image_list_;
    std::vector<std::string> test_image_list_;

    // load image list for training set
    rj::Value& imgs = (*train_doc_)["images"];
    std::cout << imgs.Size() << std::endl;
    for (int i = 0; i < imgs.Size(); i++)
    {
      int id = imgs[i]["id"].GetInt();
      char buff[33]; sprintf(buff, "%d", id);
      std::string sid = std::string(buff);    // use the id as the short form of image name, save the store space.
      std::string file_name = imgs[i]["file_name"].GetString();  // indeed, file_name = xxxxxx_00..00id.jpg
      int width = imgs[i]["width"].GetInt();
      int height = imgs[i]["height"].GetInt();
      train_image_list_.push_back(sid);
      trainval_image_list_.push_back(sid);

      this->width_[id] = width; this->height_[id] = height;   // For annotation
    }

    // load image list for validation set
    imgs = (*val_doc_)["images"];
    std::cout << imgs.Size() << std::endl;
    for (int i = 0; i < imgs.Size(); i++)
    {
      int id = imgs[i]["id"].GetInt();
      char buff[33]; sprintf(buff, "%d", id);
      std::string sid = std::string(buff);
      std::string file_name = imgs[i]["file_name"].GetString();
      int width = imgs[i]["width"].GetInt();
      int height = imgs[i]["height"].GetInt();
      val_image_list_.push_back(sid);
      trainval_image_list_.push_back(sid);

      this->width_[id] = width; this->height_[id] = height;   // For annotation
    }

    // load image list for test set
    // The annotation file is not provided yet.
    // Maybe directly search all names from the test image folder.

    this->train_image_list_ = train_image_list_;
    this->val_image_list_ = val_image_list_;
    this->trainval_image_list_ = trainval_image_list_;
    this->test_image_list_ = test_image_list_;
  }

  void MSCoco::loadAnnotations()
  {

    std::map<int, std::vector<ODObject> > M;     // Store objects for each image
    //std::map<std::string, ODAnnotation> annotations;
 
    // load annotation of training set
    rj::Value& ann = (*train_doc_)["annotations"];
    std::cout << ann.Size() << std::endl;
    /*
    for (int i = 0; i < ann.Size(); i++)
    {
      int id = ann[i]["id"].GetInt();
      std::cout << id << " ";
      int image_id = ann[i]["image_id"].GetInt();
      int cate_id = ann[i]["category_id"].GetInt();
      std::cout << cate_id << " ";
      int xmin = ann[i]["bbox"][0].GetFloat();
      int ymin = ann[i]["bbox"][1].GetFloat();
      int xmax = ann[i]["bbox"][2].GetFloat();
      int ymax = ann[i]["bbox"][3].GetFloat();
      std::cout << xmin << std::endl;

      int bbox[] = {xmin, ymin, xmax, ymax};
      ODObject obj = ODObject(this->class_list_[cate_id-1], "", 0, 0, bbox);
      M[id].push_back(obj);
    }
    */

    // load annotation of validation set
    ann = (*val_doc_)["annotations"];
    std::cout << ann.Size() << std::endl;
    getchar();
    int cnt = 0;
    for (int i = 0; i < ann.Size(); i++)
    {
      std::cout << ++cnt << ": " << std::endl;
      int id = ann[i]["id"].GetInt();
      std::cout << id << " ";
      int image_id = ann[i]["image_id"].GetInt();
      int cate_id = ann[i]["category_id"].GetInt();
      std::cout << cate_id << " ";
      int xmin = ann[i]["bbox"][0].GetFloat();
      int ymin = ann[i]["bbox"][1].GetFloat();
      int xmax = ann[i]["bbox"][2].GetFloat();
      int ymax = ann[i]["bbox"][3].GetFloat();
      std::cout << xmin << std::endl;

      int bbox[] = {xmin, ymin, xmax, ymax};
      ODObject obj = ODObject(this->class_list_[cate_id-1], "", 0, 0, bbox);
      M[id].push_back(obj);
    }
    int count = 0;
    for (std::map<int, std::vector<ODObject> >::iterator it = M.begin(); it != M.end(); it++)
    {
      count += 1;
      int width = this->width_[it->first];
      int height = this->height_[it->first];
      char buff[33]; sprintf(buff, "%d", it->first);
      std::string file_name = std::string(buff);
      ODAnnotation annotation = ODAnnotation(file_name, width, height, 3, it->second);    
      this->annotations_[file_name] = annotation;
    }
    std::cout << count << std::endl;
    M.clear();
  }

  void MSCoco::evaluation() {}

}

