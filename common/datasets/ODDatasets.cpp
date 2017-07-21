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
#include <algorithm>
#include <utility>

#include <boost/filesystem.hpp>
#include "boost/scoped_ptr.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include "caffe/util/format.hpp"
#include "caffe/util/io.hpp"
#include "caffe/util/rng.hpp"

using namespace caffe;
using std::pair;
//using boost::scoped_str;

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
    if (!bf::exists(bf::system_complete(p))) {
      //std::cout << "Path not exists" << std::endl;
      return files;
    }
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

  /**
   * This code is modified form convert_imageset.cpp of caffe.
   */
  void ODDataset::convert_dataset_to_lmdb(std::vector<std::string> image_list, std::string img_prefix, std::string save_dir,
          int resize_height, int resize_width) {
    const bool is_color = true;
    const bool check_size = false;
    const bool encoded = false;
    const bool is_shuffle = true;
    const string encode_type = "";


    std::map<std::string, ODAnnotation> anns =  annotations_;

    if (is_shuffle) {
      // randomly shuffle data
      LOG(INFO) << "Shuffling data";
      shuffle(image_list.begin(), image_list.end());
    }

    if (encode_type.size() && !encoded)
      LOG(INFO) << "encode_type specified, assuming encoded=true.";

    // Create new DB
    boost::scoped_ptr<db::DB> db(db::GetDB("lmdb"));
    db->Open(save_dir, db::NEW);
    boost::scoped_ptr<db::Transaction> txn(db->NewTransaction());

    // Storing to db
    Datum datum;
    int count = 0;
    int data_size = 0;
    bool data_size_initialized = false;

    for (int i = 0; i < image_list.size(); i++) {
      bool status;
      std::cout << image_list[i] << std::endl;
      std::string enc = encode_type;
      if (encoded && !enc.size()) {
        // Guess the encoding type from the file name
        string fn = image_list[i];
        std::cout << fn << std::endl;
        size_t p = fn.rfind('.');
        if ( p == fn.npos )
          LOG(WARNING) << "Failed to guess the encoding of '" << fn << "'";
        enc = fn.substr(p);
        std::transform(enc.begin(), enc.end(), enc.begin(), ::tolower);
      }

      /*  load image to datum */
      std::vector<std::string> items = split(image_list[i], '/');
      std::string image_name = items[items.size()-1];
      od::ODAnnotation an = anns[image_name];
      int label = an.objects_[0].label_;
      status = ReadImageToDatum(img_prefix + image_list[i], label, resize_height, resize_width, is_color,
        enc, &datum);

      //std::cout << datum.channels() << " " << datum.height() << " " << datum.width() <<  " " << datum.data().size() << std::endl;
      // load label to datum
      /*
      status = ConvertLabelToDatum(an, &datum);
      */

      if (status == false) continue;
      if (check_size) {
        if (!data_size_initialized) {
          data_size = datum.channels() * datum.height() * datum.width();
          data_size_initialized = true;
          //std::cout << "size of first data: " << data_size << std::endl;
        } else {
          const std::string& data = datum.data();
          CHECK_EQ(data.size(), data_size) << "Incorrect data field size " << data.size();
        }
      }
      // sequential
      //string key_str = caffe::format_int(line_id, 8) + "_" + lines[line_id].first;
      string key_str = image_list[i];

      // Put in db
      string out;
      CHECK(datum.SerializeToString(&out));
      txn->Put(key_str, out);

      if (++count % 1000 == 0) {
        std::cout << an << std::endl;
        std::cout << img_prefix + image_name << " " << label  << std::endl;
        // Commit db
        txn->Commit();
        txn.reset(db->NewTransaction());
        LOG(INFO) << "Processed " << count << " files.";
      }
    }

    // write the last batch
    if (count % 100 != 0) {
      txn->Commit();
      LOG(INFO) << "Processed " << count << " files.";
    }     
  
  }
}
