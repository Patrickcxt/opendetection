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
#ifdef USE_CAFFE
    cout << "Found" << endl;
#else
    cout << "Not Found" << endl;
#endif
    /*
    string p1 = "/home/amax/cxt/data/tiny-imagenet-200/";
    od::ImageNet imagenet = od::ImageNet(p1, 0);
    vector<string> test_images = imagenet.getTestImageList();


    string caffe_dir = argv[1];
    string model_def = argv[2];
    string snapshot_path = argv[3];
    string mean_path = argv[4];
    string map_file = argv[5];
    string image_dir = p1 + "test/images/";

    string cmd = caffe_dir + "./build/examples/cpp_classification/classification.bin " +
        model_def + " " + snapshot_path + " " + mean_path + " " + map_file + " " + image_dir;


    for (int i = 0; i < 100; i+= 10)
    {
        string new_cmd = cmd + test_images[i];
        cout << new_cmd << endl;
        FILE* sp = popen(new_cmd.c_str(), "r");
        if (!sp)
        {
            cout << "call ssd_detect failed."  << endl;
            return -1;
        }
        char buff[1024];
        while (fgets(buff, sizeof(buff), sp) != NULL)
        {
            cout << buff << endl; 
        }
        pclose(sp);
        cout << "--------------------------------------------------------" << endl;
    }
    */


    return 0;
}
