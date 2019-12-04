#include <cctype>
#include <ctype.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <unordered_map>

using namespace std;

//树节点
class TreeNode{
public:
    int weight;
    char name;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int w) : weight(w), left(NULL), right(NULL){
        name = NULL;
    }
};

//优先队列的比较原则
struct cmp{
    bool operator ()(TreeNode* &a, TreeNode* &b) const{
        return a->weight >= b->weight; //权值越小越优先，去掉等号会改变编码，但结果仍为最小
    }
};

//从文本中获取数据并返回优先队列
priority_queue<TreeNode*,vector<TreeNode*>,cmp> fileread(string filename, unordered_map<char,int> &counts){
    fstream f;
    f.open(filename);
    if(!f.is_open()){
        cout << "error : file not exist!" << endl;
        return {};
    }
    char c;
    while(!f.eof() && f.peek() != EOF){
        c = f.get();
        if(c == '\n' || c == '\t')
            continue;
        if(c >= 'A' && c <= 'Z')
            c = tolower(c);
        if(c != '#' && isalpha(c)==false)
            c = '#';
        counts[c]++;
    }
    //构建优先队列
    //C STL中的优先队列默认是大根堆，因此需要自定义比较方法cmp
    priority_queue<TreeNode*,vector<TreeNode*>,cmp> q;
    for(auto &x : counts){
        TreeNode *Node = new TreeNode(x.second);
        Node->name = x.first;
        q.push(Node);
    }
    f.close();
    return q;
}

//实现哈夫曼树
TreeNode* Select(priority_queue<TreeNode*,vector<TreeNode*>,cmp> &q){
    if(q.size() == 1)
        return q.top();
    //选取权值最小的两个结点
    TreeNode *a = q.top();
    q.pop();
    TreeNode *b = q.top();
    q.pop();
    //新建一个结点，权值为选择的两个结点之和，左子为权值较小的结点a，右子为权值次小的结点b
    TreeNode *newNode = new TreeNode(a->weight + b->weight);
    newNode->left = a;
    newNode->right = b;
    //把新建的结点放回队列
    q.push(newNode);
    return Select(q);
}

//深度优先搜索，实现编码
void DFS(TreeNode *t, unordered_map<char, string> &encode, string temp){
    if(t->left == NULL && t->right == NULL){
        encode[t->name] = temp;
        return;
    }
    DFS(t->left, encode, temp + '0');
    DFS(t->right, encode, temp + '1');
}

int main(){
    unordered_map<char,int> counts; //记录文本中每个字母的出现频率
    string filename = "huffman.txt";
    priority_queue<TreeNode*,vector<TreeNode*>,cmp> q = fileread(filename, counts);
    TreeNode *t = Select(q);
    unordered_map<char,string> encode;
    DFS(t, encode, "");
    int sum = 0;
    for(auto &x : encode){
        cout << x.first << "\t frequency:" << counts[x.first] << "\t encode:" << x.second << endl;
        sum += counts[x.first] * x.second.size();
    }
    cout << "use huffman encode:\t" << sum << endl;
    cout << "not use huffman:\t" << 5 * 2098 << endl;
    return 0;
}
