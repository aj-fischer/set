#include "set.h"
#include <iostream>


Set::Set() {
    _size = 0;
    _head, _tail = new Elem();
    _head, _tail = nullptr;
    _head->next = _tail;
    _tail->prev = _head;
}

Set::Set(const Set& rhs) {
    copyCode(rhs);
}

Set::~Set() {
    destructCode();
}

Set& Set::operator=(const Set &rhs) {
    if (this == &rhs) {
        return *this;
    }
    destructCode();
    _size = rhs._size;
    if (_size > 0) {
        copyCode(rhs);
    }
    return *this;
}

void Set::insert(const ELEMENT_TYPE info) {
    Elem *newNode = new Elem;
    newNode->info = info;
    Elem *curr = _head;

    if (_head->next == _tail) {
        _head->next = newNode;
        newNode->prev = _head;
        newNode->next = _tail;
        _tail->prev = newNode;
        _size++;
    } else {
        while (newNode->info > curr->next->info && curr->next != _tail) {   // Prevents infinite loop
            curr = curr->next;
        }
        if (newNode->info == curr->next->info) {
            delete newNode;
            return;
        } else {
            newNode->next = curr->next;
            newNode->prev = curr;
            curr->next->prev = newNode;
            curr->next = newNode;
            _size++;
        }
    }
}

void Set::erase(ELEMENT_TYPE info) {
    if (_size == 0) {
        return;
    }

    Elem *curr = _head->next;
    Elem *temp = _head->next;

    while (curr != _tail) {
        if (curr->info == info) {
            temp->next->prev = temp->prev;
            temp->prev->next = temp->next;
            delete curr;
            _size--;
            return;
        }
        curr = curr->next;
        temp = temp->next;
    }
}

void Set::clear() {
    if (size() == 0) {
        return;
    }

    Elem* curr = _head->next;
    Elem* temp = _head->next;

    while (temp != _tail) {
        temp = temp->next;
        delete curr;
        curr = temp;
        _size--;
    }

    _head->next = _tail;
    _tail->prev = _head;
}

int Set::size() const {
    return _size;
}

Set::Iterator Set::find(ELEMENT_TYPE info) const {
    Set::Iterator it = Iterator(_head->next);

    while (it != Set::end()) {
        if (*it == info) {
            return it;
        }
        it++;
    }
    return Set::end();
}


Set::Iterator Set::begin() const {
    return Iterator(_head->next);
}


Set::Iterator Set::end() const {
    return Iterator(_tail);
}


Set::Reverse_Iterator Set::rbegin() const {
    return Reverse_Iterator(_tail->prev);
}


Set::Reverse_Iterator Set::rend() const {
    return Reverse_Iterator(_head);
}


ELEMENT_TYPE& Set::Iterator::operator*() {
    return _cur->info;
}


Set::Iterator Set::Iterator::operator++(int) {
    if (_cur->next != nullptr) {
        _cur = _cur->next;
        return Iterator(_cur);
    } else {
        return;
    }
}


Set::Iterator Set::Iterator::operator--(int) {
    if (_cur->prev != nullptr) {
        _cur = _cur->prev;
        return Iterator(_cur);
    } else {
        return;
    }
    
}


bool Set::Iterator::operator==(Iterator it) const {
    if (_cur == it._cur) {
        return true;
    } else {
        return false;
    }
}


bool Set::Iterator::operator!=(Iterator it) const {
    if (_cur != it._cur) {
        return true;
    } else {
        return false;
    }
}


ELEMENT_TYPE& Set::Reverse_Iterator::operator*() {
    return _cur->info;
}


Set::Reverse_Iterator Set::Reverse_Iterator::operator++(int) {
    if (_cur->prev != nullptr) {
        _cur = _cur->prev;
        return Reverse_Iterator(_cur);
    } else {
        return;
    }
}


