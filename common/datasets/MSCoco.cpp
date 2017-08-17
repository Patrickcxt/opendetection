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

  void MSCoco::convertDatasetToLmdb(std::string subset, std::string save_dir, int resize_height, int resize_width)
  {
    std::cout << "Start converting MSCOCO dataset to lmdb ..." << std::endl;
    std::vector<std::string> image_list;
    std::string image_prefix = "";
    if (subset == "train")
    {
      image_list = this->train_image_list_;
      /*
      for (int i = 0; i < image_list.size(); i++)
          std::cout << image_list[i] << std::endl;
        */
      image_prefix = this->base_path_ + "Images/train2014/";
    }
    else if (subset == "val")
    {
      image_list = this->val_image_list_;
      /*
      for (int i = 0; i < image_list.size(); i++)
          std::cout << image_list[i] << std::endl;
          */
      image_prefix = this->base_path_ + "Images/val2014/";
    }
    else if (subset == "trainval")
    {
      std::cerr << "Not supported yet." << std::endl;
      return ;
    }
    else if (subset == "test")
    {
      std::cerr << "Not supported yet." << std::endl;
      return ;
    }
    else
    {
      std:: cerr << "Please specifiy correct subset among 'train', 'val', 'trainval' and 'test'."<< std::endl;
      return ;
    }
    convert_dataset_to_lmdb_detection(image_list, image_prefix, save_dir, resize_height, resize_width, "");
    std::cout << "Convert finished!" << std::endl;
  
  }
  void MSCoco::loadJsonDataset()
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
    std::map<int, std::string> categories;
    rj::Value& cate = (*train_doc_)["categories"];
    //std::cout << cate.Size() << std::endl;
    for (int i = 0; i < cate.Size(); i++)
    {
        int id = cate[i]["id"].GetInt();
        std::string class_name = cate[i]["name"].GetString();
        //std::cout << id << " " << class_name << std::endl;
        this->label2name_[id] = class_name; 
        this->name2label_[class_name] = id;
    }
    this->num_classes_ = this->label2name_.size();
  }

  void MSCoco::loadImageLists()
  {

    std::vector<std::string> train_image_list_;
    std::vector<std::string> val_image_list_;
    std::vector<std::string> trainval_image_list_;
    std::vector<std::string> test_image_list_;

    // load image list for training set
    rj::Value& imgs = (*train_doc_)["images"];
    //std::cout << imgs.Size() << std::endl;
    for (int i = 0; i < imgs.Size(); i++)
    {
      /*
      int id = imgs[i]["id"].GetInt();
      char buff[33]; sprintf(buff, "%d", id);
      std::string sid = std::string(buff);    // use the id as the short form of image name, save the store space.
      */
      std::string file_name = imgs[i]["file_name"].GetString();  // indeed, file_name = xxxxxx_00..00id.jpg
      int width = imgs[i]["width"].GetInt();
      int height = imgs[i]["height"].GetInt();
      //train_image_list_.push_back(sid);
      //trainval_image_list_.push_back(sid);
      train_image_list_.push_back(file_name);
      trainval_image_list_.push_back(file_name);

      //this->width_[id] = width; this->height_[id] = height;   // For annotation
      this->width_[file_name] = width; this->height_[file_name] = height;   // For annotation
    }

    // load image list for validation set
    imgs = (*val_doc_)["images"];
    //std::cout << imgs.Size() << std::endl;
    for (int i = 0; i < imgs.Size(); i++)
    {
      /*
      int id = imgs[i]["id"].GetInt();
      char buff[33]; sprintf(buff, "%d", id);
      std::string sid = std::string(buff);
      */
      std::string file_name = imgs[i]["file_name"].GetString();
      int width = imgs[i]["width"].GetInt();
      int height = imgs[i]["height"].GetInt();
      //val_image_list_.push_back(sid);
      //trainval_image_list_.push_back(sid);
      val_image_list_.push_back(file_name);
      trainval_image_list_.push_back(file_name);

      // this->width_[id] = width; this->height_[id] = height;   // For annotation
      this->width_[file_name] = width; this->height_[file_name] = height;   // For annotation
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

    //std::map<int, std::vector<ODObject> > M;     // Store objects for each image
    std::map<std::string, std::vector<ODObject> > M;     // Store objects for each image
 
    // load annotation of training set
    rj::Value& ann = (*train_doc_)["annotations"];
    //std::cout << ann.Size() << std::endl;
    for (int i = 0; i < ann.Size(); i++)
    {
      long long id = ann[i]["id"].GetFloat();
      int image_id = ann[i]["image_id"].GetInt();
      int cate_id = ann[i]["category_id"].GetInt();
      float xmin = ann[i]["bbox"][0].GetFloat();
      float ymin = ann[i]["bbox"][1].GetFloat();
      float xd = ann[i]["bbox"][2].GetFloat();
      float yd = ann[i]["bbox"][3].GetFloat();
      //std::cout << id << " " << image_id << " " << cate_id << " " <<  xmin << " " << ymin << " " << xmax << " " << ymax << std::endl;

      float bbox[] = {xmin, ymin, xmin+xd, ymin+yd};
      ODObject obj = ODObject(cate_id, "", 0, 0, bbox);
      char buff[15]; sprintf(buff, "%012d", image_id);
      std::string image_name = "COCO_train2014_" +  std::string(buff) + ".jpg";
      M[image_name].push_back(obj);
    }

    // load annotation of validation set
    ann = (*val_doc_)["annotations"];
    //std::cout << ann.Size() << std::endl;
    for (int i = 0; i < ann.Size(); i++)
    {
      long long id = ann[i]["id"].GetFloat();
      int image_id = ann[i]["image_id"].GetInt();
      int cate_id = ann[i]["category_id"].GetInt();
      float xmin = ann[i]["bbox"][0].GetFloat();
      float ymin = ann[i]["bbox"][1].GetFloat();
      float xd = ann[i]["bbox"][2].GetFloat();
      float yd = ann[i]["bbox"][3].GetFloat();
      //std::cout << id << " " << image_id << " " << cate_id << " " <<  xmin << " " << ymin << " " << xmax << " " << ymax << std::endl;

      float bbox[] = {xmin, ymin, xmin+xd, ymin+yd};
      ODObject obj = ODObject(cate_id, "", 0, 0, bbox);
      //M[image_id].push_back(obj);
      char buff[15]; sprintf(buff, "%012d", image_id);
      std::string image_name = "COCO_val2014_" +  std::string(buff) + ".jpg";
      M[image_name].push_back(obj);
    }
    for (std::map<std::string, std::vector<ODObject> >::iterator it = M.begin(); it != M.end(); it++)
    {
      std::string file_name = it->first;
      int width = this->width_[file_name];
      int height = this->height_[file_name];
      //char buff[33]; sprintf(buff, "%d", it->first);
      //std::string file_name = std::string(buff);
      std::cout << file_name << " " << height << " " << width << std::endl;
      ODAnnotation annotation = ODAnnotation(height, width, it->second);    
      this->annotations_[file_name] = annotation;
    }
    M.clear();
  }

  void MSCoco::evaluation() {}

}

