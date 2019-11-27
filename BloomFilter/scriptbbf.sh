if [ $1 = "build" ]
then
./bbf $1 $3 $5 $7 $9

elif [ $1 = "query" ]
then

./bbf $1 $3 $5
elif [ $1 = "query1" ]
then

./bbf $1 $3 $5 $6 $7

fi
