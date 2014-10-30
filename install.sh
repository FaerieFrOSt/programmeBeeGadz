cd libnfc-1.7.1
autoreconf -vis
./configure --enable-doc
make
sudo make install
cd ..
cp libnfc-1.7.1/libnfc/.libs/libnfc.a libnfc/
sudo apt-get install libmysqlclient-dev
