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


#ifndef OPENDETECTION_PASCALVOC_H
#define OPENDETECTION_PASCALVOC_H

#include "ODDatasets.h"


namespace od {
/** \brief Pascal VOC 2007 dataset
   * \author Xiongtao Chen
   *
   */
  class PascalVOC: public ODDataset
  {
  public:
    PascalVOC(int task_type_, std::string image_path_, std::string annotation_path_) : 
        ODDataset("pascal_voc_2007", task_type_, image_path_, annotation_path_) 
    {
        loadImageLists();
        std::cout << "======================" << std::endl;
        loadAnnotations();
    }

    // override
    void loadClassList();

    // override
    void loadImageLists();

    // override
    void loadAnnotations();

    void evaluation();

    ~PascalVOC() {}

  private:

    std::string trainset_path_;     // path of train.txt
    std::string trainvalset_path_;  // path of trainval.txt
    std::string valset_path_;       // path of val.txt
    std::string testset_path_;      // path of test.txt

    ODAnnotation load_single_annotation(std::string img_name_);

  };
}

# endif // OPENDETEDECTION_PASCAL_VOC_H


