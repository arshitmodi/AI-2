#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <set>

using namespace std;

class queens{
public :
  int n;
  vector<int> q; // q[i] represents column of queen present in row i, -1 if the row is empty (0-indexed)
  vector<set<int>> domain;
  // for any i,j ;  |q[i]-q[j]| != |i-j|
  queens(int n){
    this->n = n;
    q.resize(n);
    fill(q.begin(),q.end(),-1);
    domain.resize(n);
    for(int i = 0; i<n ;i++){
      for(int j = 0; j<n ; j++){
        domain[i].insert(j);
      }
    }
  }

  vector <pair<int, int>> get_constraints (int row, int val){
    vector <pair<int, int>> constraints;
    set <pair<int, int>> temp_constraints;

    for(int i=0; i<n; ++i){
      if(i!=row)
        temp_constraints.insert( {i, val} );
    }

    for(int i = 0 ; i<n; ++i){
      if(i == row)
        continue;
      if( val - abs(row-i) >= 0 && val - abs(row-i) < n )
        temp_constraints.insert( {i, val - abs(row-i)} );
      if( val + abs(row-i) >= 0 && val + abs(row-i) < n )
        temp_constraints.insert( {i, val + abs(row-i)} );
    }

    for(pair <int, int> each: temp_constraints)
      constraints.push_back(each);

    return constraints;
  }

  bool backtrack()
  {
    int lowest = n+1;
    int row = -1;

    bool found = false;

    //choose most constrained cell
    for(int i=0; i<n; ++i){
      if(q[i] ==-1){
        found = true;
        if(domain[i].size() < lowest){
          lowest = domain[i].size();
          row = i;
        }
      }
    }

    if(!found)
      return true;

    vector <pair<int,int>> potential_value_list;
    //find least constraining value
    for(int potential_value = 0; potential_value<n; ++potential_value){
      if(domain[row].count(potential_value) == 0)
        continue;

      int LCV_cur_value = n;
      vector<pair<int,int>> temp = get_constraints(row,potential_value);

      vector<int> count(n,0);
      for(pair <int, int> & each: temp )
        if(domain[each.first].count(each.second) == 1)
          count[each.first]++;

      for(pair <int, int> & each: temp ){
        if(q[each.first] != -1)
          continue;

          LCV_cur_value = min(LCV_cur_value, (int)domain[each.first].size() - count[each.first]);
      }

      if(LCV_cur_value!=0){
        potential_value_list.push_back({potential_value, LCV_cur_value});
      }
    }

    sort(potential_value_list.begin(), potential_value_list.end(), [&](pair<int, int> & a, pair<int, int> & b){return a.second>b.second;});

    for(int i=0; i< (int) potential_value_list.size(); ++i){
      int value = potential_value_list[i].first;
      q[row] = value;
      vector<pair<int,int>> temp = get_constraints(row,value);
      //perform forward checking
      vector<pair<int,int>> modified;
      for(pair <int, int> & each: temp ){
        if(domain[each.first].count(each.second) == 1){
          domain[each.first].erase(each.second);
          modified.push_back({each.first,each.second});
        }
      }

      if(!backtrack()){
        //reverse the changes made
        q[row]= -1;
        for(pair<int,int> & each: modified)
          domain[each.first].insert(each.second);
      }
      else
      return true;
    }
    return false;
  }

  void print(){
    cout << " The row-column pairs are as follows" << endl ;
    for(int i = 0 ; i<n ; i++){
      cout << '{' << i+1 << ',' << q[i]+1 << '}' << "  ";
    }
  }
};

int main(){
  int n ;
  cout << " Enter the value of n" << endl;
  cin >> n;
  queens nq(n);
  if(nq.backtrack()){
    nq.print();
  }
  else
    cout << "can't generate the required arrangement";

  return 0;
}
