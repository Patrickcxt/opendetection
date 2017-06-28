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
// Created by Xiongtao Chen on 16.06.17.
//

#include "ODDatasets.h"
#include <boost/filesystem.hpp>
#include <algorithm>

namespace bf = boost::filesystem;


namespace od
{

  ODBatch ODDataset::getNextBatch(int batch_size)
  {
    if (iter_ == 0 || iter_ >= train_image_list_.size())
    {
      // shuffle the data
        std::random_shuffle(train_image_list_.begin(), train_image_list_.end());
        iter_ = 0;
    }

    std::vector<std::string> image_list;
    std::vector<ODAnnotation> annotation_list;

    for (int i = iter_; i < iter_ + batch_size; ++i)
    {
      image_list.push_back(train_image_list_[i]);
      annotation_list.push_back(annotations_[train_image_list_[i]]);
    }
    iter_ = iter_ + batch_size;
    ODBatch batch = ODBatch(batch_size, image_list, annotation_list);
    return batch;
  }

  std::vector<std::string> ODDataset::get_files_in_directory(std::string base_path, bool is_folder, std::string ext)
  {
    std::vector<std::string> files;
    bf::path p(base_path);
    bf::directory_iterator end_itr;
    for (bf::directory_iterator itr(p); itr != end_itr; ++itr)
    {
      if (!is_folder && bf::is_regular_file(itr->path()))
        files.push_back(itr->path().string());
      else
        files.push_back(itr->path().string());
    }
    return files;
  }

  std::vector<std::string> ODDataset::split(std::string str, char sep)
  {
    std::vector<std::string> items;
    size_t pos = str.find(sep);
    while (pos != std::string::npos)
    {
      std::string item = str.substr(0, pos);
      items.push_back(item);
      str = str.substr(pos+1);
      pos = str.find(sep);
    }
    items.push_back(str);
    return items;
  }
}
