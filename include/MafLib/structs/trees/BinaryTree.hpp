#ifndef BINARY_TREE_H
#define BINARY_TREE_H
#pragma once

#include "MafLib/main/GlobalHeader.hpp"

namespace maf {
template <typename T>
class BinaryTree {
public:
private:
    struct Node {
        std::shared_ptr<T> _val;
        bool _owns = false;
        std::shared_ptr<Node> _left_child, _right_child;

        Node(T* val,
             bool owns,
             std::shared_ptr<Node> left = nullptr,
             std::shared_ptr<Node> right = nullptr)
            : _val(val), _owns(owns), _left_child(left), _right_child(right) {}

        // Creates a copy of a node, but object ownership stays with original
        // owner
        Node(const Node& other)
            : _val(other._val),
              _left_child(other._left_child),
              _right_child(other._right_child) {}

        // Copy assignment
        Node& operator=(const Node& other) noexcept {
            if (this != &other) {
                if (_owns && _val) {
                    delete _val;
                }

                _val = other._val;
                _owns = false;
                _left_child = other._left_child;
                _right_child = other._right_child;

                other._val = nullptr;
            }
            return *this;
        }

        // Move constructor
        Node(Node&& other) noexcept
            : _val(other._val),
              _owns(other._owns),
              _left_child(other._left_child),
              _right_child(other._right_child) {
            other._val == nullptr;
            other._owns = false;
        }

        // Move assignment
        Node& operator=(Node&& other) noexcept {
            if (this != &other) {
                if (_owns && _val) {
                    delete _val;
                }

                _val = other._val;
                _owns = other._owns;
                _left_child = std::move(other._left_child);
                _right_child = std::move(other._right_child);

                other._val = nullptr;
                other._owns = false;
            }
            return *this;
        }

        ~Node() {
            if (_owns && _val) {
                delete _val;
            }
        }

        // Transfer ownership to another node
        void transfer_ownership_to(Node& other) noexcept {
            if (_owns && other._val == _val) {
                other._owns = true;
                _owns = false;
            }
        }

        // Release ownership (caller responsible for deletion)
        T* release_ownership() {
            _owns = false;
            return _val;
        }

        // Take ownership
        void take_ownership() {
            _owns = true;
        }

        T& value() {
            return *_val;
        }
        const T& value() const {
            return *_val;
        }
    };

    std::shared_ptr<Node> _root;
};

}  // namespace maf
#endif
