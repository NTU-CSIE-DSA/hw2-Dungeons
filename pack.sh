rm -rf ./package/
mkdir ./package
mkdir ./package/testdata
cp prob.md ./package/prob.md
cp ./tests/*.in ./package/testdata/
cp ./tests/*.out ./package/testdata/
./packdata.exe 3 2262144

rm -f ./package.tar.gz
tar zcvf package.tar.gz package