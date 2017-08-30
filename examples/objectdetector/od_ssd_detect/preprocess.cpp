#include <iostream>
#include <fstream>
#include <sstream>
#include "common/datasets/ODDatasets.h"
#include "common/datasets/PascalVOC.h"
#include "common/datasets/MSCoco.h"
#include "common/datasets/ImageNet.h"
#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>

using namespace std;

int main(int argc, char* argv[]) {

    /*
     * example to prepare necessary data to train ssd on Pascal VOC 2007.
     */
    cout << "Loading Pascal VOC dataset ..." << endl;
    string p1 = "/home/amax/cxt/data/pascal_voc/";
    od::PascalVOC pascal = od::PascalVOC(p1, 1);


    //string data_dir = "./data/Detection/pascal/";
    //string ssd_dir = "/home/amax/cxt/open_detection/packages/ssd/";
    string ssd_dir = argv[1];
    string data_dir = argv[2];
    //string sub_dir = "data/VOC0712/";
    // create lmdb file
    cout << "creating lmdb for trainval subset" << endl;
    if ((access((data_dir+"pascal_trainval_dec").c_str(), F_OK)) == -1)
    {
        pascal.convertDatasetToLmdb("trainval", data_dir+"pascal_trainval_dec");
    }
    else
    {
        cout << "trainval lmdb exists!" << endl;
    }

    cout << "creating lmdb for test subset" << endl;
    if ((access((data_dir+"pascal_test_dec").c_str(), F_OK)) == -1)
    {
        pascal.convertDatasetToLmdb("test", data_dir+"pascal_test_dec");
    }
    else
    {
        cout << "test lmdb exists!" << endl;
    }

    // move trainval.txt, test.txt and test_name_size.txt to ssd_dir/sub_dir
    
    cout << "copying trainval.txt" << endl;
    if ((access((data_dir+"trainval.txt").c_str(), F_OK)) == -1)
    {
        string src = "/home/amax/cxt/data/pascal_voc/trainval/VOC2007/ImageSets/Main/trainval.txt";
        string dst = data_dir + "trainval.txt";
        string cmd = "cp " + src + " " + dst;
        system(cmd.c_str());
    }
    else
    {
        cout << "trainval.txt exists!"  << endl;
    }


    cout << "copying test.txt" << endl;
    if ((access((data_dir+"test.txt").c_str(), F_OK)) == -1)
    {
        string src = "/home/amax/cxt/data/pascal_voc/test/VOC2007/ImageSets/Main/test.txt";
        string dst = data_dir + "test.txt";
        string cmd = "cp " + src + " " + dst;
        system(cmd.c_str());
    }
    else
    {
        cout << "test.txt exists!"  << endl;
    }

    // shuffle trainval.txt

    // create image size list (test_name_size.txt list) for test set.
    cout << "creating test_name_size.txt" << endl;
    if ((access((data_dir+"test_name_size.txt").c_str(), F_OK)) == -1)
    {
        vector<string> test_list = pascal.getTestImageList();
        map<string, od::ODAnnotation> annotations = pascal.getAllAnnotations();
        string name_size_file = data_dir + "test_name_size.txt";
        ofstream outfile(name_size_file);
        if (!outfile.good())
        {
          cerr  << "Failed to open file: " << name_size_file << endl;
        }
        for (int i = 0; i < test_list.size(); i++)
        {
          //string image_name = test_list[i];
          od::ODAnnotation an = annotations[test_list[i]];
          outfile << test_list[i] << " " << an.height_ << " " << an.width_ << endl;
        }
        outfile.close();
    }
    else
    {
        cout << "test_name_size.txt exists!" << endl;
    }

    return 0;
}
