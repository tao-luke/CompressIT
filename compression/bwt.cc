#include "bwt.h"
#include <stack>
#include <utility>
#include <math.h>

//Burrows Wheeler transformation:
// given some text input S, suppose some patterns of text appears, BWT ensures a reversible text
// that has lots of high runlength texts. ex: aaabsbdbbbbbb has a long runs of a and b

//Implementation:
// generate suffix array , O(n)
// worst case: O(nlog^2n)
//expected O(n)

//using quick sort
struct suffix //mimick suffix entry
{
    size_t index; // To store original index
    long rank[2]; // To store ranks and next rank pair
    suffix(){
    }
};
 
// compare 2 rankings, return true if a<b
bool lessSuf( suffix a, suffix b)
{
    return (a.rank[0] == b.rank[0])? (a.rank[1] < b.rank[1] ?1: 0):
               (a.rank[0] < b.rank[0] ?1: 0);
}
long partition(vector<suffix>& nums,long p,long left,long right){
    if (left >= right) return -1;
    long l = left-1;
    long r = right;
    swap(nums[p],nums[right]);
    while(true){
        do{
            l++;
        }while(l < right &&  lessSuf(nums[l],nums[right]));
        do{
            r--;
        }while(r > l && lessSuf(nums[right],nums[r]));
        if (r <= l){
            swap(nums[l],nums[right]);
            return l;
        }
        swap(nums[l],nums[r]);
    }

}
long getPivot(const vector<suffix>& nums, long l, long r){
    if ( (r-l+1) >= 3){
        long mid = l + floor((r-l)/2);
        if (lessSuf(nums[l],nums[mid]) && lessSuf(nums[mid],nums[r])){
            return mid;
        }else if ( lessSuf(nums[mid],nums[l]) && lessSuf(nums[l],nums[r])){
            return l;
        }else{
            return r;
        }
    }else{
        return l;
    }
}
void quickSort(vector<suffix>& nums){
    stack<pair<long,long>> coord{};
    coord.push(make_pair(0,nums.size()-1));
    while(!(coord.empty())){
        long l = coord.top().first;
        long r = coord.top().second;
        long tmp = partition(nums,getPivot(nums,l,r),l,r);
        coord.pop();
        if (tmp != -1){
            coord.push(make_pair(l,tmp-1));
            coord.push(make_pair(tmp+1,r));
        }
    }
}
 
//construct the suffix array nlog using txt
long *buildSuffixArray(vector<long>& txt)
{
    vector<suffix> suffixes{};
    size_t n = txt.size();

    //init
    for (unsigned int i = 0; i < n; i++)
    {
        suffixes.push_back(suffix{});
        suffixes[i].index = i;
        suffixes[i].rank[0] = txt[i];
        suffixes[i].rank[1] = ((i+1) < n)? (txt[i + 1]): -1;
    }
 
    ///sort by easy 2 first
    quickSort(suffixes);
 
    //sort by 4 and 8 and so on
    long ind[n];
    for (unsigned int k = 4; k < 2*n; k = k*2)
    {
        
        // shift rank prior and get next rank prior
        int rankP = 0;
        int rank_bef = suffixes[0].rank[0];
        suffixes[0].rank[0] = rankP;
        ind[suffixes[0].index] = 0;

        for (unsigned int i = 1; i < n; i++)
        {

            if (suffixes[i].rank[0] == rank_bef &&
                    suffixes[i].rank[1] == suffixes[i-1].rank[1])
            {
                rank_bef = suffixes[i].rank[0];
                suffixes[i].rank[0] = rankP;
            }
            else 
            {
                rank_bef = suffixes[i].rank[0];
                suffixes[i].rank[0] = ++rankP;
            }
            ind[suffixes[i].index] = i;
        }
 
        //echo thourhg hte rank to next
        for (unsigned int i = 0; i < n; i++)
        {
            unsigned int nextindex = suffixes[i].index + k/2;
            suffixes[i].rank[1] = (nextindex < n)?
                                  suffixes[ind[nextindex]].rank[0]: -1;
        }
 
        //sort again
        quickSort(suffixes);
    }
 
    // save result
    long *suffixArr = new long[n];
    for (unsigned int i = 0; i < n; i++){
        long ans = suffixes[i].index;
        if (ans == 0)
            suffixArr[i] =0;
        else suffixArr[i] = txt[ans-1];
    }
        // Return the suffix array
        return suffixArr;
}
//end quick


Bwt::Bwt(Transform *next) : Transform(next){};
void Bwt::transform(vector<unique_ptr<Block> > &input){
    for(const unique_ptr<Block>& line:input){
        applyTo(line->getData()); //transform the input
    }
}
void Bwt::decode(vector<unique_ptr<Block>>& input){
    for(const unique_ptr<Block>& line: input){
        deplyTo(line->getData());
    }
}
void Bwt::deplyTo(vector<long> & line){
    //decodes this line
    int size = line.size();
    vector<suffix> a{};
    //produce vector of begin,end pair wtih their line number
    for (int i = 0; i < size; i++)
    {
        suffix tmp{};
        tmp.index = i;
        tmp.rank[0] = line[i];
        tmp.rank[1] = i;
        a.push_back(std::move(tmp));
    }
    quickSort(a);//sort them in lex
    long current = a[0].rank[1];
    //produce the original
    for (int i = 0; i < size; i++)
    {
        line[i] = a[current].rank[0];
        current = a[current].rank[1];
    }
}

void Bwt::applyTo(vector<long>& line){ //generate suffixending for encoding
    long *result = buildSuffixArray(line);
    line.assign(result, result + line.size());
    delete[] result;
}
