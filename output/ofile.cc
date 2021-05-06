#include "ofile.h"
size_t findPeriod(const char* str){
    size_t i = 0;
    while(str[i] != 0 && str[i] != '.'){
        i++;
        if (i > 256)
            throw Error("error writing file name");
    }
    return i;
}
void flatten(vector<long> &flatData, vector<unique_ptr<Block> > &data)
{
    for(auto && ptr: data){
        for(const auto& n: ptr->getData()){
            flatData.push_back(n);
        }
    }
    data.clear(); //free up data
}
Ofile::Ofile(vector<unique_ptr<Block> > &data, vector<long> encodeMapArr, vector<Transformation> Tseq,unsigned int originalSize,const char* FILE_NAME, size_t FILE_NAME_LENGTH):FILE_NAME_LENGTH(FILE_NAME_LENGTH),TRANSFORM_LENGTH(Tseq.size()),FILE_NAME(FILE_NAME),FILE_BYTE_COUNT(originalSize)
{
    //output as encoded file ctor
    const vector<long> encodingLengthArr = std::move(data.back()->getData());
    data.pop_back(); //pop out the encodinglength arr

    initTransformArr(Tseq);
    initHuffTrio(encodeMapArr);
    initData(data,encodingLengthArr);
    writeAsEncodedFile();
    delete[] TRANSFORM_ARR;
    delete[] HUFFTRIOS;
    delete[] dataPtr;
}
Ofile::Ofile(vector<long> &data, const char *FILE_NAME) : FILE_NAME(FILE_NAME)
{
    //raw output file ctor
    writeAsRawFile(data);
}
void Ofile::initTransformArr(vector<Transformation> &Tseq)
//sequnce of Transformation types, to indicate what transformation happened in what order
{
    unsigned int count = Tseq.size();
    char *X_NULL_RESULT = new char[count]; //create a NOT null terminated char arr
    for (size_t i = 0; i < count;i++)
    {
        X_NULL_RESULT[i] = Tseq[i];
    }

    TRANSFORM_ARR = X_NULL_RESULT;
    cout << "complete Transform Init" << endl;
}
void Ofile::insertBigChar(vector<unsigned char>& result,unsigned long n)
//insert a long data into the unsigned arr
{
    int digits = ceil(result.back()/(double)8.0);
    if (digits > 8)
        throw Error("unsupported file");
    unsigned char *buffer = new unsigned char[digits];
    memcpy(buffer, &n, digits);
    for (int i = 0; i < digits; i++)
    {
        result.push_back(buffer[i]);
    }
    delete[] buffer;
}
void Ofile::initHuffTrio(vector<long> encodeMapArr)
{
    // have the entries in order, if an entry is bigger than 1 byte, it wll be more bytes
    vector<unsigned char> result{};
    unsigned int count = encodeMapArr.size();
    for (size_t i = 0; i < count; i++)
    {
        if (i % 3 != 2)
        {
            result.push_back(encodeMapArr[i]);
        }
        else
        {
            insertBigChar(result, encodeMapArr[i]);
        }
    }
    HUFFTRIOS = new char[result.size()];
    memcpy(HUFFTRIOS, reinterpret_cast<char *>(result.data()), result.size());
    DATA_TRIO_COUNT = count / 3;
    huffbyte = result.size();
    if (count % 3 != 0)
        throw Error("encodeMparr not divisble by 3");
    cout << "complete Huff Trio init" << endl;
}
void Ofile::writeAsEncodedFile(){
    ofstream outfile;
    size_t periodIndex = findPeriod(FILE_NAME);
    char *concatName = new char[periodIndex+5]; //name(.dat) thus +4
    memcpy(concatName, FILE_NAME, periodIndex);
    concatName[periodIndex + 4] = 0;
    concatName[periodIndex + 3] = 't';
    concatName[periodIndex + 2] = 'a';
    concatName[periodIndex + 1] = 'd';
    concatName[periodIndex] = '.';

    outfile.open(concatName, ios::binary | ios::out);
    outfile.write(FILE_SIG, 2 * sizeof(char));
    outfile.write(&FILE_NAME_LENGTH, sizeof(char));
    outfile.write(&TRANSFORM_LENGTH, sizeof(char));
    outfile.write(FILE_NAME, sizeof(char) * FILE_NAME_LENGTH);
    outfile.write(TRANSFORM_ARR, sizeof(char) * TRANSFORM_LENGTH);

    // cerr << COMP_CHAR_COUNT << endl;
    // cerr << FILE_BYTE_COUNT<< endl;

    outfile.write(reinterpret_cast<char *>(&COMP_CHAR_COUNT), sizeof(COMP_CHAR_COUNT));
    outfile.write(reinterpret_cast<char *>(&FILE_BYTE_COUNT), sizeof(FILE_BYTE_COUNT));
    outfile.write(reinterpret_cast<char *>(&DATA_TRIO_COUNT), sizeof(DATA_TRIO_COUNT));
    outfile.write(HUFFTRIOS, huffbyte*sizeof(char));
    outfile.write(dataPtr, databyte*sizeof(char));
    outfile.write(FILE_SIG, 2 * sizeof(char));

    outfile.close();

    // ifstream infile;
    // infile.open("test.dat", ios::binary | ios::in);
    // unsigned int test;
    // unsigned int test2;
    // infile.read(reinterpret_cast<char *>(&test), sizeof(test));
    // infile.read(reinterpret_cast<char *>(&test2), sizeof(test));
    // cout << test << endl;
    // cout << test2 << endl;
    cout << "write complete, deflated original file by: " << ceil((1-(COMP_CHAR_COUNT/(double)FILE_BYTE_COUNT))*100) << "% in size, saved to " << concatName << endl;
    delete[] concatName;
}
void Ofile::initData(vector<unique_ptr<Block>>& data,const vector<long>&encodingLength){
    //combine as many bits as possible to byte chars! and if the bit count is NOT divisible by 8, we add a #
    //at the end to indicate the # of valid bit in the byte after.
    queue<bool> stream{};
    bitset<64> tmp{};
    vector<long> flatData{};
    flatten(flatData, data);
    unsigned int size = flatData.size();
    for (size_t i = 0; i < size;i++){
        tmp = bitset<64>(flatData[i]);
        for (int j = encodingLength[i] - 1; j >= 0;j--){
            stream.push(tmp[j]);
        }
    }
    vector<unsigned char> result{};
    bool out = false;
    while (true)
    {
        int counter = 0;
        tmp.reset();
        for (int i = 7; i >= 0;i--){
            if (stream.empty())
            {
                result.push_back(counter);
                COMP_CHAR_COUNT = result.size();
                if (counter != 0){
                    result.push_back(tmp.to_ulong());
                }
                out = true;
                break;
            }
            tmp[i] = stream.front();
            stream.pop();
            counter++;

        }
        if (out)
            break;
        result.push_back(tmp.to_ulong());
    }

    char *X_NULL_RESULT = new char[result.size()];
    memcpy(X_NULL_RESULT, reinterpret_cast<char *>(result.data()), result.size());
    dataPtr = X_NULL_RESULT;
    databyte = result.size();
    cout << "complete Data init" << endl;
}
void Ofile::writeAsRawFile(const vector<long> &data)
{
    ofstream outfile;
    outfile.open(FILE_NAME, ios::binary | ios::out);
    char *result = new char[data.size()];
    unsigned int i = 0;
    for (const auto &n : data)
    {
        if (n > 256)
            throw Error("out of range char in writing raw output file");
        result[i++] = n;
    }
    outfile.write(result, data.size());
    delete[] result;
    outfile.close();
}