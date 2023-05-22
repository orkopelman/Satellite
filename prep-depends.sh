sudo apt install -y libsqlite3-dev
git submodule init
git submodule update
mkdir -p libcsp/build
cd libcsp/build
cmake ..
make libcsp
cd ../..
