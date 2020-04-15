if [ ! -d "./build" ];then
echo "Create build folder..."
mkdir build
else
rm -r build
echo "Create build folder..."
mkdir build
fi

cd build
cmake ..
make
