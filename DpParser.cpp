//
// Created by wyl on 2022/8/5.
//
#include "DpParser.h"

DpParser::DpParser() {
    maxn = 1500;
    f = new int*[maxn];
    opt = new char*[maxn];
    pre = new pair<int,int>* [maxn];
    for(int i=0;i<maxn;i++){
        f[i]=new int[maxn];
        opt[i] = new char[maxn];
        pre[i] = new pair<int,int>[maxn];
    }
}

DpParser::DpParser(int n) {
    if(n>=2000){
        cerr<<"this is not advice to set tolerant length larger than 2000 "<<endl;
    }
    maxn = 1.5*n;
    f = new int*[maxn];
    opt = new char*[maxn];
    pre = new pair<int,int>* [maxn];
    for(int i=0;i<maxn;i++){
        f[i]=new int[maxn];
        opt[i] = new char[maxn];
        pre[i] = new pair<int,int>[maxn];
    }
}
string DpParser::dp_get_cigar_str(string s1,string s2,int& size,int& delete_size,int &mismatch_size,int &insert_size){
    stringstream ss;
    if(s1.size()==0&&s2.size()==0)return "";
    if(s1.size()==0){
        ss<<s2.size()<<"I";
        insert_size+=s2.size();
        return ss.str();
    }
    if(s2.size()==0){
        ss<<s1.size()<<"D";
        delete_size+=s1.size();
        return ss.str();
    }
    f[0][0] = 0;
    int len1 = s1.size();
    int len2 = s2.size();
    for(int i=1;i<=len1;i++){f[i][0]=i;pre[i][0]={i-1,0};opt[i][0]='D';}
    for(int i=1;i<=len2;i++){f[0][i]=i;pre[0][i]={0,i-1};opt[0][i]='I';}
    for(int i=1;i<=len1;i++){
        for(int j=1;j<=len2;j++){
            if(s1[i-1]==s2[j-1]){
                f[i][j]=f[i-1][j-1];//match
                pre[i][j]={i-1,j-1};
                opt[i][j]='M';
            }else{
                int a = f[i-1][j-1]+1;//update
                int b = f[i-1][j]+1;//delete
                int c = f[i][j-1]+1;//insert
                if(a<=b&&a<=c){
                    pre[i][j]={i-1,j-1};
                    opt[i][j]='X';
                    f[i][j]=a;
                }else if(b<a&&b<=c){
                    pre[i][j]={i-1,j};
                    opt[i][j]='D';
                    f[i][j]=b;
                }else{
                    pre[i][j]={i,j-1};
                    opt[i][j]='I';
                    f[i][j]=c;
                }
            }
        }
    }
    stack<char> s;
    pair<int,int> t={len1,len2};
    while(t.first!=0||t.second!=0){
        s.push(opt[t.first][t.second]);
        t = pre[t.first][t.second];
    }
    string str;
    while(s.size()){
        if(s.top()=='M')size++;
        if(s.top()=='D')delete_size++;
        if(s.top()=='I')insert_size++;
        if(s.top()=='X')mismatch_size++;
        str.push_back(s.top());
        s.pop();
    }
    if(str.size()==1){
        ss<<1<<str[0];
        return ss.str();
    }
    char temp = str[0];
    int cnt = 1;
    for(int i=0,j=1;j<str.size();i++,j++){
        if(str[i]==str[j]){
            cnt++;
        }else{
            ss<<cnt<<str[i];
            temp = str[j];
            cnt = 1;
        }
    }
    ss<<cnt<<str[str.size()-1];
    return ss.str();
}


string DpParser::get_cigar_str(AlignmentGraph& graph,path_res& res,string s){
    vector<merge_res>& v = res.path;
    if(v.size()==0)return "";
    stringstream ss;
    ss<<v[0].match_size<<"=";
    string s1,s2;
    for(int i=0,j=1;j<v.size();i++,j++){
        pair<int,int> range={v[i].string_range.second+1,v[j].string_range.first-1};
        int len1 = v[i].v.size();
        int len2 = v[j].v.size();
        node_res pre = v[i].v[len1-1];
        node_res next = v[j].v[0];

        if(pre.node_id==next.node_id){
            if(range.second-range.first+1<0){//如果insert情况正好的字母正好可以向左拓展
                res.mergeRes.delete_size+=(next.node_range.first-pre.node_range.second-1)-(range.second-range.first+1);
                ss<<(next.node_range.first-pre.node_range.second-1)-(range.second-range.first+1)<<'D';
                ss<<v[j].match_size<<"=";
                res.mergeRes.match_size+=range.second-range.first+1;
                continue;
            }
            s1=graph.node[pre.node_id].substr(pre.node_range.second+1,next.node_range.first-pre.node_range.second-1);
        }else{
            s1=graph.node[pre.node_id].substr(pre.node_range.second+1);
            s1+=graph.node[next.node_id].substr(0,next.node_range.first+1);
        }
        s2=s.substr(range.first,range.second-range.first+1);
        ss<<dp_get_cigar_str(s1,s2,res.mergeRes.match_size,res.mergeRes.delete_size,res.mergeRes.mismatch_size,res.mergeRes.insert_size);
        ss<<v[j].match_size<<"=";
    }
    return ss.str();
}

///**
// * this is test for DpParser
// **/
//int main(){
//   DpParser dpParser;
//   int size;
//   cout<<dpParser.dp_get_cigar_str("wangyulong","wanglong",size)<<endl;
//}
