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
     * example to prepare necessary data to train ssd on Imagenet.
     */
    cout << "Loading tiny-imagenet-200 dataset ..." << endl;
    string p1 = "/home/amax/cxt/data/tiny-imagenet-200/";
    od::ImageNet imagenet = od::ImageNet(p1, 0);


    string data_dir = argv[1];
    string ssd_dir = "/home/amax/cxt/open_detection/packages/ssd/";
    string sub_dir = "data/VOC0712/";

    // create lmdb file
    cout << "creating lmdb for train subset" << endl;
    if ((access((data_dir+"train_cls").c_str(), F_OK)) == -1)
    {
        imagenet.convertDatasetToLmdb("train", data_dir+"train_cls", 256, 256);
    }
    else
    {
        cout << "train lmdb exists!" << endl;
    }

    cout << "creating lmdb for val subset" << endl;
    if ((access((data_dir+"val_cls").c_str(), F_OK)) == -1)
    {
        imagenet.convertDatasetToLmdb("val", data_dir+"val_cls", 256, 256);
    }
    else
    {
        cout << "val lmdb exists!" << endl;
    }

    // move trainval.txt, test.txt and test_name_size.txt to ssd_dir/sub_dir
    cout << "creating image mean ..." << endl;
    if ((access((data_dir+"imagenet_mean.binaryproto").c_str(), F_OK)) == -1)
    {
        string cmd = "cd "  + ssd_dir + " && ./build/tools/compute_image_mean " + data_dir+"train_cls " + data_dir+"imagenet_mean.binaryproto";
        FILE* p = popen(cmd.c_str(), "r");
        if (!p)
        {
            cout << "execute error!" << endl;
            return -1;
        }
        pclose(p);
    }
    else
    {
        cout << "imagenet_mean.binaryproto exists!"  << endl;
    }

    return 0;
}
