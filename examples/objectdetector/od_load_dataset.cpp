#include <iostream>
#include <fstream>
#include <sstream>
#include "common/datasets/ODDatasets.h"
#include "common/datasets/PascalVOC.h"
#include "common/datasets/MSCoco.h"
#include "common/datasets/ImageNet.h"

using namespace std;

int main() {

    cout << "Loading Pascal VOC dataset ..." << endl;
    string p1 = "/home/amax/cxt/data/pascal_voc/";
    od::PascalVOC pascal = od::PascalVOC(p1, 0);
    cout << "Dataset name: " << pascal.getDatasetName() << endl;
    cout << "Number of categories: " << pascal.getNumOfClasses() << endl;
    cout << "Number of classlist: " << pascal.getClassesList().size() << endl;
    cout << "Number of training images: " << pascal.getTrainImageList().size() << endl;
    cout << "Number of validation images: " << pascal.getValImageList().size() << endl;
    cout << "Number of trainging and validation images: " << pascal.getTrainvalImageList().size() << endl;
    cout << "Number of test images: " << pascal.getTestImageList().size() << endl;
    cout << "Number of annotated images: " << pascal.getAllAnnotations().size() << endl;
    cout << "Annotation of image 000005.jpg: " << endl;
    od::ODAnnotation a = pascal.getAnnotationByName("000005");
    cout << a << endl;
    cout << endl;

    cout << "Loading Tiny-imagenet dataset ..." << endl;
    string p2 = "/home/amax/cxt/data/tiny-imagenet-200/";
    od::ImageNet imagenet = od::ImageNet(p2, 0);
    cout << "Dataset name: " << imagenet.getDatasetName() << endl;
    cout << "Number of categories: " << imagenet.getNumOfClasses() << endl;
    cout << "Number of classlist: " << imagenet.getClassesList().size() << endl;
    cout << "Number of training images: " << imagenet.getTrainImageList().size() << endl;
    cout << "Number of validation images: " << imagenet.getValImageList().size() << endl;
    cout << "Number of trainging and validation images: " << imagenet.getTrainvalImageList().size() << endl;
    cout << "Number of test images: " << imagenet.getTestImageList().size() << endl;
    cout << "Number of annotated images: " << imagenet.getAllAnnotations().size() << endl;
    cout << "Annotation of image val_6139.JPEG: " << endl;
    a = imagenet.getAnnotationByName("val_6139.JPEG");
    cout << a << endl;
    cout << endl;

    string p3 = "/media/amax/Seagate Backup Plus Drive/cxt/data/MSCOCO/";
    od::MSCoco coco = od::MSCoco(p3, 0);
    cout << "Dataset name: " << coco.getDatasetName() << endl;
    cout << "Number of categories: " << coco.getNumOfClasses() << endl;
    cout << "Number of classlist: " << coco.getClassesList().size() << endl;
    cout << "Number of training images: " << coco.getTrainImageList().size() << endl;
    cout << "Number of validation images: " << coco.getValImageList().size() << endl;
    cout << "Number of trainging and validation images: " << coco.getTrainvalImageList().size() << endl;
    cout << "Number of test images: " << coco.getTestImageList().size() << endl;
    cout << "Number of annotated images: " << coco.getAllAnnotations().size() << endl;
    cout << "Annotation of image 233116.jpg: " << endl;
    a = coco.getAnnotationByName("233116");
    cout << a << endl;
    cout << endl;

    return 0;
}
