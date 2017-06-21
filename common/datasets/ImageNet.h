//
// Created by chenxt.
//


#ifndef OPENDETECTION_ODDATASETS_H
#define OPENDETECTION_ODDATASETS_H

namespace od {

/** \brief ImageNet dataset (ILSVRC).
   *
   * \author Xiongtao Chen
   *
   */
  class ImageNet: public ODDataset
  {
  public:
    ImageNet(int task_type, std::string image_path, std::string annotation_path) : 
        ODDataset("ImageNet", task_type, image_path, annotation_path)
    { 
    }

    // override
    void loadImageLists() {}  

    // override
    void loadAnnotations() {}

    void evaluation() {}
    ~ImageNet() {}


  };
}
#endif //OPENDETECTION_ODDATASETS_H
