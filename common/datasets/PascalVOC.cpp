//
// Created by Xiongtao Chen on 19.06.2017
//

#include <fstream>
#include <iostream>

#include "PascalVOC.h"

#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "rapidxml/rapidxml_print.hpp"

namespace rx = rapidxml;

namespace od
{

  void PascalVOC::loadClassList()
  {
    std::string categories[] = {"aeroplane", "bicycle", "bird", "boat", "bottle",
                           "bus", "car", "cat", "chair", "cow", 
                           "diningtable", "dog", "horse", "motorbike", "person", 
                           "pottedplant", "sheep", "sofa", "train", "tvmonitor",
                           "background"};
    this->num_classes_ = 21;  // hard code;
    for (int i = 1; i < this->num_classes_; i++)
        this->class_list_[i] = categories[i];
    //this->class_list_ = std::vector<std::string> (categories, categories+this->num_classes_);
  }

  void PascalVOC::loadImageLists()
  {

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
    while (input >> s)
    {
        train_image_list_.push_back(s);
    }
    input.close();
    input.clear();

    // load valset
    input.open(valset_path_.c_str());
    if (!input)
    {
        std::cerr << "error: unable to open input file: " << valset_path_ << std::endl;
        return;
    }
    while (input >> s)
    {
        val_image_list_.push_back(s);
    }
    input.close();
    input.clear();

    // laod trainvalset
    input.open(trainvalset_path_.c_str());
    if (!input)
    {
        std::cerr << "error: unable to open input file: " << trainvalset_path_ << std::endl;
        return;
    }
    while (input >> s)
    {
        trainval_image_list_.push_back(s);
    }
    input.close();
    input.clear();

    // load testset
    input.open(testset_path_.c_str());
    if (!input)
    {
        std::cerr << "error: unable to open input file: " << testset_path_ << std::endl;
        return;
    }
    while (input >> s)
    {
        test_image_list_.push_back(s);
    }
    input.close();
    input.clear();


    this->train_image_list_ = train_image_list_;
    this->val_image_list_ = val_image_list_;
    this->trainval_image_list_ = trainval_image_list_;
    this->test_image_list_ = test_image_list_;
  }

  void PascalVOC::loadAnnotations()
  {
    
    //  load annotations for trainset.
    std::vector<std::string> files = ODDataset::get_files_in_directory(this->train_annotation_path_, "jpg");
    std::cout << files.size() << std::endl;
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++)
    {
      ODAnnotation annotation = load_single_annotation(*it);
      
      int slash_pos = (*it).rfind('/')+1;
      int dot_pos = (*it).find('.');
      std::string prefix_name = (*it).substr(slash_pos, dot_pos-slash_pos);
      this->annotations_[prefix_name] = annotation;
    }
    std::cout << this->annotations_.size() << std::endl;


    // load annotations for testset.
    files = ODDataset::get_files_in_directory(this->test_annotation_path_, "jpg");
    std::cout << files.size() << std::endl;
    for (std::vector<std::string>::iterator it = files.begin(); it != files.end(); it++)
    {
      ODAnnotation annotation = load_single_annotation(*it);
      int slash_pos = (*it).rfind('/')+1;
      int dot_pos = (*it).find('.');
      std::string prefix_name = (*it).substr(slash_pos, dot_pos-slash_pos);
      this->annotations_[prefix_name] = annotation;
    }
    std::cout << this->annotations_.size() << std::endl;
  }

  void PascalVOC::evaluation() {}

  ODAnnotation PascalVOC::load_single_annotation(std::string xml_file)
  {
      rx::file<> fdoc(xml_file.c_str());
      rx::xml_document<> doc;
      doc.parse<0>(fdoc.data());
      rx::xml_node<>* root = doc.first_node();

      std::string filename = root->first_node("filename")->value();
      rx::xml_node<>* size = root->first_node("size");
      int width = atoi((size->first_node("width")->value()));
      int height = atoi((size->first_node("height")->value()));
      int channel = atoi((size->first_node("depth")->value()));


      std::vector<ODObject> objects;
      for (rx::xml_node<>* node = root->first_node("object"); node; node = node->next_sibling())
      {
          std::string class_name = node->first_node("name")->value();
          std::string pose = node->first_node("pose")->value();
          bool is_truncated = atoi((node->first_node("truncated")->value()));
          bool is_difficult = atoi((node->first_node("difficult")->value()));
          rx::xml_node<>* box_node = node->first_node("bndbox");
          
          int xmin = atoi((box_node->first_node("xmin")->value()));
          int ymin = atoi((box_node->first_node("ymin")->value()));
          int xmax = atoi((box_node->first_node("xmax")->value()));
          int ymax = atoi((box_node->first_node("ymax")->value()));
          float bbox[] = {xmin, ymin, xmax, ymax};

          ODObject obj = ODObject(class_name, pose, is_truncated, is_difficult, bbox);
          objects.push_back(obj);
      
      }
      
      ODAnnotation annotation = ODAnnotation(objects);
      return annotation;
  
  }
}

