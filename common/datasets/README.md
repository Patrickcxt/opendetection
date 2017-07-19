The rapidxml and rapidjson packages have been moved to the /root/3rdparty/. The example of loading datasets is provided in /root/examples/objectdetector/od_load_dataset.cpp. Compile the whole project and ./examples/objectdetector/od_load_dataset to test the code.

To simply test the code in this folder, modify included path of rapidxml and rapidjson in PascalVOC.cpp and MSCoco.h.
For example, change "3rdparty/rapidxml/..." to "../../3rdparty/rapidxml/..".
run "g++ test.cpp PascalVOC.cpp ODDatasets.cpp MSCoco.cpp ImageNet.cpp -lboost_system -lboost_filesystem
" to compile the code.
