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

    string ssd_root = argv[1];
    string cmd = "cd " + ssd_root + " && python examples/ssd/my_ssd_pascal.py";
    
    cout << cmd << endl;
    FILE* sp1 = popen(cmd.c_str(), "r");
    if (!sp1)
    {
        cout << "call ssd_detect failed."  << endl;
        return -1;
    }
    pclose(sp1);

    return 0;
}
