echo "v2.0 raw" > $1.hex
./sqasm < $1.subleq | tr -s ' ' '\n' > tmp.txt 
cat tmp.txt | while read line; 
do 
	v=$(printf "%x" $line)
	echo "${v:0:6}" >> $1.hex
done