Set::Reverse_Iterator Set::Reverse_Iterator::operator--(int) {
    if (_cur->next != nullptr) {
        _cur = _cur->next;
        return Reverse_Iterator(_cur);
    } else {
        return;
    }
}


bool Set::Reverse_Iterator::operator==(Reverse_Iterator it) const {
    if (_cur == it._cur) {
        return true;
    } else {
        return false;
    }
}


bool Set::Reverse_Iterator::operator!=(Reverse_Iterator it) const {
    if (_cur != it._cur) {
        return true;
    } else {
        return false;
    }
}


void Set::copyCode(const Set &v) {
    // If empty, we're done
    if (v._head->next == v._tail) {
        return;
    }

    _head = new Elem();
    _tail = new Elem();
    _head->next = _tail;
    _tail->prev = _head;
    _size = v._size;

    Elem *curr = v._head->next;
    Elem *copyCurr = _head;
    Elem *copyPrev = _head;

    // Make deep copy of elements from original set
    while (curr != v._tail) {
        copyCurr = copyCurr->next;
        Elem *newElem = new Elem();
        newElem->info = curr->info;
        copyCurr->prev = newElem;
        copyCurr->prev->next = copyCurr;
        copyPrev->next = copyCurr->prev;
        copyPrev->next->prev = copyPrev;
        copyPrev = copyPrev->next;
        copyCurr = copyCurr->prev;
        curr = curr->next;
    }
}

void Set::destructCode() {
    Elem *curr = _head;
    Elem *temp = _head;
    
    while (temp != _tail) {
        temp = temp->next;
        delete curr;
        curr = temp;
    }
    delete _tail;
}


ostream& Set::dump(ostream& out, const Set &v){
    if (v.size() == 0) {
        out << endl;
    } else {
        Set::Iterator curr = Iterator(v.begin());
        while (curr != v.end()) {
            out << *curr << " ";
            curr++;
            }
        out << endl;
        }
    return out;
}


Set operator&(const Set& lhs, const Set& rhs) {
    Set s = Set();
    Set::Iterator left_it = lhs.begin();
    Set::Iterator right_it = rhs.begin();

    while (right_it != rhs.end()) {
        while (left_it != lhs.end()) {
            if (*right_it == *left_it) {
                s.insert(*right_it);
            }
            left_it++;
        }
        right_it++;
    }
    return s;
}


Set operator|(const Set& lhs, const Set& rhs) {
    Set s = Set();
    Set::Iterator left_it = lhs.begin();
    Set::Iterator right_it = rhs.begin();

    while (left_it != lhs.end()) {
        s.insert(*left_it);
        left_it++;
    }
    while (right_it != rhs.end()) {
        s.insert(*right_it);
        right_it++;
    }
    return s;
}


Set operator-(const Set& lhs, const Set& rhs) {
    Set s = Set();
    Set::Iterator left_it = lhs.begin();
    Set::Iterator right_it = rhs.begin();
    bool valInSet;

    while (left_it != lhs.end()) {
        valInSet = false;
        while (right_it != rhs.end()) {
            if (*left_it == *right_it) {
                valInSet = true;
                break;
            }
            right_it++;
        }
        if (valInSet == false) {
            s.insert(*left_it);
        }
        left_it++;
    }
    return s;
}


bool operator==(const Set& s1, const Set& s2) {
    Set::Iterator s1_it = s1.begin();
    Set::Iterator s2_it = s2.begin();
    bool valInSet;

    if (s1.size() != s2.size()) {
        return false;
    }
    
    while (s1_it != s1.end()) {
        valInSet = false;
        while (s2_it != s2.end()) {
            if (*s1_it == *s2_it) {
                valInSet = true;
            }
            s2_it++;
        }
        if (valInSet != true) {
            return false;
        }
        s1_it++;
    }
    return true;
}

ostream& operator<<(ostream& os, Set &v){
    v.dump(os, v);
    return os;
}