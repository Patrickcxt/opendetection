//
// Created by Xiongtao Chen on 25.06.2017
//

#include <fstream>
#include <iostream>
#include <cstdlib>

#include "ImageNet.h"


namespace od
{
  void ImageNet::convertDatasetToLmdb(std::string subset, std::string save_dir, int resize_height, int resize_width)
  {
     std::vector<std::string> image_list;
     std::string img_prefix = "";
     if (subset == "train")
     {
       /*
       for (int i = 0; i < train_image_list_.size(); i++)
       {
         std::cout << train_image_list_[i] << std::endl;
       }
       img_prefix = this->base_path_ + "train/";
       */
       image_list = this->train_image_list_;
     }
     else if (subset == "val")
     {
       //img_prefix =  this->base_path_ + "val/images/";
       image_list = this->val_image_list_;
     }
     else if (subset == "trainval")
     {
        // To implement.
     }
     else
     {
       //img_prefix = this->base_path_ + "test/images/";
       image_list = this->test_image_list_;
     }
     convert_dataset_to_lmdb(image_list, img_prefix, save_dir, resize_height, resize_width);
  }

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
      this->label2name_[id] = label_name;
      this->name2label_[label_name] = id;
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
        float xmin = atof(items[1].c_str());
        float ymin = atof(items[2].c_str());
        float xmax = atof(items[3].c_str());
        float ymax = atof(items[4].c_str());
        //std::cout << xmin << " " << ymin << " " << xmax << " " << ymax << std:: endl;
        float bbox[4] = {xmin, ymin, xmax, ymax};
        ODObject object = ODObject(id, "", 0, 0, bbox);
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
      float xmin = atof(items[2].c_str());
      float ymin = atof(items[3].c_str());
      float xmax = atof(items[4].c_str());
      float ymax = atof(items[5].c_str());
      float bbox[4] = {xmin, ymin, xmax, ymax};
      ODObject object = ODObject(id, "", 0, 0, bbox);
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

