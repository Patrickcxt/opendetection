#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "common/datasets/ODDatasets.h"
#include "common/datasets/PascalVOC.h"
#include "common/datasets/MSCoco.h"
#include "common/datasets/ImageNet.h"
//#include "caffe/util/db.hpp"

/*
#include "rapidjson/document.h"
using namespace rapidjson;
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml_utils.hpp"
#include "boost/filesystem.hpp"

using namespace rapidxml;
using namespace boost::filesystem;
*/

#include <algorithm>
#include <fstream>  // NOLINT(readability/streams)
#include <string>
#include <utility>
#include <vector>

#include "boost/scoped_ptr.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"

/*
#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include "caffe/util/format.hpp"
#include "caffe/util/io.hpp"
#include "caffe/util/rng.hpp"
*/

using namespace std;
//using namespace caffe;


int main(int argc, char* argv[]) {

    string p1 = "/home/amax/cxt/data/pascal_voc/";
    od::PascalVOC pascal = od::PascalVOC(p1, 1);
    vector<string> test_images = pascal.getTestImageList();

    string current_root = "/home/amax/cxt/open_detection/opendetection/mybuild/examples/objectdetector/od_ssd_detect/";
    string ssd_root = "/home/amax/cxt/open_detection/packages/ssd/";

    // create image_list file that you want to detect
    ofstream outfile(current_root + "image_list.txt");
    string test_path = "/home/amax/cxt/data/pascal_voc/test/VOC2007/JPEGImages/";
    for (int i = 20; i < 1000; i += 50)
    {
        cout << test_images[i] << endl;
        outfile << test_path + test_images[i] + ".jpg" << endl;
    }
    outfile.close();

    // call ssd_detect.cpp to detect the above images
    string cmd_sp1 = "cd " + ssd_root + " && ./build/examples/ssd/ssd_detect ./models/VGGNet/VOC0712/SSD_300x300/deploy.prototxt ./models/VGGNet/VOC0712/SSD_300x300/VGG_VOC0712_SSD_300x300_iter_60000.caffemodel " + current_root + "image_list.txt " + " -confidence_threshold 0.5 -out_file " + current_root + "/save/result";
    cout << cmd_sp1 << endl;
    //string cmd = "cd " + ssd_root + " && ./build/examples/ssd/ssd_detect ./models/VGGNet/VOC0712/SSD_300x300/deploy.prototxt ./models/VGGNet/VOC0712/SSD_300x300/VGG_VOC0712_SSD_300x300_iter_60000.caffemodel ./models/VGGNet/VOC0712/imagelist.txt -confidence_threshold 0.5 -out_file ./models/VGGNet/VOC0712/save/result";
    FILE* sp1 = popen(cmd_sp1.c_str(), "r");
    if (!sp1)
    {
        cout << "call ssd_detect failed."  << endl;
        return -1;
    }
    pclose(sp1);
    string cmd_sp2 = "python " + ssd_root+"examples/ssd/plot_detections.py --labelmap-file " + ssd_root+"data/VOC0712/labelmap_voc.prototxt --visualize-threshold 0.5 --save-dir " + current_root+"./save/ " + current_root+"./save/result " + "/";
    //string cmd = "python " + ssd_root+"examples/ssd/plot_detections.py --labelmap-file " + ssd_root+"data/VOC0712/labelmap_voc.prototxt --visualize-threshold 0.5 --save-dir " + "./save/ " + ssd_root+"./models/VGGNet/VOC0712/save/result " + "/";

    
    cout << cmd_sp2 << endl;
    FILE* sp2 = popen(cmd_sp2.c_str(), "r");
    if (!sp2)
    {
        cout << "execute error!" << endl;
        return -1; 
    }
    pclose(sp2);

    /*
    cout << "Loading Pascal VOC dataset ..." << endl;
    string p1 = "/home/amax/cxt/data/pascal_voc/";
    //string p1 = "../data/pascal_voc/";
    od::PascalVOC pascal = od::PascalVOC(p1, 1);
    cout << "Dataset name: " << pascal.getDatasetName() << endl;
    cout << "Number of categories: " << pascal.getNumOfClasses() << endl;
    cout << "Number of classlist: " << pascal.getClassesList().size() << endl;
    cout << "Number of training images: " << pascal.getTrainImageList().size() << endl;
    cout << "Number of validation images: " << pascal.getValImageList().size() << endl;
    cout << "Number of trainging and validation images: " << pascal.getTrainvalImageList().size() << endl;
    cout << "Number of test images: " << pascal.getTestImageList().size() << endl; 
    cout << "Number of annotated images: " << pascal.getAllAnnotations().size() << endl; 
    cout << "Annotation of image 000005.jpg: " << endl;
    od::ODAnnotation a1 = pascal.getAnnotationByName("000005");
    cout << a1 << endl;
    cout << endl;

    // convert dataset to lmdb.
    pascal.convertDatasetToLmdb("trainval", "/media/amax/cxt/data/Detection/lmdb/pascal/pascal_trainval_enc");
    pascal.convertDatasetToLmdb("test", "/media/amax/cxt/data/Detection/lmdb/pascal/pascal_test_enc");
    */


    /*
    cout << "Loading Tiny-imagenet dataset ..." << endl;
    string p2 = "/home/amax/cxt/data/tiny-imagenet-200/";
    //string p2 = "../data/tiny-imagenet-200/";
    od::ImageNet imagenet = od::ImageNet(p2, 1);  // 0 for classification, 1 for detection
    cout << "Dataset name: " << imagenet.getDatasetName() << endl;
    cout << "Number of categories: " << imagenet.getNumOfClasses() << endl; cout << "Number of classlist: " << imagenet.getClassesList().size() << endl;
    cout << "Number of training images: " << imagenet.getTrainImageList().size() << endl;
    cout << "Number of validation images: " << imagenet.getValImageList().size() << endl;
    cout << "Number of trainging and validation images: " << imagenet.getTrainvalImageList().size() << endl;
    cout << "Number of test images: " << imagenet.getTestImageList().size() << endl;
    cout << "Number of annotated images: " << imagenet.getAllAnnotations().size() << endl;
    cout << "Annotation of image val_0.JPEG: " << endl;
    od::ODAnnotation a2 = imagenet.getAnnotationByName("val_0.JPEG");
    cout << a2 << endl;
    
    map<int, string> cates = imagenet.getClassesList();
    for (map<int, string>::iterator it = cates.begin(); it != cates.end(); it++)
        cout << it->first << " "  << it->second << endl;
    cout << endl;

    imagenet.convertDatasetToLmdb("train", "/media/amax/cxt/data/Detection/lmdb/imagenet/train_lmdb_dec");
    imagenet.convertDatasetToLmdb("val", "/media/amax/cxt/data/Detection/lmdb/imagenet/val_lmdb_dec");
    */


    /*
    //  the json file is too large to push to github.
    string p3 = "/media/amax/Seagate Backup Plus Drive/cxt/data/MSCOCO/";
    //string p3 = "./data/MSCOCO/";
    od::MSCoco coco = od::MSCoco(p3, 1);
    cout << "Dataset name: " << coco.getDatasetName() << endl;
    cout << "Number of categories: " << coco.getNumOfClasses() << endl;
    cout << "Number of classlist: " << coco.getClassesList().size() << endl;
    cout << "Number of training images: " << coco.getTrainImageList().size() << endl;
    cout << "Number of validation images: " << coco.getValImageList().size() << endl;
    cout << "Number of trainging and validation images: " << coco.getTrainvalImageList().size() << endl;
    cout << "Number of test images: " << coco.getTestImageList().size() << endl;
    cout << "Number of annotated images: " << coco.getAllAnnotations().size() << endl;
    cout << "Annotation of image 233116.jpg: " << endl;
    od::ODAnnotation a3 = coco.getAnnotationByName("COCO_train2014_000000233116");
    cout << a3 << endl;
    getchar();
    cout << endl;

    coco.convertDatasetToLmdb("train", "/media/amax/cxt/data/Detection/lmdb/mscoco/train_lmdb_dec");
    //coco.convertDatasetToLmdb("val", "/media/amax/cxt/data/Detection/lmdb/mscoco/val_lmdb_dec");
    */

    return 0;
}
