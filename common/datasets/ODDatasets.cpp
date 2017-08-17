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

/*
#include <boost/filesystem.hpp>
#include "boost/scoped_ptr.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include "caffe/util/format.hpp"
#include "caffe/util/io.hpp"
#include "caffe/util/rng.hpp"
*/

using namespace caffe;
using std::pair;
using boost::scoped_ptr;

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

  std::vector<std::string> ODDataset::get_files_in_directory(std::string base_path, bool is_folder, std::string ext, bool is_truncated)
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
      {
        if (is_truncated)
        {
          std::string path = itr->path().string();
          std::size_t pos = path.rfind("/");
          if (pos != std::string::npos)
              path = path.substr(pos+1);
          files.push_back(path);
        }
        else
        {
          files.push_back(itr->path().string());
        }
      }
      else
      {
        files.push_back(itr->path().string());
      }
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
   * This function is modified from "caffe(ssd)/tools/convert_annoset.cpp"
   */
  void ODDataset::convert_dataset_to_lmdb_detection(std::vector<std::string> image_list, std::string img_prefix,
      std::string save_dir, int resize_height, int resize_width, std::string encode_type) {

    std::map<std::string, ODAnnotation> anns = annotations_;

    const bool is_color = true;
    const bool check_size = false;
    const bool encoded = false;
    const std::string anno_type = (task_type_ == 0) ? "classification" : "detection";
    const std::string backend = "lmdb";
    AnnotatedDatum_AnnotationType type;
    const bool check_label = false;
    int min_dim = 0;
    int max_dim = 0;

    std::string filename;
    int label;

    /*
    if (FLAGS_shuffle) {
      // randomly shuffle data
      LOG(INFO) << "Shuffling data";
      shuffle(lines.begin(), lines.end());
    }
    LOG(INFO) << "A total of " << lines.size() << " images.";
    */

    if (encode_type.size() && !encoded)
      LOG(INFO) << "encode_type specified, assuming encoded=true.";


    // Create new DB
    scoped_ptr<db::DB> db(db::GetDB(backend));
    db->Open(save_dir, db::NEW);
    scoped_ptr<db::Transaction> txn(db->NewTransaction());

    // Storing to db
    AnnotatedDatum anno_datum;
    Datum* datum = anno_datum.mutable_datum();
    int count = 0;
    int data_size = 0;
    bool data_size_initialized = false;

    for (int id = 0; id < image_list.size(); id++) {
      bool status = true;
      std::string enc = encode_type;
      std::cout << image_list[id] << std::endl;
      if (encoded && !enc.size()) {
        // Guess the encoding type from the file name
        std::string fn = image_list[id];
        size_t p = fn.rfind('.');
        if ( p == fn.npos )
          LOG(WARNING) << "Failed to guess the encoding of '" << fn << "'";
        enc = fn.substr(p);
        std::transform(enc.begin(), enc.end(), enc.begin(), ::tolower);
      }
      filename = img_prefix + image_list[id] + enc;
      std::cout << filename << std::endl;
      od::ODAnnotation odan = anns[image_list[id]];
      std::cout << "-------------------------" << std::endl;
      std::cout << odan << std::endl;

      if (anno_type == "classification") {
        if (name_ != "ImageNet") {
            std::cerr << name_ << " dataset is not supported! Only support single-label classification for ImageNet dataset now!";
            continue;
        }
        ODObject object = odan.objects_[0];
        label = object.label_;
        status = ReadImageToDatum(filename, label, resize_height, resize_width,
            min_dim, max_dim, is_color, enc, datum);
      } else if (anno_type == "detection") {
        status = ReadImageToDatum(filename, -1, resize_height, resize_width,
            min_dim, max_dim, is_color, enc, datum);
        if (status == true) {
          anno_datum.clear_annotation_group();
          int ori_height, ori_width;
          GetImageSize(filename, &ori_height, &ori_width);
          std::cout << "-----------------------------" << std::endl;
          std::cout << ori_height << " " << ori_width << std::endl;
          status = read_bbox_to_annotated_datum(filename, odan, ori_height, ori_width,  &anno_datum);
          anno_datum.set_type(AnnotatedDatum_AnnotationType_BBOX);
        }
      }
      if (status == false) {
        LOG(WARNING) << "Failed to read " << image_list[id];
        continue;
      }
      std::cout << datum->channels() << " " << datum->height() << " " << datum->width() << std::endl;
      std::cout << (datum->data()).size() << std::endl;
      if (check_size) {
        if (!data_size_initialized) {
          data_size = datum->channels() * datum->height() * datum->width();
          data_size_initialized = true;
        } else {
          const std::string& data = datum->data();
          CHECK_EQ(data.size(), data_size) << "Incorrect data field size "
              << data.size();
        }
      }
      // sequential
      std::string key_str = caffe::format_int(id, 8) + "_" + image_list[id];

      // Put in db
      std::string out;
      CHECK(anno_datum.SerializeToString(&out));
      txn->Put(key_str, out);

      if (++count % 1000 == 0) {
        // Commit db
        txn->Commit();
        txn.reset(db->NewTransaction());
        LOG(INFO) << "Processed " << count << " files.";
      }
    }
    // write the last batch
    
    if (count % 1000 != 0) {
      txn->Commit();
      LOG(INFO) << "Processed " << count << " files.";
    }
  }

  /**
   * This function is modified from "caffe(ssd)/util/io.cpp/ReadXMLFromAnnotatedDatum
   */
  bool ODDataset::read_bbox_to_annotated_datum(const std::string filename, od::ODAnnotation annotation, const int img_height, const int img_width,  AnnotatedDatum* anno_datum) {

    int height = annotation.height_;
    int width = annotation.width_;
    std::cout << height << " " << width << std::endl;
    std::cout << "-----------------------------" << std::endl;
    LOG_IF(WARNING, height != img_height) << filename <<  " inconsistent image height.";
    LOG_IF(WARNING, width != img_width) << filename <<  " inconsistent image width.";
    CHECK(width != 0 && height != 0) << filename << " no valid image width/height.";
    int instance_id = 0;
    vector<od::ODObject> objects = annotation.objects_;
    for (vector<od::ODObject>::iterator it = objects.begin(); it != objects.end(); it++) {
      Annotation* anno = NULL;
      bool difficult = false;
      od::ODObject object = *it;

      /* find corresponding category group */
      int label = object.label_;
      bool found_group = false;
      for (int g = 0; g < anno_datum->annotation_group_size(); ++g) {
        AnnotationGroup* anno_group = anno_datum->mutable_annotation_group(g);
        if (label == anno_group->group_label()) {
          if (anno_group->annotation_size() == 0) {
            instance_id = 0;
          } else {
            instance_id = anno_group->annotation(
                anno_group->annotation_size() - 1).instance_id() + 1;
          }
          anno = anno_group->add_annotation();
          found_group = true;
        }
      }
      if (!found_group) {
        // If there is no such annotation_group, create a new one.
        AnnotationGroup* anno_group = anno_datum->add_annotation_group();
        anno_group->set_group_label(label);
        anno = anno_group->add_annotation();
        instance_id = 0;
      }

      anno->set_instance_id(instance_id++); 
      /*  difficult */
      difficult = object.is_difficult_;

      /* bounding box */
      int xmin = object.bbox_[0];
      int ymin = object.bbox_[1];
      int xmax = object.bbox_[2];
      int ymax = object.bbox_[3];
      CHECK_NOTNULL(anno);
      LOG_IF(WARNING, xmin > width) << filename <<
            " bounding box exceeds image boundary.";
      LOG_IF(WARNING, ymin > height) << filename <<
            " bounding box exceeds image boundary.";
      LOG_IF(WARNING, xmax > width) << filename <<
            " bounding box exceeds image boundary.";
      LOG_IF(WARNING, ymax > height) << filename <<
            " bounding box exceeds image boundary.";
      LOG_IF(WARNING, xmin < 0) << filename <<
            " bounding box exceeds image boundary.";
      LOG_IF(WARNING, ymin < 0) << filename <<
            " bounding box exceeds image boundary.";
      LOG_IF(WARNING, xmax < 0) << filename <<
            " bounding box exceeds image boundary.";
      LOG_IF(WARNING, ymax < 0) << filename <<
            " bounding box exceeds image boundary.";
      LOG_IF(WARNING, xmin > xmax) << filename <<
            " bounding box irregular.";
      LOG_IF(WARNING, ymin > ymax) << filename <<
            " bounding box irregular.";
        // Store the normalized bounding box.
      NormalizedBBox* bbox = anno->mutable_bbox();
      bbox->set_xmin(static_cast<float>(xmin) / width);
      bbox->set_ymin(static_cast<float>(ymin) / height);
      bbox->set_xmax(static_cast<float>(xmax) / width);
      bbox->set_ymax(static_cast<float>(ymax) / height);
      bbox->set_difficult(difficult);
    }
    return true;
  }


  /**
   * This code is modified form convert_imageset.cpp of caffe.
   */
  /*
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

      //  load image to datum
      std::vector<std::string> items = split(image_list[i], '/');
      std::string image_name = items[items.size()-1];
      od::ODAnnotation an = anns[image_name];
      int label = an.objects_[0].label_;
      status = ReadImageToDatum(img_prefix + image_list[i], label, resize_height, resize_width, is_color,
        enc, &datum);


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
  */
}
