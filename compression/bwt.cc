#include "bwt.h"
#include <stack>
#include <utility>
#include <math.h>

// quick
bool lesseq(const vector<long>& a, const vector<long>& b){
    for (int i = 0; i <= a.size(); i++){
        if (a[i] > b[i])
            return false;
        if (a[i] < b[i])
            return true;
    }
    return true;
}
int partition(vector<vector<long>>& nums,int p,int left, int right){
    if (left >= right) return -1;
    int l = left-1;
    int r = right;
    swap(nums[p],nums[right]);
    while(true){
        do{
            l++;
        }while(l < right &&  lesseq(nums[l],nums[right]));
        do{
            r--;
        }while(r > l && lesseq(nums[right],nums[r]));
        if (r <= l){
            swap(nums[l],nums[right]);
            return l;
        }
        swap(nums[l],nums[r]);
    }

}
int getPivot(const vector<vector<long>>& nums,int l, int r){
    if ( (r-l+1) >= 3){
        int mid = l + floor((r-l)/2);
        if (lesseq(nums[l],nums[mid]) && lesseq(nums[mid],nums[r])){
            return mid;
        }else if ( lesseq(nums[mid],nums[l]) && lesseq(nums[l],nums[r])){
            return l;
        }else{
            return r;
        }
    }else{
        return l;
    }
}
void quickSort(vector<vector<long>>& nums){
    stack<pair<int,int>> coord{};
    coord.push(make_pair(0,nums.size()-1));
    while(!(coord.empty())){
        int l = coord.top().first;
        int r = coord.top().second;
        int tmp = partition(nums,getPivot(nums,l,r),l,r);
        coord.pop();
        if (tmp != -1){
            coord.push(make_pair(l,tmp-1));
            coord.push(make_pair(tmp+1,r));
        }
    }
}
//end quick


Bwt::Bwt(Transform *next) : Transform(next){};
void Bwt::transform(vector<unique_ptr<Block> > &input){
    for(const unique_ptr<Block>& line:input){
        applyTo(line->getData()); //transform the input
    }
    //         cout << "before target debuf" << endl;
    //     for (const auto &p : input)
    //     {
    //         for (const auto &c : p->getData())
    //         {
    //             cout << c << " ";
    //         }
    // }
}
void Bwt::decode(vector<unique_ptr<Block>>& input){
    //         cout << "before target debuf" << endl;
    //     for (const auto &p : input)
    //     {
    //         for (const auto &c : p->getData())
    //         {
    //             cout << c << " ";
    //         }
    // }
    for(const unique_ptr<Block>& line: input){
        deplyTo(line->getData());
    }
}
void Bwt::deplyTo(vector<long> & line){
    //decodes this line
    int size = line.size();
    vector<vector<long> > a{};
    //produce vector of begin,end pair wtih their line number
    for (int i = 0; i < size; i++)
    {
        vector<long> tmp{};
        tmp.push_back(line[i]);
        tmp.push_back(i);
        a.push_back(std::move(tmp));
    }
    quickSort(a);//sort them in lex
    long current = a[0].at(1);
    //produce the original
    for (int i = 0; i < size; i++)
    {
        line[i] = a[current].at(0);
        current = a[current].at(1);
    }
}

void Bwt::applyTo(vector<long>& line){ //generate suffixending for encoding
    int size = line.size();
    vector<vector<long> > sarr{};
    //produce suffix array
    for (int i = 0; i < size; i ++){
        vector<long> tmp{};
        int tmpi = i;
        int counter = 0;
        while (counter < size)
        {
            tmp.push_back(line[tmpi++]);
            if (tmpi >= size)
                tmpi = 0;
            counter++;
        }
        sarr.push_back(std::move(tmp));
    }
    quickSort(sarr);
    //encode
    for (int i = 0; i < size; i++){
        line[i] = sarr[i].at(size-1);
    }
}

