OpenDetection
=============

OpenDetection is a standalone open source project for object detection and recognition in images and 3D point clouds.

Website - http://opendetection.com or http://krips89.github.io/opendetection_docs

Git - https://github.com/krips89/opendetection

Getting started - http://opendetection.com/getting_started.html

User guide - http://opendetection.com/introduction_general.html

API Documentation - http://opendetection.com/annotated.html

Short demo video - https://www.youtube.com/watch?v=sp8W0NspY54


=============
Code to provide unified api for datasets, eg. PasvalVOC, ImageNet, MSCOCO
Main code is located in common/datasets/

We provide examples to use these api (build the opendetedtion project first).

1. Apis we provide:
cd build
./examples/objectdetector/od_load_dataset


2. Example to using SSD to perform object detection on PascalVOC 2007
cd build
# prepare lmdb and files needed by ssd.
./examples/objectdetector/od_ssd_detect/preprocess  ../data/Detection/pascal/
cp ../data/Detection/pascal/\*.txt ssd_dir/data/VOC0712/
# train and test ssd in ssd directory.
cd ssd_root
python examples/ssd/ssd_pascal.py
python examples/ssd/score_ssd_pascal.py
# perform detectition using the trained model.
cd build
./examples/objectdetector/od_ssd_detect/detect





