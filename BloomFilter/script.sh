#! /bin/bash
#g++ -o rank2 Source.cpp #-i headers -I folders 
#$bf build -k <key file> -f <fpr> -n <num. distinct keys> -o <output file>
#g++ -std=c++11 Source.cpp Wavelet/wavelet.cpp Select/Select.cpp Rank/Rank.cpp -o src -D_GLIBCXX_USE_CXX11_ABI=1
#$bf query -i <input file> -q <queries>
#g++  -std=c++14 Source.cpp Headers/MurmurHash3.cpp -o src;

echo "$1"
if [ $1 = "build" ]
then
./src $1 $3 $5 $7 $9

elif [ $1 = "query" ]
then

./src $1 $3 $5
elif [ $1 = "query1" ]
then

./src $1 $3 $5 $6 $7

fi
#./src $1 $2 $3
#echo "$#"

# bf build -k keyfile -f 0.2 -n 5 -o output
#bf query -i output -q keyfile