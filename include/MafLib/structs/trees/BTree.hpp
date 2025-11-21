#include <MafLib/main/GlobalHeader.hpp>

// TODO: revisit this. Add it to namespace
template <typename T>
class BTree {
private:
    struct BTValue;
    struct BTNode;

    struct BTValue : std::enable_shared_from_this<BTValue> {
        T _value;
        std::shared_ptr<BTValue> _prev = nullptr;
        std::shared_ptr<BTValue> _next = nullptr;
        std::shared_ptr<BTNode> _left_child = nullptr;
        std::shared_ptr<BTNode> _right_child = nullptr;

        explicit BTValue(const T& value) : _value(value) {}

        void set_next(const std::shared_ptr<BTValue>& next) {
            _next = next;
            if (next) {
                next->_prev = this->shared_from_this();
            }
        }

        void set_prev(const std::shared_ptr<BTValue>& prev) {
            _prev = prev;
            if (prev) {
                prev->_next = this->shared_from_this();
            }
        }

        void reset_left_child() {
            _left_child = nullptr;
        }
        void reset_right_child() {
            _right_child = nullptr;
        }
    };

    struct BTNode : std::enable_shared_from_this<BTNode> {
        std::shared_ptr<BTValue> _first = nullptr;
        std::shared_ptr<BTValue> _last = nullptr;
        size_t _size = 0;
        uint8 _deg = 5;
        std::shared_ptr<BTNode> _parent_node = nullptr;
        std::shared_ptr<BTValue> _parent_value = nullptr;
        std::shared_ptr<BTNode> orphan = nullptr;

        BTNode() = default;
        explicit BTNode(const T& val, uint8 deg = 5, size_t size = 1)
            : _deg(deg), _size(size) {
            _first = new BTValue(val);
            _last = _first;
        }

        ~BTNode() {
            std::shared_ptr<BTValue> val = _first;
            while (val) {
                std::shared_ptr<BTValue> next = val->next;
                delete val;
                val = next;
            }
        }

        std::pair<std::shared_ptr<BTNode>, std::shared_ptr<BTValue>> search(
            const T& value) {
            if (!_first) {
                return std::make_pair(this->shared_from_this(), nullptr);
            }

            std::shared_ptr<BTValue> val = _first;
            while (val->_value < value && val->_next) {
                val = val->_next;
            }

            if (val->_value == value) {
                return std::make_pair(this->shared_from_this(), val);
            }
            if (val->_next && val->_value < value) {
                if (val->_right_child != nullptr) {
                    return val->_right_child->search(value);
                }
                return std::make_pair(this->shared_from_this(), nullptr);
            }
            if (val->_left_child) {
                return val->_left_child->search(value);
            }
            return std::make_pair(this->shared_from_this(), nullptr);
        }

        std::shared_ptr<BTValue> insert(const T& value) {
            std::shared_ptr<BTValue> val = _first;
            while (val && val->_value <= value) {
                val = val->_next;
            }
            auto new_val = std::make_shared<BTValue>(value);

            if (val) {
                std::shared_ptr<BTValue> prev = val->_prev;
                new_val->set_next(val);
                if (prev) {
                    prev->set_next(val);
                } else {
                    _first = new_val;
                }
            } else {
                if (_last) {
                    _last->set_next(new_val);
                } else {
                    _first = new_val;
                }
            }
            if (!new_val->_next) {
                _last = new_val;
            }
            ++_size;
            return new_val;
        }
    };

    std::shared_ptr<BTNode> _root;
    uint8 _deg;

public:
    BTree(const T& val, int deg = 5) : _deg(deg) {
        _root = std::make_shared<BTNode>(val, _deg);
    }

    std::shared_ptr<BTNode> get_root() {
        return _root;
    }

    std::shared_ptr<BTValue> search(const T& val) {
        return _root->search(val).second;
    }

    void insert(const T& val) {
        auto [node, value] = _root->search(val);
        node->insert(val);
        auto nroot = node->split();
        if (nroot) {
            _root = nroot;
        }
    }
};
