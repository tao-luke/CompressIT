# FlexiPress
---
Remember that one email attachment or discord file that just won't fit to be sent? **It sucks!** With more data being transmitted online than ever, size issues are becoming an ever more painful daily occurance. Although we do have handy general compression techniques like zip, rar, and lossy formats like jpeg, the fault lies in that their operations are already pre-determined. This means that some operations applied on the data might not have been worth the wait, and some operations might have been missing when it could have easily saved you that excessive 5% !

*FlexiPress* is a compression program that offers the full range of freedom to how your data is handled. Seeing a large discrepancy of strings but using ascii chars outside of the 26 alphabet? perhaps you would benefit more from Burrows- Wheeler transformation than Lempel–Ziv–Welch. And with library usage no more than std C++ lib, this would perform on almost all devices available. 

## Usage:
- Compile program:
```
make
```
- General format for compression and decompression:
```
./FlexiPress <mode> <transformation> < inputflow
(or)
./FlexiPress <mode> <transformation> inputFile
```
- Compress `test.txt` with bzip2 sequence (b-burrows wheeler, m-move to front, r-runlength encoding in 2's bijective numeration.. feel free to add more). This produces a file called `test.dat`. If an inputflow is given instead, the program defaults to file `noName.dat`
```
./FlexiPress -encode -bmr test.txt
```
- Decode `test.dat`
```
./FlexiPress -decode < test.dat
```

## Example:
- To compress using zip, one sequence would be -lmr(l-lempel_ziv, m-move to front, r-run length encoding). This would yield around 55% deflation rate with one of my favorite poem, "The Hill We Climb" by Amanda Gorman. Another obvious choice would be to use bzip2, -bmr, which yields around 58% delation rate.

## Looking Ahead:
- Implement Jpeg transformations to offer lossy data handling
- Optimize a ML model to automatically predict best path of compression
- Extend to Video compression! 
