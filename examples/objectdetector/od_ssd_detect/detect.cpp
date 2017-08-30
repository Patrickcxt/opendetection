#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include "common/datasets/ODDatasets.h"
#include "common/datasets/PascalVOC.h"
#include "common/datasets/MSCoco.h"
#include "common/datasets/ImageNet.h"

#include <algorithm>
#include <fstream>  // NOLINT(readability/streams)
#include <string>
#include <utility>
#include <vector>
#include <unistd.h>
#include <fcntl.h>

#include "boost/scoped_ptr.hpp"
#include "gflags/gflags.h"
#include "glog/logging.h"


using namespace std;


int main(int argc, char* argv[]) {

    string p1 = "/home/amax/cxt/data/pascal_voc/";
    od::PascalVOC pascal = od::PascalVOC(p1, 1);
    vector<string> test_images = pascal.getTestImageList();
 
    string ssd_dir = argv[1];
    string model_def = argv[2];
    string snapshot_path = argv[3];
    string data_dir = argv[4];

    // create image_list file that you want to detect
    ofstream outfile(data_dir + "image_list.txt");
    string test_path = "/home/amax/cxt/data/pascal_voc/test/VOC2007/JPEGImages/";
    for (int i = 20; i < 1000; i += 50)
    {
        cout << test_images[i] << endl;
        outfile << test_path + test_images[i] + ".jpg" << endl;
    }
    outfile.close();

    // call ssd_detect.cpp to detect the above images
    if ((access((data_dir+"result").c_str(), F_OK)) == -1)
        system(("mkdir " + data_dir + "result").c_str());

    string cmd_sp1 = "cd " + ssd_dir + " && ./build/examples/ssd/ssd_detect " + model_def + " " + snapshot_path + " "
        + data_dir+"image_list.txt "  + " -confidence_threshold 0.5 -out_file " + data_dir + "result/output";

    cout << cmd_sp1 << endl;
    FILE* sp1 = popen(cmd_sp1.c_str(), "r");
    if (!sp1)
    {
        cout << "call ssd_detect failed."  << endl;
        return -1;
    }
    pclose(sp1);

    string cmd_sp2 = "python " + ssd_dir+"examples/ssd/plot_detections.py --labelmap-file " + ssd_dir+"data/VOC0712/labelmap_voc.prototxt --visualize-threshold 0.5 --save-dir " + data_dir+"./result/ " + data_dir+"./result/output " + "/";
    cout << cmd_sp2 << endl;
    FILE* sp2 = popen(cmd_sp2.c_str(), "r");
    if (!sp2)
    {
        cout << "execute error!" << endl;
        return -1; 
    }
    pclose(sp2);

    return 0;
}
