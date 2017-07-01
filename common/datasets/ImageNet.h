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


#ifndef OPENDETECTION_IMAGENET_H
#define OPENDETECTION_IMAGENET_H

#include "ODDatasets.h"


namespace od
{
/** \brief Pascal VOC 2007 dataset
   * \author Xiongtao Chen
   *
   */
  class ImageNet: public ODDataset
  {
  public:
    ImageNet(std::string base_path_, int task_type_) : 
        ODDataset("ImageNet", base_path_, task_type_) 
    {
        // Set paths.
        this->trainset_path_ = this->base_path_ + "train/";
        this->valset_path_ = this->base_path_ + "val/";
        this->testset_path_ = this->base_path_ + "test/";

        std::cout << "======= Loading Classes List ...  ===============" << std::endl;
        loadClassList();
        std::cout << "======= Loading Image List ...  ===============" << std::endl;
        loadImageLists();
        std::cout << "======= Loading Annotations ...  ===============" << std::endl;
        loadAnnotations();
        std::cout << "======= Done ...  ===============" << std::endl;
    }


    void evaluation();

    ~ImageNet() {}

  private:

    std::string trainset_path_;
    std::string valset_path_;
    std::string testset_path_;

    std::map<int, std::string> id2wnid_;   // map 0, 1, ..n to wnid.  e.g. wnid2id_[0] = "n01443537";
    std::map<std::string, int> wnid2id_;

    // override
    void loadClassList();

    // override
    void loadImageLists();

    // override
    void loadAnnotations();
  };
}

# endif // OPENDETEDECTION_IMAGENET_H


