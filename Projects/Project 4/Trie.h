#ifndef TRIE_INCLUDED
#define TRIE_INCLUDED

#include <string>
#include <vector>

using namespace std;

template<typename ValueType>
class Trie
{
public:
	Trie() {
		root = new Node();
	}
	~Trie() {
		clearNode(root);
	}
	void reset() {
		clearNode(root);
	}

	// O(key.length() * average # of children) runtime
	void insert(const std::string& key, const ValueType& value) {
		Node* cNode = root;
		for (char c : key) { // iterates through key
			int child;
			bool charFound = false;
			for (child = 0; child < cNode->children.size(); child++) { // iterates through children
				if (cNode->children[child]->path == c) {
					charFound = true;
					break;
				}
			}
			if (!charFound) { // adds path if not found
				cNode->children.push_back(new Node(c));
				child = cNode->children.size() - 1;
			}
			cNode = cNode->children[child]; // continues to next level of tree
		}
		cNode->vals.push_back(value); // pushes value to the node that matches the string
	}

	std::vector<ValueType> find(const std::string& key, bool exactMatchOnly) const {
		if (exactMatchOnly) {
			Node* n = findNode(key);
			if (n == nullptr)
				return vector<ValueType>();
			return n->vals;
		}
		else {
			vector<ValueType> result;
			if (key == "") return root->vals;
			for (Node* n : root->children) {
				if (n->path == key[0])
					findSNiP(key.substr(1), n, false, result);
			}
			return result;
		}
	}

	// C++11 syntax for preventing copying and assignment
	Trie(const Trie&) = delete;
	Trie& operator=(const Trie&) = delete;
private:
	struct Node {
		Node(char p, vector<ValueType> v) : vals(v), path(p) {};
		Node(char p) : path(p) { vals.clear(); };
		Node() : Node('~') {};

		char path;
		vector<ValueType> vals;

		vector<Node*> children;
	};

	//
	void findSNiP(const std::string& key, Node* cNode, bool failedYet, std::vector<ValueType>& result) const {
		if (key == "") {
			result.insert(
				result.end(), cNode->vals.begin(),
				cNode->vals.end());  // if at end of key, found it -> add to result
			return;
		}
		for (Node* child : cNode->children)  // otherwise note that failed once and continue
			if (child->path == key[0])
				findSNiP(key.substr(1), child, failedYet, result);
			else {
				if (failedYet) continue;
				findSNiP(key.substr(1), child, true, result);
			}
	}

	//runtime is O(L*C) where L is key.size(), C is average # of children
	Node* findNode(const std::string& key) const {
		Node* cNode = root;
		for (char c : key) {
			int child;
			bool charFound = false;
			for (child = 0; child < cNode->children.size(); child++) {
				if (cNode->children[child]->path == c) {
					charFound = true;
					break;
				}
			}
			if (!charFound) return nullptr;
			cNode = cNode->children[child];
		}
		return cNode;
	}

	void clearNode(Node* n) {
		if (n == nullptr)
			return;
		for (auto e : n->children) {
			clearNode(e);
		}
		delete n;
	}

	Node* root;
};

#endif // TRIE_INCLUDED
