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
      std::string class_name_;
      std::string pose_;  // if provided
      bool is_truncated_; // if provided
      bool is_difficult_; // if provided
      int bbox_[4];   // Bounding box: xmin, ymin, xmax, ymax

      ODObject(std::string class_name, std::string pose, 
              bool is_truncated, bool is_difficult, int* bbox)
      {
          class_name_ = class_name;
          pose_ = pose;
          is_truncated_ = is_truncated;
          is_difficult_ = is_difficult;
          for (int i = 0; i < 4; i++) {
              bbox_[i] = bbox[i];
          }
      }
  };

  /** \brief The annotation class for each image.
   *
   * \author Xiongtao Chen
   *
   */
  struct ODAnnotation
  {
      std::string filename_; 
      int width_;
      int height_;
      int channel_;
      std::vector<ODObject> objects_;  // Object list that a image contains.

      ODAnnotation() {}
      ODAnnotation(std::string filename, int width, int height, 
              int channel, std::vector<ODObject> objects)
      {
        filename_ = filename;
        width_ = width;
        height_ = height;
        channel_ = channel;
        objects_ = objects; 
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

    ODDataset(std::string name_, int task_type_, std::string image_path_, std::string annotation_path_) 
    { 
      this->name_ = name_;
      this->task_type_ = task_type_;
      this->image_path_ = image_path_;
      this->annotation_path = annotation_path_;
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

    virtual void loadClassList() {}

    std::vector<std::string> getClassesList() const  // return the classes list of the dataset
    {
      return class_list_; 
    }

    // load the image path list of train_image_list, val_image_list and test_image_list, maybe private.
    virtual void loadImageLists() {}  
 
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

    // load label and ground truth from the annotation files, maybe private
    virtual void loadAnnatations() {}  

    std::map<std::string, ODAnnotation> getAllAnnotations() const
    {
      return annotations_; 
    }

    ODAnnotation getSingleAnnotation(std::string image_name)
    {
      return annotations_[image_name]; 
    }

    virtual ODBatch getNextBatch(int batch_size_); // get a minibatch, ODBatch class/structure to be implemented later.

    virtual void evaluate() {}

    virtual ~ODDataset() {}

  protected:

    std::string name_;  // Name of the dataset
    std::string image_path_;
    std::string annotation_path;
    int task_type_;      // Task to perfom.  Classification or Detection etc.
    int num_classes_;    // Number of the categories
    int iter_;           // cursor that points to current batch. 


    std::vector<std::string> class_list_; // List of categories

    std::vector<std::string> train_image_list_;   // image list of training set
    std::vector<std::string> val_image_list_;     // image list of validation set
    std::vector<std::string> trainval_image_list_;   // image list of training and validation set, if provided
    std::vector<std::string> test_image_list_;    // image list of test set

    std::map<std::string, ODAnnotation> annotations_;    // annotations for each images, ODAnnotation class/structure to be implemented later.

    std::vector<std::string> get_files_in_directory(std::string base_path, std::string ext);
    
  };


}
#endif //OPENDETECTION_ODDATASETS_H
