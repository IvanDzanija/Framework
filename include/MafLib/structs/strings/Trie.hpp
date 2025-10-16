#include "MafLib/main/GlobalHeader.hpp"

namespace maf {

template <typename T> class Trie {
  private:
    struct TrieNode {
        // Definition of sigma -> 256 ascii chars
        std::array<TrieNode *, 256> children;
        TrieNode *parent;
        uint8 child_count = 0;

        TrieNode(TrieNode *parent = nullptr) : parent(parent) {
            children.fill(nullptr);
        }

        ~TrieNode() {
            for (TrieNode *child : children) {
                delete child;
            }
        }

        /// Checks if this node is leaf.
        /// Uses current status of child_count variable.
        /// @return Boolean
        [[nodiscard]] bool is_leaf() { return child_count == 0; }

        /// Checks if there already is a transition for some $value.
        /// @return Pointer to the transition, nullptr if no transition exists.
        [[nodiscard]] TrieNode *transition(char value) {
            return children.at(static_cast<uint8>(value));
        }

        /// Inserts $value to children of this node.
        /// Adds this node as parent for inserted node.
        /// @return Pointer to new/old node with $value.
        TrieNode *insert(char value) {
            TrieNode *node = transition(value);
            if (node == nullptr) {
                auto *node = new TrieNode(this);
                children.at(static_cast<uint8>(value)) = node;
                ++child_count;
            }
            return node;
        }

        /// Removes child node with $value.
        /// @return Boolean based on the success of the removal.
        bool remove(char value) {
            TrieNode *node = transition(value);
            if (node != nullptr) {
                children.at(static_cast<unsigned char>(value)) = nullptr;
                --child_count;
                return true;
            }
            return false;
        }

        [[nodiscard]] std::string toString() const noexcept {
            std::string s = "Children(" + std::to_string(child_count) + "):\n";
            for (uint8 i = 0; i < 256; ++i) {
                if (children.at(i) != nullptr) {
                    s += ' ';
                    s += static_cast<char>(i);
                }
            }
            return s;
        }

        void print() const noexcept { std::cout << toString() << std::endl; }
    };

    TrieNode *_root = new TrieNode(nullptr);

    TrieNode *_search(std::string word) {
        TrieNode *current_node = _root;
        word += "\0";
        for (char c : word) {
            TrieNode *current_node = current_node->transition(c);
            if (current_node == nullptr) {
                return nullptr;
            }
        }
        if (current_node->is_leaf()) {
            return current_node;
        }
        return nullptr;
    }

  public:
    Trie() = default;
    Trie(TrieNode *root) : _root(root) {};
    ~Trie() { delete _root; }

    /// Inserts given word into existant trie.
    void insert(const std::string &word) {
        for (char c : word) {
            _root->insert(c);
        }
        _root->insert('\0');
    }

    [[nodiscard]] bool search(const std::string &word) {
        return _search(word) != nullptr;
    }

    bool remove(const std::string &word) {
        TrieNode *node = _search(word);

        if (node != nullptr) {
            std::string rev_word =
                "\0" + std::string(word.rbegin(), word.rend());

            TrieNode *current_node = node->parent;
            for (const char c : rev_word) {
                current_node->remove(c);
                if (current_node->child_count) {
                    break;
                }
                current_node = current_node->parent;
                if (current_node == nullptr) {
                    break;
                }
            }
            return true;
        }

        return false;
    }
};

} // namespace maf
