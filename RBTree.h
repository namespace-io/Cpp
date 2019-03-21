#include <iostream>
#include <string>
template <typename K, typename V>
class RBTree{
    private:
        const static bool RED = true;
        const static bool BLACK = false;
        typedef struct Node
        {
            Node* left;
            Node* right;
            bool color; // true, false -> red, black, parnet to this' s color
            K key;
            V val;
            Node(const K& k, const V& v, bool c):key(k), val(v), color(c){}
        } Node;

        Node* root;
        Node* fixUp(Node *h){
            if(isRed(h->right)) h = rotateLeft(h);
            if(isRed(h->left) && isRed(h->left->left)) h = rotateRight(h);
            if(isRed(h->left) && isRed(h->right)) ColorFlip(h);

            return h;
        }
        Node* rotateLeft(Node* h){
            Node* x = h->right;
            h->right = x->left;
            x->left = h;
            x->color = h->color;
            h->color = RED;
            return x;
        }

        Node* rotateRight(Node* h){
            Node* x = h->left;
            h->left = x->right;
            x->right = h;
            x->color = h->color;
            h->color = RED;
            return x;
        }
        bool isRed(const Node* h){return h ? h->color == RED : BLACK; }
        void ColorFlip(Node* h){
            h->left->color = !h->left->color;
            h->right->color = !h->right->color;
            h->color = !h->color;
        }
        
        Node* insert(Node* h, const K k, const V v){
            if(h == nullptr) return new Node(k, v, RED);
            if(isRed(h->left) && isRed(h->right))
                ColorFlip(h);

            if(k == h->key) h->val = v;
            else if(k > h->key) h->right = insert(h->right, k, v);
            else h->left = insert(h->left, k, v);

            if(isRed(h->right))
                h = rotateLeft(h);  
            if(isRed(h->left) && isRed(h->left->left))
                h = rotateRight(h);
            
            return h;
        }
        Node* moveRedLeft(Node* h){
            ColorFlip(h);
            if(isRed(h->right->left)){ // 对应的是2-node旁边是3-node或者4-node
                h->right = rotateRight(h->right);
                h = rotateLeft(h);
                ColorFlip(h);
            }
            return h;
        }
        Node* deleteMin(Node* h){
            if(h->left == nullptr) {
                delete h;
                return nullptr;
            }

            if(!isRed(h->left) && !isRed(h->left->left))
                h = moveRedLeft(h);
            
            h->left = deleteMin(h->left);
            return fixUp(h);
        }


        Node* moveRedRight(Node* h){
            ColorFlip(h);
            if(h->left->right){
                h->left = rotateLeft(h->left);
                h = rotateRight(h);
                ColorFlip(h);
            }

            return h;
        }

        Node* deleteMax(Node* h){
            if(isRed(h->left)) h = rotateRight(h);
            if(h->right == nullptr) {
                delete h;
                return nullptr;
            }
            if(!isRed(h->right) && !isRed(h->right->left))
                h = moveRedRight(h);
            h->right = deleteMax(h->right);
            return fixUp(h);
        }
        Node* min(Node*h ){ return h?min(h->left):nullptr;}
        Node* erase(Node* h, const K& k){
            if(k < h->key){
                if(!isRed(h->left) && !isRed(h->left->left))
                    h = moveRedLeft(h);
                h->left = erase(h->left, k);
            } else {
                if(isRed(h->left)) h = rotateRight(h);
                if(h->key == k && h->right == nullptr){
                    delete h;
                    return nullptr;
                }

                if(!isRed(h->right) && !isRed(h->right->left))
                    h = moveRedRight(h);
                if(h->key == k){
                    Node* minNode = min(h->right);
                    h->key = minNode->key;
                    h->val = minNode->val;
                    h->right = deleteMin(h->right);
                } else h->right = erase(h->right, k);
                    
            }

            return fixUp(h);
        } 

        Node* find(Node* h, const K& k){
            if(h == nullptr) return nullptr;
            if(k == h->key) return h;
            else if(k > h->key) return find(h->right, k);
            else return find(h->left, k);
        }
        void destory(Node* h){
            if(h){
                destory(h->left);
                destory(h->right);
                delete h;
                h = nullptr;
            }
        }

        void prettyPrintTree(Node* node, std::string prefix = "", bool isLeft = true) {
            if (node == nullptr) {
                std::cout << "Empty tree\n";
                return;
            }

            if(node->right) {
                prettyPrintTree(node->right, prefix + (isLeft ? "│   " : "    "), false);
            }

            std::cout << prefix + (isLeft ? "└── " : "┌── ") + std::to_string(node->val) + "\n";

            if (node->left) {
                prettyPrintTree(node->left, prefix + (isLeft ? "    " : "│   "), true);
            }
        }

    public:
        void deleteMin(){
            root = deleteMin(root);
            root->color = BLACK;
        }

        void insert(const K& k, const V &v){
            root = insert(root, k, v);
            root->color = BLACK;
        }

        void erase(const K& k) {
            if(empty()) return;
            if(!isRed(root->left) && !isRed(root->right)) root->color = RED;
            root = erase(root, k);
            if(!empty()) root->color = BLACK;
        }

        bool empty(){return root == nullptr;}
        void deleteMax(){
            root = deleteMax(root);
            root->color = BLACK;
        }
        RBTree():root(nullptr){}
        ~RBTree(){
            destory(root);
        }
      

        void print(){
		    prettyPrintTree(root);
	    }
};