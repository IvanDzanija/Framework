#include "MafLib/main/GlobalHeader.hpp"

template <typename T> class AVLTree {
  private:
    struct AVLNode {
        T value;
        AVLNode *left, right;
        int32 height;

        int32 balance_factor() {
            int32 lsd = 0;
            int32 rsd = 0;
            if (this->left != nullptr) {
                lsd = this->left->height;
            }
            if (this->right != nullptr) {
                rsd = this->right.height;
            }
            return rsd - lsd;
        }
    };

    AVLNode *root;
};
