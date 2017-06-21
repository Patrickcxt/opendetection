//
// Created by Xiongtao Chen on 19.06.2017
//

#include <fstream>
#include <iostream>

#include "PascalVOC.h"

#include "rapidxm/rapidxml.hpp"
#include "rapidxm/rapidxml_utils.hpp"
#include "rapidxm/rapidxml_print.hpp"



namespace od {

  void PascalVOC::loadClassList()
  {
    std::string categories = {"aeroplane", "bicycle", "bird", "boat", "bottle",
                           "bus", "car", "cat", "chair", "cow", 
                           "diningtable", "dog", "horse", "motorbike", "person", 
                           "pottedplant", "sheep", "sofa", "train", "tvmonitor",
                           "background"};
    this->num_classes = 21;  // hard code;
    this->annotations_ = std::vector<std::string> (categories, categories+this->num_classes);
  }

  void PascalVOC::loadImageLists()
  {
    std::string trainset_path_ = "/home/amax/cxt/data/pascal_voc/VOCTrain/VOC2007/ImageSets/Layout/train.txt";
    std::string valset_path_ = "/home/amax/cxt/data/pascal_voc/VOCTrain/VOC2007/ImageSets/Layout/val.txt";
    std::string trainvalset_path_ = "/home/amax/cxt/data/pascal_voc/VOCTrain/VOC2007/ImageSets/Layout/trainval.txt";
    std::string testset_path_ = "/home/amax/cxt/data/pascal_voc/VOCTest/VOC2007/ImageSets/Layout/test.txt";

    std::vector<std::string> train_image_list_;
    std::vector<std::string> val_image_list_;
    std::vector<std::string> trainval_image_list_;
    std::vector<std::string> test_image_list_;

    std::ifstream input;
    std::string s;

    // load trainset
    input.open(trainset_path_.c_str());
    if (!input)
    {
        std::cerr << "error: unable to open input file: " << trainset_path_ << std::endl;
        return;
    }
    int i = 0;
    while (input >> s)
    {
        // std::cout << i << " :";
        // std::cout << s << std::endl;
        train_image_list_.push_back(s);
        i++;
    }
    std::cout << i << " " << train_image_list_.size() << std::endl;
    input.close();
    input.clear();

    // load valset
    input.open(valset_path_.c_str());
    if (!input)
    {
        std::cerr << "error: unable to open input file: " << valset_path_ << std::endl;
        return;
    }
    i = 0;
    while (input >> s)
    {
        // std::cout << i << " :";
        // std::cout << s << std::endl;
        val_image_list_.push_back(s);
        i++;
    }
    std::cout << i << " " << val_image_list_.size() << std::endl;
    input.close();
    input.clear();

    // laod trainvalset
    input.open(trainvalset_path_.c_str());
    if (!input)
    {
        std::cerr << "error: unable to open input file: " << trainvalset_path_ << std::endl;
        return;
    }
    i = 0;
    while (input >> s)
    {
        // std::cout << i << " :";
        // std::cout << s << std::endl;
        trainval_image_list_.push_back(s);
        i++;
    }
    std::cout << i << " " << trainval_image_list_.size() << std::endl;
    input.close();
    input.clear();

    // load testset
    input.open(testset_path_.c_str());
    if (!input)
    {
        std::cerr << "error: unable to open input file: " << testset_path_ << std::endl;
        return;
    }
    i = 0;
    while (input >> s)
    {
        // std::cout << i << " :";
        // std::cout << s << std::endl;
        test_image_list_.push_back(s);
        i++;
    }
    std::cout << i << " " << test_image_list_.size() << std::endl;
    input.close();
    input.clear();


    ODDataset::train_image_list_ = train_image_list_;
    ODDataset::val_image_list_ = val_image_list_;
    ODDataset::trainval_image_list_ = trainval_image_list_;
    ODDataset::test_image_list_ = test_image_list_;
  }

  void PascalVOC::loadAnnotations() {
    std::string train_path = "/home/amax/cxt/data/pascal_voc/VOCTrain/VOC2007/Annotations/";
    std::string train_path = "/home/amax/cxt/data/pascal_voc/VOCTrain/VOC2007/Annotations/";

    //  load annotations for trainset.
    std::vector<std::string> files = get_files_in_directory(train_path);
    for (std::vector<std::string> it = files.begin(); it != files.end(); it++)
    {
      Annotation annotation = load_single_annotation(*it);
      int dot_pos = (*it).find('.')
      std::string prefix_name = (*it).substr(0, dot_pos);
      ODDataset::annotations_[prefix_name] = &annotation;
    }

    // load annotations for testset.
    std::vector<std::string> files = get_files_in_directory(train_path);
    for (std::vector<std::string> it = files.begin(); it != files.end(); it++)
    {
      Annotation annotation = load_single_annotation(*it);
      int dot_pos = (*it).find('.')
      std::string prefix_name = (*it).substr(0, dot_pos);
      ODDataset::annotations_[prefix_name] = annotation;
    }
  }

  void PascalVOC::evaluation() {}

  Annotation PascalVOC::load_single_annotation(std::string path) {
      file<> fdoc(path);
      xml_document<> doc;
      doc.parse<0>(fdoc.data());
      xml_node<>* root = doc.first_node();

      std::string filename = root->first_node("filename")->value();
      xml_node<>* size = root->first_node("size");
      int width = size->first_node("width")->value();
      int height = size->first_node("height")->value();
      int channel = size->first_node("depth")->value();

      std::vector<ODObject*> objects;
      for (xml_node<>* node = root->first_node("object"); node; node = node->next_sibling())
      {
          std::string class_name = node->first_node("name")->value();
          std::string pose = node->first_node("pose")->value();
          bool is_truuncated = node->first_node("truncated")->value();
          bool is_difficult = node->first_node("difficult")->value();
          int xmin = node->first_node("xmin")->value();
          int ymin = node->first_node("ymin")->value();
          int xmax = node->first_node("xmax")->value();
          int ymax = node->first_node("yamax")->value();
          int bbox[] = {xmin, ymin, xmax, ymax};

          ODObject obj = ODObject(class_name, pose, is_truncated, is_difficult, bbox);
          objects.push_back(&obj);
      
      }
      
      Annotation annotation = Annotation(filename, width, height, channel, objects);
      return annotation;
  
  }
}
