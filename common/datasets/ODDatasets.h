/*
Copyright (c) 2015, Kripasindhu Sarkar
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holder(s) nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
//
// Created by chenxt.
//

#ifndef OPENDETECTION_ODDATASETS_H
#define OPENDETECTION_ODDATASETS_H

#include <iostream>
#include <vector>
#include <map>

namespace od
{

  /** \brief The structure for an object instance.
   *
   * \author Xiongtao Chen
   *
   */
  struct ODObject {
      //std::string class_name_;
      int label_;
      std::string pose_;  // if provided
      bool is_truncated_; // if provided
      bool is_difficult_; // if provided
      float bbox_[4];   // Bounding box: xmin, ymin, xmax, ymax

      ODObject(int label, std::string pose, 
              bool is_truncated, bool is_difficult, float* bbox)
      {
          //class_name_ = class_name;
          label_ = label;
          pose_ = pose;
          is_truncated_ = is_truncated;
          is_difficult_ = is_difficult;
          for (int i = 0; i < 4; i++) {
              bbox_[i] = bbox[i];
          }
      }

      friend std::ostream& operator <<(std::ostream& out, const ODObject& object)
      {
        float xmin = object.bbox_[0], ymin = object.bbox_[1];
        float xmax = object.bbox_[2], ymax = object.bbox_[3];

        out << "Category: "   << object.label_ << " | "
            << "Bbox: [" << xmin << ", " << ymin<< ", " << xmax << ", " << ymax << "]" << std::endl;
        return out;
      }
  };

  /** \brief The annotation class for each image.
   *
   * \author Xiongtao Chen
   *
   */
  struct ODAnnotation
  {
      int height_;
      int width_;
      std::vector<ODObject> objects_;  // Object list that a image contains.

      ODAnnotation() {}
      ODAnnotation(int height, int width, std::vector<ODObject> objects): 
          height_(height), width_(width), objects_(objects) {}

      friend std::ostream& operator <<(std::ostream& out, const ODAnnotation& annotation)
      {
        std::vector<ODObject> objects = annotation.objects_;
        out << "---------------------------------------------------------------------------" << std::endl;
        out << "Height: " << annotation.height_ << "  Width: " << annotation.width_ << endl;
        out << objects.size() << " objects contained: " << std::endl;
        for (int i = 0; i < objects.size(); i++)
        {
            out << "Object " << i+1 << std::endl << objects[i] << std::endl;
        }
        out << "---------------------------------------------------------------------------" << std::endl;
        return out;
      }
  };

  /** \brief The minibatch class for training.
   *
   * \author Xiongtao Chen
   *
   */
  struct ODBatch
  {
      int batch_size_;
      std::vector<std::string> image_list_;
      std::vector<ODAnnotation> annotation_list_;

      ODBatch(int batch_size, std::vector<std::string> image_list, 
              std::vector<ODAnnotation> annotation_list)
      {
        batch_size_ = batch_size;
        image_list_ = image_list;
        annotation_list_ = annotation_list;
      }
  };


  /** \brief The basic dataset class; all special datasets derive from this.
   *
   * \author Xiongtao Chen
   *
   */
  class ODDataset
  {
  public:

    ODDataset(std::string name_, std::string base_path_, int task_type_) 
    { 
      this->name_ = name_;
      this->task_type_ = task_type_;
      this->base_path_ = base_path_;
      this->iter_ = 0;
    }

    std::string getDatasetName() const  // return name of the specified dataset, e.g. pascal_voc_2012.
    {
        return name_;
    }

    int getNumOfClasses() const  // return number of the categories. e.g. 20 classes for pascal_voc_2012
    {
      return num_classes_; 
    }


    std::map<int, std::string> getClassesList() const  // return the classes list of the dataset, label to category name
    {
      //return class_list_; 
      return label2name_;
    }

    std::map<std::string, int> getName2Label() const  // return mapping of category name to label
    {
      return name2label_; 
    }

    std::vector<std::string> getTrainImageList() const
    {
      return train_image_list_;
    }

    std::vector<std::string> getValImageList() const
    {
      return val_image_list_; 
    }

    std::vector<std::string> getTrainvalImageList() const
    {
      return trainval_image_list_;
    }

    std::vector<std::string> getTestImageList() const
    {
      return test_image_list_; 
    }

    std::map<std::string, ODAnnotation> getAllAnnotations() const
    {
      return annotations_; 
    }

    ODAnnotation getAnnotationByName(std::string image_name)
    {
      return annotations_[image_name]; 
    }

    virtual ODBatch getNextBatch(int batch_size_); // get a minibatch, ODBatch class/structure to be implemented later.

    virtual void convertDatasetToLmdb(std::string subset, std::string save_dir, int resize_height, int resize_width) {}  // convert dataset to lmdb format for caffe input.


    virtual void evaluate() {}

    virtual ~ODDataset() {}

  protected:

    std::string name_;  // Name of the dataset
    std::string base_path_;  // root path for a specific dataset, e.g. /path/to/coco/
    int task_type_;      // Task to perfom.  Classification or Detection etc.
    int num_classes_;    // Number of the categories
    int iter_;           // cursor that points to current batch. 


    //std::map<int, std::string> class_list_; // List of categories
    std::map<int, std::string> label2name_; // List of categories
    std::map<std::string, int> name2label_; 

    std::vector<std::string> train_image_list_;   // image list of training set
    std::vector<std::string> val_image_list_;     // image list of validation set
    std::vector<std::string> trainval_image_list_;   // image list of training and validation set, if provided
    std::vector<std::string> test_image_list_;    // image list of test set 

    std::map<std::string, ODAnnotation> annotations_;    // annotations for each images, ODAnnotation class/structure to be implemented later.


    std::vector<std::string> get_files_in_directory(std::string base_path, bool is_folder, std::string ext="");

    std::vector<std::string> split(std::string str, char sep);

    // convert dataset to lmdb for classification
    void convert_dataset_to_lmdb(std::vector<std::string> image_list, std::string img_prefix, std::string save_dir, int resize_height, int resize_width);

    // convert dataset to lmdb for detection, classification and detection will be merged later.
    void convert_dataset_to_lmdb_detection(std::vector<std::string> image_list, std::string img_prefix, std::string save_dir, int resize_height, int resize_width);

    void read_bbox_to_annotated_datum(const string filename, od::ODAnnotation annotation, const int img_height, const int img_width, AnnotatedDatum* anno_datum);

  private:

    // load the list of all categories
    virtual void loadClassList() {}

    // load the image path list of train_image_list, val_image_list and test_image_list, maybe private.
    virtual void loadImageLists() {}  

    // load label and ground truth from the annotation files, maybe private
    virtual void loadAnnatations() {}  


  };


}
#endif //OPENDETECTION_ODDATASETS_H
