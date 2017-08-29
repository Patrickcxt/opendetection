OpenDetection
=============

OpenDetection is a standalone open source project for object detection and recognition in images and 3D point clouds.

Website - http://opendetection.com or http://krips89.github.io/opendetection_docs

Git - https://github.com/krips89/opendetection

Getting started - http://opendetection.com/getting_started.html

User guide - http://opendetection.com/introduction_general.html

API Documentation - http://opendetection.com/annotated.html

Short demo video - https://www.youtube.com/watch?v=sp8W0NspY54


Od Datasets
=============

Code to provide unified api for datasets, eg. PascalVOC, ImageNet, MSCOCO.

Main code is located in common/datasets/.

### Preparation
1. To produce lmdb for object detection, now ssd-caffe(https://github.com/weiliu89/caffe/tree/ssd) is needed to install.

2. Download dataset, we now provide code for three popular datasets. Please organize the dataset as following:

Pascal VOC (2007 for example):
```Shell
voc_root
  |
  -- trainval/VOC2007
  |       |
  |       -- Annotations
  |       -- ImageSets
  |       -- JPEGImages
  |       -- SegmentationClass
  |       -- SegmentationObject
  |
  -- test/VOC2007
         |
         -- Annotations
         -- ImageSets
         -- JPEGImages
         -- SegmentationClass
         -- SegmentationObject
```


ImageNet (tiny-imagenet for example: https://tiny-imagenet.herokuapp.com, no need to modify after downloading)
```Shell
imagenet_root
  |
  -- train
  |     |
  |     -- n01443537
  |     -- n01629819
  |     -- ...
  -- val
  |    |
  |    -- images
  |    -- val_annotations.txt
  -- test
  |    |
  |    -- images
  -- wnids.txt
  -- words.txt
```

MSCOCO (2014 for examples)
```Shell
coco_root
  |
  | -- Annotations
  |     |
  |     -- instances
  |          |
  |          -- instances_train2014.json
  |          -- instances_val2014.json
  |     -- person_keypoints
  |     -- ...
  |
  |-- Images
  |     |
  |     -- train2014
  |     -- val2014
  |     -- test2014
```


  

### Examples
We provide examples to use these api (build the opendetedtion project first).


1. Apis we provide:
```Shell
# This examples will load the datasets of PascalVOC2007, tiny-imagenet and MSCOCO
cd build
./examples/objectdetector/od_load_dataset
```

2. Example to using SSD to perform object detection on PascalVOC 2007
```Shell
cd build
# prepare lmdb and files needed by ssd.
./examples/objectdetector/od_ssd_detect/preprocess  ../data/Detection/pascal/

# copy trainval.txt, test.txt and test_name_size.txt to ssd directorys. ssd_dir is the root directory you install your ssd-caffe
cp ../data/Detection/pascal/*.txt ssd_dir/data/VOC0712/

# train and test ssd in ssd directory.
cd ssd_root
python examples/ssd/ssd_pascal.py
python examples/ssd/score_ssd_pascal.py

# perform detectection using the trained model.
cd build
./examples/objectdetector/od_ssd_detect/detect
```

Detection examples of ssd:

<p align="center">
<img src="./data/images/000040.jpg.png" width="300px">
<img src="./data/images/000137.jpg.png" width="300px">
<img src="./data/images/000247.jpg.png" width="300px">
</p>




