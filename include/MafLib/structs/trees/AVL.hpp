#include "MafLib/main/GlobalHeader.hpp"

template <typename T> class AVLTree {
  private:
    struct AVLNode {
        T value;
        AVLNode *left, right;
        int32 height;

        int32 balance_factor() {
            int32 lsd = (this->left != nullptr) ? this->left->height : 0;
            int32 rsd = (this->right != nullptr) ? this->right.height : 0;
            return rsd - lsd;
        }
    };

    AVLNode *root;
};
