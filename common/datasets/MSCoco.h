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


#ifndef OPENDETECTION_MSCOCO_H
#define OPENDETECTION_MSCOCO_H

#include "ODDatasets.h"
#include "rapidjson/document.h"

namespace rj = rapidjson;

namespace od
{
  /** \brief MSCOCO 2014 dataset
   * \author Xiongtao Chen
   *
   */
  class MSCoco: public ODDataset
  {
  public:
    MSCoco(std::string base_path_, int task_type_) : 
        ODDataset("MSCOCO_2014", base_path_, task_type_) 
    {
        // Set paths.
        this->train_image_path_ = this->base_path_ + "Images/train2014/";
        this->val_image_path_ = this->base_path_ + "Images/val2014/";
        this->test_image_path_ = this->base_path_ + "Imagestest2014/";
        this->train_annotation_path_ = this->base_path_ + "Annotations/instances/instances_train2014.json";
        this->val_annotation_path_ = this->base_path_ + "Annotations/instances/instances_val2014.json";
        this->test_annotation_path_ = "";  // Not provided yet

        std::cout << "======= Loading Annotation Files ...  ===============" << std::endl;  // takes several seconds
        loadDataset();
        std::cout << "======= Loading Classes List ...  ===============" << std::endl;
        loadClassList();
        std::cout << "======= Loading Image List ...  ===============" << std::endl;
        loadImageLists();
        std::cout << "======= Loading Annotations ...  ===============" << std::endl;
        loadAnnotations();
        std::cout << "======= Done ...  ===============" << std::endl;
    }

    void loadDataset();
    // override
    void loadClassList();

    // override
    void loadImageLists();

    // override
    void loadAnnotations();

    void evaluation();

    ~MSCoco() {}

  private:

    std::string train_image_path_;
    std::string val_image_path_;
    std::string test_image_path_;


    std::string train_annotation_path_;
    std::string val_annotation_path_;
    std::string test_annotation_path_;
    rj::Document* train_doc_;        // training dataset
    rj::Document* val_doc_;          // validation dataset
    rj::Document* test_doc_;  // not provided
    std::map<int, int> width_;
    std::map<int, int> height_;

  };
}

# endif // OPENDETEDECTION_MSCOCO_H

