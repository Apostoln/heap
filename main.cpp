#include <iostream>
#include <vector>
#include <memory>
#include <cassert>

template <class T>
struct Node {
    T value;
    Node<T>* parent = nullptr;
    Node<T>* left = nullptr;
    Node<T>* right = nullptr;
};

template <class T>
class Heap {
    Node<T>* root = nullptr;

public:
    Heap() = default;
    Heap(const Heap&) = default;
    Heap(Heap&&) = default;
    ~Heap() = default;

    void add(const T& value) {
        //pick new node to the end
        if(nullptr == root) {
            root = new Node<T>{value, nullptr, nullptr, nullptr};
            return;
        }

        std::vector<Node<T>*> nodes = {root};
        Node<T>* node = nullptr;
        while(nullptr == node ) {
            for(auto& n : nodes) {
                Node<T>** parent = (nullptr == n->left) ? &(n->left)
                                                        : (nullptr == n->right) ? &(n->right)
                                                                                : nullptr;
                if(nullptr != parent) {
                    *parent = node = new Node<T>{value, n, nullptr, nullptr};
                    break;
                }
            }
            nodes = getNextLayer(nodes);
        }

        //swap with parent until child > parent
        while (nullptr != node->parent && node->value > node->parent->value) {
            std::swap(node->value, node->parent->value);
            node = node->parent;
        }
    }

    T* getMax() {
        return root == nullptr? nullptr : &(root->value);
    }

    T* extractMax() {
        if (nullptr == root) {
            return nullptr;
        }

        T* result = new T(root->value);

        //swap node value with biggerChild until both children are nullptr, then delete node
        Node<T>* node = root;
        while(nullptr != node->left ||
              nullptr != node->right) {

            Node<T> *biggerChild = nullptr;
            if(nullptr == node->left) {
                biggerChild = node->right;
            }
            else if(nullptr == node->right) {
                biggerChild = node->left;
            }
            else {
                biggerChild = node->left->value >= node->right->value ?
                                                           node->left :
                                                           node->right;
            }

            std::swap(biggerChild->value, node->value);
            node = biggerChild;
        }

        if (node->parent == nullptr) {
            delete(node);
            root = nullptr;
        }
        else if (node->parent->left == node) {
            node->parent->left = nullptr;
            delete(node);
        }
        else if (node->parent->right == node) {
            node->parent->right = nullptr;
            delete(node);
        }
        else {
            assert(false);
        }

        return result;
    }

    void print() {
        std::vector<Node<T>*> nodes = {root};
        while(!nodes.empty()) {
            for(const auto& node : nodes) {
                std::cout << node->value << " ";
            }
            std::cout << std::endl;
            nodes = getNextLayer(nodes);
        }
    }

    std::vector<Node<T>*> getNextLayer(std::vector<Node<T>*> nodes) {
        std::vector<Node<T>*> result;
        result.reserve(nodes.size()*2);
        for(const auto& node : nodes) {
            if(nullptr != node->left) {
                result.push_back(node->left);
            }
            if(nullptr != node->right) {
                result.push_back(node->right);
            }
        }
        return result;
    }

};

template <class T>
std::ostream& operator << (std::ostream& out, const Node<T>& node) {
    out << &node << " "
        << node.value << " "
        << node.parent << " "
        << node.left << " "
        << node.right << std::endl;
    return out;
}

bool test1() {

    Heap<int> h;
    {
        auto max = std::unique_ptr<int>(h.extractMax());
        assert(nullptr == max);
    }

    h.add(200);
    h.add(10);
    {
        auto max = std::unique_ptr<int>(h.extractMax());
        assert(nullptr != max);
        assert(200 == *max);
    }
    h.add(5);
    h.add(500);
    {
        auto max = std::unique_ptr<int>(h.extractMax());
        assert(nullptr != max);
        assert(500 == *max);
    }

    return true;
}

bool test2() {
    Heap<int> h;
    h.add(200);
    h.add(10);
    h.add(5);
    h.add(500);
    {
        auto max = std::unique_ptr<int>(h.extractMax());
        assert(nullptr != max);
        assert(200 == *max);
    }
    {
        auto max = std::unique_ptr<int>(h.extractMax());
        assert(nullptr != max);
        assert(10 == *max);
    }
    {
        auto max = std::unique_ptr<int>(h.extractMax());
        assert(nullptr != max);
        assert(5 == *max);
    }
    {
        auto max = std::unique_ptr<int>(h.extractMax());
        assert(nullptr != max);
        assert(500 == *max);
    }
}


int main() {
    Heap<int> h;

    int n;
    std::cin >> n;
    for(int i = 0; i < n; i++) {
        std::string cmd;
        std::cin >> cmd;
        if ( auto pos = cmd.find("Insert"); pos != std::string::npos) {
            int num;
            std::cin >> num;

            h.add(num);

        }
        else if ( cmd.find("ExtractMax") != std::string::npos) {
            auto max = h.extractMax();
            std::cout << (max != nullptr ? *max : 0) << std::endl;

        }
        else {
            assert(false);
        }
    }
    return 0;
}