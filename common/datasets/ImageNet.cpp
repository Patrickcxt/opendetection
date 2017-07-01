//
// Created by Xiongtao Chen on 25.06.2017
//

#include <fstream>
#include <iostream>
#include <cstdlib>

#include "ImageNet.h"


namespace od
{

  void ImageNet::loadClassList()
  {
    std::string wnids_path = this->base_path_ + "wnids.txt";
    std::string words_path = this->base_path_ + "words.txt";
    std::map<std::string, std::string> id2name;

    std::string s;
    std::ifstream input;
    input.open(words_path.c_str());
    if (!input)
    {
        std::cerr << "error: unable to open input file: " << words_path << std::endl;
        return;
    }
    while (getline(input, s))
    {
      int tab_pos = s.find('\t');
      std::string wnid = s.substr(0, tab_pos);
      std::string name = s.substr(tab_pos+1);
      id2name[wnid] = name;
    }
    input.close();
    input.clear();

    input.open(wnids_path.c_str());
    if (!input)
    {
        std::cerr << "error: unable to open input file: " << wnids_path << std::endl;
        return;
    }
    int id = 0;
    while (getline(input, s))
    {
      std::string label_name = id2name[s];
      this->class_list_[id] = label_name;
      this->id2wnid_[id] = s;
      this->wnid2id_[s] = id++;
    }
    input.close();
    input.clear();

    this->num_classes_ = id;
  }

  void ImageNet::loadImageLists()
  {
    std::vector<std::string> train_image_list;
    std::vector<std::string> val_image_list;
    std::vector<std::string> trainval_image_list;
    std::vector<std::string> test_image_list;

    for (std::map<int, std::string>::iterator it = id2wnid_.begin(); it != id2wnid_.end(); it++)
    {
      std::string wnid = it->second;
      std::vector<std::string> files = get_files_in_directory(this->trainset_path_+wnid+"/images/", "JPEG");
      train_image_list.insert(train_image_list.end(), files.begin(), files.end());
    }
    
    val_image_list = get_files_in_directory(this->valset_path_+"images/", false, "JPEG");
    test_image_list = get_files_in_directory(this->testset_path_+"images/", false, "JPEG");
    trainval_image_list = train_image_list;
    trainval_image_list.insert(trainval_image_list.end(), val_image_list.begin(), val_image_list.end());

    this->train_image_list_ = train_image_list;
    this->val_image_list_ = val_image_list;
    this->trainval_image_list_ = trainval_image_list;
    this->test_image_list_ = test_image_list;
  }

  void ImageNet::loadAnnotations()
  {

    /*
    std::vector<std::string> train_image_list;
    std::vector<std::string> val_image_list;
    std::vector<std::string> trainval_image_list;
    std::vector<std::string> test_image_list;
    */
    std::map<std::string, std::vector<ODObject> > M;
    std::map<std::string, ODAnnotation> annotations;

    std::ifstream input;
    std::string s;

    // load train image list and annotations
    for (std::map<int, std::string>::iterator it = id2wnid_.begin(); it != id2wnid_.end(); it++)
    {
      int id = it->first;
      std::string wnid = it->second;
      std::string ann_path = this->trainset_path_ + wnid + "/" + wnid + "_boxes.txt";
      input.open(ann_path.c_str());
      if (!input)
      {
        std::cerr << "error: unable to open input file: " << ann_path << std::endl;
        return;
      }
      while (getline(input, s))
      {

        // get the bounding box;
        //std::cout << s << std::endl;
        std::vector<std::string> items = split(s, '\t');
        
        std::string image_name = items[0];
        int xmin = atoi(items[1].c_str());
        int ymin = atoi(items[2].c_str());
        int xmax = atoi(items[3].c_str());
        int ymax = atoi(items[4].c_str());
        //std::cout << xmin << " " << ymin << " " << xmax << " " << ymax << std:: endl;
        float bbox[4] = {xmin, ymin, xmax, ymax};
        ODObject object = ODObject(this->class_list_[id], "", 0, 0, bbox);
        M[image_name].push_back(object);
        //train_image_list.push_back(image_name);
        //trainval_image_list.push_back(image_name);

      }
      input.close();
      input.clear();
    }

    // load validation image list and annotations
    std::string val_ann_path = this->valset_path_ + "val_annotations.txt";
    input.open(val_ann_path.c_str());
    if (!input)
    {
      std::cerr << "error: unable to open input file: " << val_ann_path << std::endl;
      return;
    }
    while (getline(input, s))
    {

      std::vector<std::string> items = split(s, '\t');
      std::string image_name = items[0];
      int id = wnid2id_[items[1]];
      int xmin = atoi(items[2].c_str());
      int ymin = atoi(items[3].c_str());
      int xmax = atoi(items[4].c_str());
      int ymax = atoi(items[5].c_str());
      float bbox[4] = {xmin, ymin, xmax, ymax};
      ODObject object = ODObject(class_list_[id], "", 0, 0, bbox);
      M[image_name].push_back(object);
      //val_image_list.push_back(image_name);
      //trainval_image_list.push_back(image_name);
    }

    // load test image list.  Annotation file is not provided.
    // test_image_list = get_files_in_directory(this->testset_path_+"images/", false, "JPEG");

    // Construct anntations
    for (std::map<std::string, std::vector<ODObject> >::iterator it = M.begin(); it != M.end(); it++)
    {
      ODAnnotation ann = ODAnnotation(it->second); 
      annotations[it->first] = ann;
    }

    /*
    this->train_image_list_ = train_image_list;
    this->val_image_list_ = val_image_list;
    this->trainval_image_list_ = trainval_image_list;
    this->test_image_list_ = test_image_list;
    */
    this->annotations_ = annotations;
  }

}

